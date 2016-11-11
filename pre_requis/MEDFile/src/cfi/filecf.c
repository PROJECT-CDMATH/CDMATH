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


#include <med.h>
#include "med_config.h"
#include "med_outils.h"
#include <string.h>
#include <stdlib.h>

extern void *MedVersionedApi3F;
extern void f77Api3IsSet(void * obj);

/*
From Fortran call of following C functions : 
- MEDfileOpen
- MEDfileClose      
- MEDfileCommentWr   
- MEDfileCommentRd    
- MEDfileNumVersionRd  
- MEDfileStrVersionRd
- MEDfileCompatibility  
- MEDfileObjectsMount  
- MEDfileObjectsUnmount  
*/

#define nmfifope F77_FUNC(mfifope,MFIFOPE)
#define nmfifclo F77_FUNC(mfifclo,MFIFCLO)
#define nmfifnam F77_FUNC(mfifnam,MFIFNAM)
#define nmfifcow F77_FUNC(mfifcow,MFIFCOW)
#define nmfifcor F77_FUNC(mfifcor,MFIFCOR)
#define nmfifnvr F77_FUNC(mfifnvr,MFIFNVR)
#define nmfifsvr F77_FUNC(mfifsvr,MFIFSVR)
#define nmfifcom F77_FUNC(mfifcom,MFIFCOM)
#define nmfifomn F77_FUNC(mfifomn,MFIFOMN)
#define nmfifoun F77_FUNC(mfifoun,MFIFOUN)


#ifdef PPRO_NT
med_int
MFIFOPE(const char const *name,
		const unsigned int bidon,
		  const int const *access, 
		  const med_int const *len)
#else
med_int
nmfifope (const char const *name,
	  const int const *access, 
	  const med_int const *len)
#endif
{
  char *_fn;
  med_int _ret;
  med_access_mode _access;

  /* Cette méthode a pour unique but de forcer la construction
   de l'objet MedVersionedApi3F et donc la mise à jour de MedVersionedApi3
  avec les APis fortran. Certains compilateurs suppriment le symbole MedVersionedApi3F
  croyant qu'il nest pas utilisé et la mise à jour de MedVersionedApi n'est
  alors pas effectuée.*/
  f77Api3IsSet(MedVersionedApi3F);

  _fn = _MED2cstring((char *) name, (int) *len);
  if (!_fn)
	return(-1);
  _access = (med_access_mode) *access;
  
  _ret = (med_int) MEDfileOpen(_fn, _access); 

  _MEDcstringFree(_fn);

  return(_ret); 
}



#ifdef PPRO_NT
med_int
MFIFNAM(med_int *fid,
	char const *filename,
	const unsigned int bidon,
	/* const med_int const *filenamesize,  */
  	const med_int const *filenamelen)
#else
med_int
nmfifnam(med_int *fid,
	char const *filename,
	/* const med_int const *filenamesize,  */
        const med_int const *filenamelen)
#endif
{
  char *_fn=NULL;
  med_int _ret=-1;

  /* ISCRUTE(*filenamesize); */
  /* ISCRUTE(*filenamelen); */
  /* ISCRUTE_id(*fid); */
  _ret = MEDfileName((med_idt) *fid, NULL, 0);
  /* ISCRUTE(_ret); */
  /* if ( (_ret<0) || ( (*filenamesize) == 0) ) return _ret; */
  if ( (_ret<0) || ( (*filenamelen) == 0) ) return _ret;
  if ( (*filenamelen != 0) && 
       ( (_ret) > (*filenamelen) ) ) return -1;

  char *_fs1=(char *) malloc(_ret*sizeof(char)+1);

  _ret = (med_int) MEDfileName((med_idt) *fid,_fs1, *filenamelen); 
  /* SSCRUTE(_fs1); */
  /* ISCRUTE(*filenamesize); */
  /* ISCRUTE_id(*fid); */
  _MEDc2fString(_fs1,(char*)filename,*filenamelen);
  free(_fs1);

  return(_ret); 
}



#ifdef PPRO_NT
med_int
MFIFCLO(med_int *fid)
#else
med_int
nmfifclo(med_int *fid)
#endif
{
  med_int _ret;

  _ret = (med_int) MEDfileClose((med_idt) *fid);

  return(_ret);
}



