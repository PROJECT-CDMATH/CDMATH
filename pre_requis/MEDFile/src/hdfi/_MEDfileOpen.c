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
#include <med_config.h>
#include <med_outils.h>
#include <hdf5.h>


med_idt _MEDfileOpen(const char * const filename,const med_access_mode accessmode)
{

  med_idt _fid =-1;
  int     _hdf_mode=-1;
  hid_t   _fapl    = H5P_DEFAULT;

H5AC_cache_config_t config;

  switch(accessmode)
    {
    case MED_ACC_RDWR :
    case MED_ACC_RDEXT    :
      _hdf_mode = MED_ACC_RDWR;
      break;

    case MED_ACC_RDONLY :
      _hdf_mode = H5F_ACC_RDONLY;
      break;

    default :
      MED_ERR_(_fid,MED_ERR_RANGE,MED_ERR_ACCESS,filename);
      goto ERROR;
    }

  if ( (_fapl = H5Pcreate (H5P_FILE_ACCESS)) < 0 ) {
    MED_ERR_(_fid,MED_ERR_CREATE,MED_ERR_PROPERTY,MED_ERR_FILEVERSION_MSG);
    goto ERROR;
  }


  /* Ce ligne présente depuis la 3.0 impose l'utilisation du modèle
   de données HDF 1.8 pour :
  - Utiliser les nouvelles représentations plus efficaces que dans
  les versions précédentes
  - Empêcher l'utilisation de nvlle représentations d'une future bibliothèque hdf 1.9 :
  qui posèrerait d'eventuels problèmes de relecture aux bibliothèques med
  utilisant encore la 1.8 (ce choix doit être manuel) :
  Les fichier HDF 1.8 utilisent la nouvelle représentation des liens au sein des groupes :
  compact (header) ou dense (hors header)  */
  if ( H5Pset_libver_bounds( _fapl, H5F_LIBVER_18, H5F_LIBVER_18 ) ) {
    MED_ERR_(_fid,MED_ERR_INIT,MED_ERR_PROPERTY,MED_ERR_FILEVERSION_MSG);
    goto ERROR;
  }

  if ((_fid = H5Fopen(filename,_hdf_mode,_fapl)) < 0) {
    MED_ERR_(_fid,MED_ERR_OPEN,MED_ERR_FILE,"");
    ISCRUTE_int(accessmode);
/* Ne pas activer la ligne suivante en production, car certains code utlisent MEDfileOpen
pour tester la présence d'un fichier */
/*    H5Eprint1(stderr); */
    goto ERROR;
  }

/*   if ((_fid = H5Fopen(filename,_hdf_mode,H5P_DEFAULT)) < 0) { */
/*     MED_ERR_(_fid,MED_ERR_OPEN,MED_ERR_FILE,""); */
/*     ISCRUTE_int(accessmode); */
/*     H5Eprint1(stderr); */
/*     goto ERROR; */
/*   } */

  if ( H5Pclose(_fapl) < 0 ) {
    MED_ERR_(_fid,MED_ERR_CLOSE,MED_ERR_PROPERTY,"");
    _fid=-1;goto ERROR;
  }

/* Adjust the size of metadata cache */
/* config.version = H5AC__CURR_CACHE_CONFIG_VERSION; */
/* H5Fget_mdc_config(_fid, &config); */
/* config.set_initial_size = 1; */
/* config.initial_size = 8*1024*1024; */
/* config.max_size = 16*1024*1024; */
/* H5Fset_mdc_config(_fid, &config); */

  _MEDsetModeAcces(_fid,accessmode);
  /* Si le fichier _fid ne possède pas la structure MED_INFOS;
     le fichier est considéré en version 0.0.0 mais n'est pas inscrit
     en cache de version.
  */
  _MEDfileVersion(_fid);

 ERROR:

  return _fid;
}
