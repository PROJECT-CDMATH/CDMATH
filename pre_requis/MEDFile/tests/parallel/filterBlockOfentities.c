/*  This file is part of MED.
 *
 *  COPYRIGHT (C) 1999 - 2016  EDF R&D, CEA/DEN
 *  MED is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  MED is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with MED.  If not, see <http://www.gnu.org/licenses/>.
 */

#define MAX(a,b) ((a) > (b) ? (a) : (b))

#include <med.h>
#define MESGERR 1
#include "med_utils.h"
#include "med_config.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#ifndef HAVE_UNISTD_H
#error "unistd.h reuired."
#endif

#include "getBlocksOfEntitiesPartition.h"
#include "generateDatas.h"
#include "generateFilterArray.h"


#ifdef DEF_LECT_ECR
#define MODE_ACCES MED_ACC_RDWR
#elif DEF_LECT_AJOUT
#define MODE_ACCES MED_ACC_RDEXT
#else
#define MODE_ACCES MED_ACC_CREAT
#endif

/*Valeur de l'enum dans med.h.in*/
/* #define MED_NO_INTERLACE 2 */
/* #define MED_FULL_INTERLACE 1 */


/* #ifndef USER_INTERLACE */
/* #define USER_INTERLACE MED_FULL_INTERLACE */
/* #warning "Defining MED_FULL_INTERLACE mode..." */
/* #endif */

/* #if USER_INTERLACE == MED_NO_INTERLACE */
/* #define generateDatas generateNoIDatas */
/* #warning "Using generateNoIDatas..." */
/* #elif USER_INTERLACE == MED_FULL_INTERLACE */
/* #define generateDatas generateFullIDatas */
/* #warning "Using generateFullIDatas..." */
/* #else */
/* #error "The USER_INTERLACE macro value match neither MED_NO_INTERLACE nor MED_FULL_INTERLACE" */
/* #endif */

/* #define USER_MODE MED_COMPACT */


int main (int argc, char **argv)


