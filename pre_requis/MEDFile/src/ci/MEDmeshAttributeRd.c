/*  This file is part of MED.
 *
 *  COPYRIGHT (C) 1999 - 2012  EDF R&D, CEA/DEN
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
#include <string.h>

/**\ingroup MEDmesh
  \brief \MEDmeshAttributeRdBrief
  \param fid \fid
  \param meshname \meshname
  \param isolatednodes \isolatednodes
  \param verticesnodes \verticesnodes
  \param cellmaxnodes \cellmaxnodes
  \retval med_err \error
  \details \MEDmeshAttributeRdDetails
 */

med_err
MEDmeshAttributeRd(const med_idt fid,
		   const char * const meshname,
		   med_int *isolatednodes,
		   med_int *verticesnodes,
		   med_int *cellmaxnodes)
{
  med_err _ret=-1;
  med_idt _meshid;
  char    _path [MED_TAILLE_MAA+MED_NAME_SIZE+1]=MED_MAA;


  /*
   * On inhibe le gestionnaire d'erreur
   */
  _MEDmodeErreurVerrouiller();


  /*
   * Si le maillage n'existe pas => erreur
   */
  strcat(_path,meshname);
  if ((_meshid = _MEDdatagroupOuvrir(fid,_path)) < 0) {
    MED_ERR_(_ret,MED_ERR_OPEN,MED_ERR_DATAGROUP,_path);
    ISCRUTE_id(_meshid);goto ERROR;
  }


  /*
   * Creation de l'attribut "Nombre de Noeuds Isoles"
   */
  if ( _MEDattrEntierLire(_meshid,MED_NOM_NNI,isolatednodes) < 0 ) {
    _ret = MED_ERR_READ+MED_ERR_ATTRIBUTE;goto ERROR;
  }

  /*
   * Creation de l'attribut "Nombre de Noeuds Sommets"
   */
  if ( _MEDattrEntierLire(_meshid,MED_NOM_NNS,verticesnodes) < 0 ) {
    _ret=MED_ERR_READ+MED_ERR_ATTRIBUTE;goto ERROR;
  }


  /*
   * Creation de l'attribut "Nombre de Noeuds Max par maille"
   */
  if ( _MEDattrEntierLire(_meshid,MED_NOM_NNM,cellmaxnodes) < 0 ) {
    _ret=MED_ERR_READ+MED_ERR_ATTRIBUTE;goto ERROR;
  }


  _ret = 0;
 ERROR:

  if (_meshid>0)            if (_MEDdatagroupFermer(_meshid) < 0) {
    MED_ERR_(_ret,MED_ERR_CLOSE,MED_ERR_DATAGROUP,_path);
    ISCRUTE_id(_meshid);
  }

  return _ret;
}

