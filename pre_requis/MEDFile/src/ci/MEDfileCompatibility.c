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


/**\ingroup MEDfile
  \brief \MEDfileCompatibilityBrief
  \param filename \filename
  \param hdfok \hdfok
  \param medok \medok
  \retval med_err \error
  \details \MEDfileCompatibilityDetails
 */

med_err
MEDfileCompatibility(const char* const filename,
		     med_bool* const hdfok,
		     med_bool* const medok)
{
  med_err _ret = -1;
  med_idt _fid =  0;
  med_idt _id  =  0;
  med_int _major;
  med_int _minor;

  _MEDmodeErreurVerrouiller();

  /* compatibility with hdf5 */
  if  ( H5Fis_hdf5(filename) > 0 )
    *hdfok = MED_TRUE;
  else
    *hdfok = MED_FALSE;

  /* compatibility with med */
  if ((_fid = _MEDfileOpen((char *)filename,MED_ACC_RDONLY)) < 0) {
    MED_ERR_(_ret,MED_ERR_OPEN,MED_ERR_FILE,filename);
    SSCRUTE(filename);
    goto ERROR;
  }

  if ((_id = _MEDdatagroupOuvrir(_fid,MED_INFOS)) < 0) {
    MED_ERR_(_ret,MED_ERR_OPEN,MED_ERR_DATAGROUP,MED_INFOS);
    goto ERROR;
  }

  if (_MEDattrEntierLire(_id,MED_MAJOR_NAME,&_major) < 0) {
    MED_ERR_(_ret,MED_ERR_READ,MED_ERR_ATTRIBUTE,MED_MAJOR_NAME);
    goto ERROR;
  }

  if (_MEDattrEntierLire(_id,MED_MINOR_NAME,&_minor) < 0) {
    MED_ERR_(_ret,MED_ERR_READ,MED_ERR_ATTRIBUTE,MED_MINOR_NAME);
    goto ERROR;
  }

  *medok = MED_FALSE;
  if (*hdfok)
    if ((_major > 2) || ((_major == 2) && (_minor > 1)))
      *medok = MED_TRUE;

  _ret = 0;
 ERROR:

  if (_id > 0)
    if (_MEDdatagroupFermer(_id) < 0) {
      MED_ERR_(_ret,MED_ERR_CLOSE,MED_ERR_DATAGROUP,MED_INFOS);
      _ret = -1;
    }
  
  if (_fid > 0)
    if (MEDfileClose(_fid) < 0) {
      MED_ERR_(_ret,MED_ERR_CLOSE,MED_ERR_FILE,filename);
      _ret = -1;
    }

  return _ret;
}