{
  med_err _ret=0;
  med_idt _fid;

  int mpi_size, mpi_rank;
  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Info info = MPI_INFO_NULL;

  med_int    _nentitiesfiltered=0;
  med_int    *_filterarray=NULL;


  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);


  med_err generateFieldFile( const med_size nentities, const med_size nvaluesperentity, const med_size nconstituentpervalue,
			     const med_switch_mode constituentmode,GetBlocksOfEntitiesType getBlockOfEntities, const med_int nbblocksperproc,
			     GenerateDataType generateDatas,
			     const med_storage_mode storagemode, const med_size profilearraysize,  const char * const fieldnameprefix ) {

/*     static int   _fileno=0; */
    med_err      _ret=-1;
    char         _filename   [255]="";
    char         _meshname[MED_NAME_SIZE+1]="Empty mesh";
    med_int      _meshdim=3;
    char         _meshcomponentname[3*MED_SNAME_SIZE+1] = "x               y               z               ";
    char         _meshcomponentunit[3*MED_SNAME_SIZE+1] = "cm              cm              cm              ";
    char         _fieldname  [MED_NAME_SIZE+1]="";
    char         *componentname,*componentunit;
    char         _profilename[MED_NAME_SIZE+1]=MED_NO_PROFILE;
    med_int       *_profilearray=0;
    int          _i=0,_j=0,_k=0, _lastusedrank=0;
    med_size     _blocksize=0,_lastblocksize=0,_count=0,_stride=0,_start=0,_index=0;
    med_float    *_arrayvalues;
    med_filter   filter = MED_FILTER_INIT;
    med_size     _nusedentities        = nentities;
    med_size     _io_count                = nbblocksperproc;
    med_idt      _fidseq;

    /*TODO : EXTERNALISER CES DEFINITIONS ET GENERALISER LE PRINCIPE */
    char         *_MED_MODE_SWITCH_MSG[3]={"MED_FULL_INTERLACE", "MED_NO_INTERLACE","MED_UNDEF_INTERLACE",};
    char         *_MED_STORAGE_MODE_MSG[3]={"MED_NO_PFLMODE","MED_GLOBAL_PFLMODE", "MED_COMPACT_PFLMODE"};

    med_geometry_type     _geotype       = MED_TRIA6;
    med_int               _geodim        = _geotype/100;
    med_int               _geonnodes     = _geotype%100;
    char       _ipointname[MED_NAME_SIZE+1];
    med_float* _ipointrefcoo = 0;
    med_int    _ipoint       = nvaluesperentity;
    med_float* _ipointcoo    = 0;
    med_float* _ipointwg     = 0;

    sprintf(_filename,"%s_CPU-%03d_@_%s_%s.med",fieldnameprefix,mpi_size,_MED_MODE_SWITCH_MSG[constituentmode],_MED_STORAGE_MODE_MSG[storagemode]);
/*     SSCRUTE(_filename); */
    /* Ouverture du fichier en mode parallel */
    if ((_fid = MEDparFileOpen(_filename, MODE_ACCES ,comm, info)) < 0){
      MED_ERR_(_ret,MED_ERR_OPEN,MED_ERR_FILE,_filename);
      goto ERROR;
    }

/*     SSCRUTE(_meshname); */
    if (MEDmeshCr( _fid,_meshname,_meshdim,_meshdim, MED_UNSTRUCTURED_MESH,
		   "Un maillage pour le test parallel","s", MED_SORT_DTIT,
		   MED_CARTESIAN, _meshcomponentname, _meshcomponentunit) < 0) {
      MED_ERR_(_ret,MED_ERR_CREATE,MED_ERR_MESH,_meshname);
      goto ERROR;
    };

    componentname = (char*) malloc((nconstituentpervalue*MED_SNAME_SIZE+1)*sizeof(char));
    componentunit = (char*) malloc((nconstituentpervalue*MED_SNAME_SIZE+1)*sizeof(char));
    /*TODO : Compléter le nom */
    strcpy(componentname,"");
    strcpy(componentunit,"");
    strcpy(_fieldname,fieldnameprefix);
    if ( MEDfieldCr(_fid,_fieldname,MED_FLOAT64,nconstituentpervalue,componentname,componentunit,"s",_meshname ) < 0) {
      MED_ERR_(_ret,MED_ERR_CREATE,MED_ERR_FIELD,_fieldname);
      goto ERROR;
    };
    free(componentname);
    free(componentunit);


    if ( _ipoint > 1 ) {

      MESSAGE("Creating a localization of integration points...");
      strcpy(_ipointname,_fieldname);
      strcat(_ipointname,"_loc");

      /*Attention ancienne spec*/
      _ipointrefcoo = (med_float *) calloc(_geodim*_geonnodes,sizeof(med_float));
      _ipointcoo    = (med_float *) calloc(_ipoint*_geodim,sizeof(med_float));
      _ipointwg     = (med_float *) calloc(_ipoint,sizeof(med_float));

      if (MEDlocalizationWr(_fid, _ipointname, _geotype, _geotype/100, _ipointrefcoo, constituentmode,
			    _ipoint, _ipointcoo, _ipointwg, MED_NO_INTERPOLATION, MED_NO_MESH_SUPPORT ) < 0) {
	MED_ERR_(_ret,MED_ERR_CREATE,MED_ERR_LOCALIZATION,_ipointname);
	ISCRUTE_int(constituentmode);
	goto ERROR;
      }
      free(_ipointrefcoo );
      free(_ipointcoo    );
      free(_ipointwg     );

    } else {
      strcpy(_ipointname,MED_NO_LOCALIZATION);
    }

    if (profilearraysize) {
      MESSAGE("Creating a profile...");

      strcpy(_profilename,_fieldname);strcat(_profilename,"_profile");

      _profilearray = (med_int*) calloc(profilearraysize,sizeof(med_int));

      for (_i=0; _i < profilearraysize; ++_i) _profilearray[_i]=_i;
      if ( MEDprofileWr(_fid,_profilename,profilearraysize,_profilearray) < 0) {
	MED_ERR_(_ret,MED_ERR_CREATE,MED_ERR_PROFILE,_profilename);
	goto ERROR;
      };
      _nusedentities = profilearraysize;
    } else {
      strcpy(_profilename,MED_NO_PROFILE);
    }


    MESSAGE("Generating partition...");
    getBlockOfEntities ( mpi_rank , mpi_size, _nusedentities,
			 &_start, &_stride, &_io_count, &_blocksize,
			 &_lastusedrank, &_lastblocksize);

    _count=_io_count;
    MESSAGE("Generating filter...");
    if ( MEDfilterBlockOfEntityCr(_fid, nentities, nvaluesperentity, nconstituentpervalue,
				  MED_ALL_CONSTITUENT, constituentmode, storagemode, _profilename,
				  _start,_stride,_count,_blocksize,_lastblocksize,  &filter) < 0 ) {
	MED_ERR_(_ret,MED_ERR_CREATE,MED_ERR_FILTER,"");
	goto ERROR;
    }

    MESSAGE("Generating datas...");
    generateDatas(mpi_rank, _lastusedrank, sizeof(med_float),
		  storagemode, profilearraysize, _profilearray,
		  _start, _stride, _count, _blocksize, _lastblocksize,
		  nentities, nvaluesperentity, nconstituentpervalue,
		  &_arrayvalues );

    MESSAGE("Writing field...");
    if ( MEDfieldValueAdvancedWr(_fid,_fieldname,MED_NO_DT,MED_NO_IT,0.0, MED_CELL, _geotype,
				 _ipointname, &filter, (unsigned char*)_arrayvalues ) < 0) {
      MED_ERR_(_ret,MED_ERR_WRITE,MED_ERR_FIELD,_fieldname);
      ISCRUTE(mpi_rank);
      goto ERROR;
    }

    /* Test de lecture du même fichier avec filtre simple par un seul processeur */
    /* TODO : Créer MEDflush */
    H5Fflush(_fid, H5F_SCOPE_GLOBAL );

    /*Le flush suffit pas besoin de synchroniser les processus : MPI_Barrier(MPI_COMM_WORLD); */
    if (mpi_rank == 0 ) {
      MESSAGE("Reading field...");


      med_int    _nentitiesarrayvalues=0;
      med_float  *_filteredarrayvalues=NULL;
      med_filter filter2=MED_FILTER_INIT;
      int        _ind=0;
      FILE *     _asciifile;
      char       _asciifilename[255]="";


      if ((_fidseq = MEDfileOpen(_filename, MED_ACC_RDONLY )) < 0){
	MED_ERR_(_ret,MED_ERR_OPEN,MED_ERR_FILE,_filename);
	goto ERROR;
      }

      sprintf(_asciifilename,"%s_CPU-%03d_@_%s_%s.ascii",fieldnameprefix,mpi_size,_MED_MODE_SWITCH_MSG[constituentmode],_MED_STORAGE_MODE_MSG[storagemode]);
      _asciifile=fopen(_asciifilename, "w");

      /*Génère un filtre de selection simple s'il n'a pas déjà été généré lors d'un précédent appel */
      /*TODO : Déplacer cette appel dans le main après avoir externaliser la génération du profile */
      if (!_filterarray)
	if ( generateFilterArray(  nentities,  nvaluesperentity, nconstituentpervalue,
				   profilearraysize, _profilearray,
				   &_nentitiesfiltered, &_filterarray ) < 0 ) {
	  goto ERROR;
	}

      ISCRUTE(_nentitiesfiltered);
      /*Stocke le filtre utilisé dans le fichier .ascii*/
      for (_i=0; _i < _nentitiesfiltered; ++_i ) {
/* 	ISCRUTE(_filterarray[_i]); */
	fprintf(_asciifile,"%d ",_filterarray[_i]) ;
      }
      fprintf(_asciifile,"\n") ;


      /*Pas de profile possible (profilearraysize == 0) en MED_GLOBAL_PFLMODE sur un fichier géré en parallel */
      if ( profilearraysize ) {
	_nentitiesarrayvalues = profilearraysize;
      } else {
	_nentitiesarrayvalues = nentities;
      }

      /*Attention allocation mémoire potentiellement grosse car réalisée uniquement par le processus 0
       qui rassemble les données.*/
      /* C'est une taille maxi qui ne prend pas en compte le COMPACT+filter */
      /* TODO : Ajuster la taille au storage_mode*/
      _filteredarrayvalues = (med_float*) malloc(_nentitiesarrayvalues*
						 nvaluesperentity*
						 nconstituentpervalue*sizeof(med_float));

      /* Permet de vérifier une erreur d'indiçage après la lecture */
      for (_i=0;_i<_nentitiesarrayvalues*nvaluesperentity*nconstituentpervalue; ++_i)
	_filteredarrayvalues[_i]=-_i;


      /*Création d'un filtre de sélection simple, pour une lecture séquentielle par le processys 0*/
      if ( MEDfilterEntityCr(_fidseq, nentities, nvaluesperentity, nconstituentpervalue,
			     MED_ALL_CONSTITUENT, constituentmode, storagemode, _profilename,
			     _nentitiesfiltered,_filterarray, &filter2) < 0 ) {
	MED_ERR_(_ret,MED_ERR_CREATE,MED_ERR_FILTER,"");
	goto ERROR;
      }

      if ( MEDfieldValueAdvancedRd(_fidseq,_fieldname,MED_NO_DT,MED_NO_IT, MED_CELL, _geotype,
				   &filter2, (unsigned char*)_filteredarrayvalues ) < 0) {
	MED_ERR_(_ret,MED_ERR_READ,MED_ERR_FIELD,_fieldname);
	ISCRUTE(mpi_rank);
	goto ERROR;
      }

      /*AFFICHAGE TOUJOURS EN FULL INTERLACE QUELQUES SOIENT LES COMBINAISONS*/
      /*TODO : Externaliser l'affichage*/
      if ( storagemode == MED_GLOBAL_PFLMODE ) {
	switch (constituentmode) {
	case MED_FULL_INTERLACE:
	  for (_i=0; _i < _nentitiesfiltered; ++_i)
	    for (_j=0; _j < nvaluesperentity; ++_j)
	      for (_k=0; _k < nconstituentpervalue; ++_k) {
		_ind = (_filterarray[_i]-1)*nvaluesperentity*nconstituentpervalue+ _j*nconstituentpervalue+_k;
/* 		fprintf(stdout,"%s%3d%s = %f\n","_filteredarrayvaluesFULLGLB[",_ind,"]",_filteredarrayvalues[_ind]) ; */
		fprintf(_asciifile,"%f\n",_filteredarrayvalues[_ind]) ;
	      }
	  break;
	case MED_NO_INTERLACE:
	  for (_j=0; _j < _nentitiesfiltered; ++_j)
	    for (_k=0; _k < nvaluesperentity; ++_k)
	      for (_i=0; _i < nconstituentpervalue; ++_i) {
		_ind =_i*nentities*nvaluesperentity+ (_filterarray[_j]-1)*nvaluesperentity +_k;
/* 		fprintf(stdout,"%s%3d%s = %f\n","_filteredarrayvaluesNOGLB[",_ind,"]",_filteredarrayvalues[_ind]); */
		fprintf(_asciifile,"%f\n",_filteredarrayvalues[_ind]);
	      }
	  break;
	}
      }  else
	switch (constituentmode) {
	case MED_FULL_INTERLACE:
	  for (_i=0; _i < _nentitiesfiltered; ++_i )
	    for (_j=0; _j < nvaluesperentity; ++_j)
	      for (_k=0; _k < nconstituentpervalue; ++_k) {
		_ind = _i*nvaluesperentity*nconstituentpervalue+_j*nconstituentpervalue+_k;
/* 		fprintf(stdout,"%s%3d%s = %f\n","_filteredarrayvaluesFULLCP[",_ind,"]",_filteredarrayvalues[_ind]) ; */
		fprintf(_asciifile,"%f\n",_filteredarrayvalues[_ind]) ;
	  }
	  break;
	case MED_NO_INTERLACE:
	  for (_j=0; _j < _nentitiesfiltered; ++_j)
	    for (_k=0; _k < nvaluesperentity; ++_k)
	      for (_i=0; _i < nconstituentpervalue; ++_i) {
		_ind =_i*_nentitiesfiltered*nvaluesperentity+ _j*nvaluesperentity +_k;
		/* _ind =_i*_nentitiesarrayvalues*nvaluesperentity+ (_filterarray[_j]-1)*nvaluesperentity +_k; */
/* 		fprintf(stdout,"%s%3d%s = %f\n","_filteredarrayvaluesNOCP[",_ind,"]",_filteredarrayvalues[_ind]); */
		fprintf(_asciifile,"%f\n",_filteredarrayvalues[_ind]);
	      }
	  break;
	}


      free(_filteredarrayvalues);

      fclose(_asciifile);

      if ( MEDfilterClose(&filter2) < 0 ) {
	MED_ERR_(_ret,MED_ERR_CLOSE,MED_ERR_FILTER,"");
	goto ERROR;
      }

    } /*fin if (mpi_rank == 0) */

  if ( MEDfilterClose(&filter) < 0 ) {
    MED_ERR_(_ret,MED_ERR_CLOSE,MED_ERR_FILTER,"");
    goto ERROR;
  }


    _ret=0;
  ERROR:
    if (_arrayvalues)     free(_arrayvalues);
    if (profilearraysize) free(_profilearray);

    if (  MEDfileClose(_fid) < 0) {
      MED_ERR_(_ret,MED_ERR_CLOSE,MED_ERR_FILE,""); _ret = -1;
    }

    if (mpi_rank == 0 ) {
      if (  MEDfileClose(_fidseq) < 0) {
	MED_ERR_(_ret,MED_ERR_CLOSE,MED_ERR_FILE,""); _ret = -1;
      }
    }

    return _ret;
  }




  /* MAIN */
  med_size            _nbblocksperproc    = 0;
  int           _nentities             = 0;
  int           _nvaluesperentity      = 0;
  int           _nconstituentpervalue  = 0;

  if (mpi_rank == 0 ) {

    struct tm *_tm ;
    time_t _tt=time(0);
    _tm = localtime(&_tt);

    srandom((*_tm).tm_sec * (*_tm).tm_min );
    _nbblocksperproc         = 1 + (int) (mpi_size * (random() / (RAND_MAX + 1.0)));
    _nentities            = 1 + (int) (1000.0 * (random() / (RAND_MAX + 1.0)));
    _nvaluesperentity     = 1 + (int) (11.0 * (random() / (RAND_MAX + 1.0)));
    _nconstituentpervalue = 1 + (int) (7.0 * (random() / (RAND_MAX + 1.0)));
/*     _nbblocksperproc         = 1 + (int) (mpi_size * (random() / (RAND_MAX + 1.0))); */
/*     _nentities            = 1 + (int) (5.0 * (random() / (RAND_MAX + 1.0))); */
/*     _nvaluesperentity     = 1 + (int) (3.0 * (random() / (RAND_MAX + 1.0))); */
/*     _nconstituentpervalue = 1 + (int) (3.0 * (random() / (RAND_MAX + 1.0))); */
/*     _nbblocksperproc         = 2; */
/*     _nentities            = 4; */
/*     _nvaluesperentity     = 1; */
/*     _nconstituentpervalue = 1; */

  }

  if ( (sizeof(med_size)%(sizeof(MPI_LONG)))==0 ) {

    MPI_Bcast(&_nbblocksperproc         , sizeof(med_size)/sizeof(MPI_LONG), MPI_LONG, 0, MPI_COMM_WORLD);
    MPI_Bcast(&_nentities            , sizeof(med_size)/sizeof(MPI_LONG), MPI_LONG, 0, MPI_COMM_WORLD);
    MPI_Bcast(&_nvaluesperentity     , sizeof(med_size)/sizeof(MPI_LONG), MPI_LONG, 0, MPI_COMM_WORLD);
    MPI_Bcast(&_nconstituentpervalue , sizeof(med_size)/sizeof(MPI_LONG), MPI_LONG, 0, MPI_COMM_WORLD);
  } else {
    assert(sizeof(med_size) == (sizeof(MPI_LONG)));
  }

  char                _fieldnameprefix[256] = "";

  sprintf(_fieldnameprefix,"NENT-%03d_NVAL-%03d_NCST-%03d_NBL-%03llu",_nentities,_nvaluesperentity,
	  _nconstituentpervalue,_nbblocksperproc);

  GenerateDataType generateDatas = 0;
  med_switch_mode  _switchmode  = MED_UNDEF_INTERLACE;
  med_storage_mode _storagemode = MED_UNDEF_PFLMODE;
  /*Pour que les 4 fichiers générés soient identiques, on désactive l'utilisation des profils
    qui n'est pas utilisable en mode GLOBAL et // */
  med_int          _profilearraysize=0;
  /*   med_int       _profilearraysize=_nentities/2; */

  for (_switchmode = MED_FULL_INTERLACE ; _switchmode <= MED_NO_INTERLACE; ++_switchmode) {

    if ( _switchmode == MED_FULL_INTERLACE ) generateDatas = generateFullIDatas;
    else generateDatas = generateNoIDatas;

    for (_storagemode = MED_GLOBAL_PFLMODE ; _storagemode <= MED_COMPACT_PFLMODE; ++_storagemode) {

      if ( (_storagemode == MED_GLOBAL_PFLMODE ) && (_profilearraysize) ) _profilearraysize=0;

      if ( generateFieldFile( _nentities,  _nvaluesperentity, _nconstituentpervalue,
			      _switchmode, getCyclicBlocksOfEntities, _nbblocksperproc, generateDatas,
			      _storagemode, _profilearraysize,  _fieldnameprefix ) < 0 ) {
	MED_ERR_(_ret,MED_ERR_WRITE,MED_ERR_FIELD,_fieldnameprefix);
	ISCRUTE(mpi_rank);
	goto ERROR;
      }

    }
  }


  _ret = 0;
 ERROR:

  if (_filterarray) free(_filterarray);

  /*pour arch. BLueGene : Sync entre GPFS et LSF : sleep(360) */

  /* MPI_Finalize must be called AFTER MEDclose which may use MPI calls */
  MPI_Finalize();

  /* Catcher l'erreur en retour mpirun  et .sh*/
  return _ret;
}