#ifdef PPRO_NT
med_int
MFIFCOW(const med_int const *fid,
		  const char const *comment,
		  const unsigned int bidon,
		  const med_int *commentlen)
#else
med_int
nmfifcow(const med_int const *fid,
	 const char const *comment,
	 const med_int *commentlen)
#endif
{
  med_int _ret;
  char* _fs1;

  _fs1 = _MED2cstring((char *) comment, (med_int) *commentlen);
  if (!_fs1)
	return(-1);

  _ret = (med_int) MEDfileCommentWr((med_idt) *fid, _fs1);

  _MEDcstringFree(_fs1);

  return(_ret);
}



#ifdef PPRO_NT
med_int
MFIFCOR(const med_int const *fid,
		  char const *comment,
		  const unsigned int bidon,
		  const med_int *commentlen)
#else
med_int
nmfifcor(const med_int const *fid,
	 char const *comment,
	 const med_int *commentlen)
#endif
{
  med_int _ret;
  char _fs1[MED_COMMENT_SIZE+1]="";

  _ret = (med_int) MEDfileCommentRd((med_idt) *fid,_fs1);

  _MEDc2fString(_fs1,(char*)comment,*commentlen);

  return(_ret);
}



#ifdef PPRO_NT
med_int
MFIFNVR(const med_int const *fid,
		  med_int* const major, 
		  med_int* const minor, 
		  med_int* const release)
#else
med_int
nmfifnvr(const med_int const *fid,
	 med_int* const major, 
	 med_int* const minor, 
	 med_int* const release)
#endif
{
  med_int _ret;
  
  _ret = (med_int) MEDfileNumVersionRd((med_idt) *fid,major,minor,release);

  return(_ret);
}



#ifdef PPRO_NT
med_int
MFIFSVR(const med_int const *fid,
		  char* const medversion, 
		  const unsigned int bidon, 
		  const med_int const *len)
#else
med_int
nmfifsvr(const med_int const *fid,
	 char* const medversion, 
	 const med_int const *len)
#endif
{
  med_int _ret;
  char    _fs1[20]="";

  _ret = (med_int) MEDfileStrVersionRd((med_idt) *fid, _fs1);

  _MEDc2fString(_fs1,medversion,*len);

  return(_ret);
}


#ifdef PPRO_NT
med_int
MFIFCOM(const char const *name,
		    const unsigned int bidon,
		    const med_int const *len,
		    med_int *hdfok,
		    med_int *medok)
#else
med_int
nmfifcom(const char const *name,
	 const med_int const *len,
	 med_int *hdfok,
	 med_int *medok)
#endif
{
  med_int _ret;
  med_bool _medok, _hdfok;
  char *_fs1;

  _fs1 = _MED2cstring((char *) name, (int) *len);
  if (!_fs1)
	return(-1);

  _ret = (med_int) MEDfileCompatibility(_fs1,&_hdfok,&_medok);

  *hdfok = (med_int) _hdfok;
  *medok = (med_int) _medok;

  _MEDcstringFree(_fs1);

  return(_ret);
}


#ifdef PPRO_NT
med_int
MFIFOMN (med_int  *fid,
		   char *fname,
		   unsigned int bidon,
		   med_int *len,
		   med_int *class)
#else
med_int
nmfifomn(med_int  *fid,
	 char *fname,
	 med_int  *len,
	 med_int *class)
#endif
{
  char *_fn;
  med_int _ret;
  med_class _class = (med_class) *class;

  _fn = _MED2cstring((char *) fname, (int) *len);
  if (!_fn)
    return(-1);

  _ret = (med_int) MEDfileObjectsMount((med_idt) *fid, _fn, _class );

  _MEDcstringFree(_fn);

  return(_ret); 
}

#ifdef PPRO_NT
med_int
MFIFOUN (const med_int const *fid,
		   med_int const *mid,
		   const med_int const *class)
#else
med_int
nmfifoun(const med_int const *fid,
	 med_int const *mid,
	 const med_int const *class)
#endif
{
  med_int _ret;
  med_class _class = (med_class) *class;

  _ret = (med_int) MEDfileObjectsUnmount((med_idt) *fid, 
					 (med_idt) *mid,
					 _class);

  return(_ret); 
}
