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
#include <med_utils.h>
#include <stdlib.h>



int main(int argc, char *argv[]) {
  med_idt fid;
  med_int majeur, mineur, release;
  med_err ret=-1;
  med_bool hdfok=MED_FALSE;
  med_bool medok=MED_FALSE;

  if (argc != 2) {
    fprintf(stdout,">> Utilisation : medconforme <nom_de_fichier_med> \n");
    return 0;
  }

  /*
   * Quelle version de la bibliotheque MED est utilisee ?
   */
  ret=MEDlibraryNumVersion(&majeur, &mineur, &release);
  EXIT_IF( ret<0 ,
	   "Erreur d'appel de la routine MEDlibraryNumVersion.", NULL	);
  fprintf(stdout,"- Version de MED-fichier utilisée par medconforme : "IFORMAT"."IFORMAT"."IFORMAT" \n",majeur,mineur,release); 

  /*
   * Le fichier à lire est-il au bon format de fichier HDF ?
   */

  EXIT_IF(MEDfileCompatibility(argv[1],&hdfok,&medok) < 0,
	  "Fichier non conforme.", NULL);
  if ( hdfok )
    fprintf(stdout,"- Format HDF du fichier MED [%s] conforme au format HDF utilisé par la bibliothèque \n",argv[1]);
  else {
    fprintf(stdout,"- Format HDF du fichier MED [%s] non conforme au format HDF utilisé par la bibliothèque \n",argv[1]);
    return -1;
  }

  /*
   * Le fichier à lire a-t-il été créé avec une version de la bilbiothèque MED conforme avec celle utilisée ?
   * (Numéros majeur et mineur identiques).
   */
  if ( medok )
    fprintf(stdout,"- Version MED du fichier [%s] conforme a la bibliothèque MED utilisée \n",argv[1]);
  else
    fprintf(stdout,"- Version MED du fichier [%s] non conforme avec celle de la bibliothèque utilisée \n",argv[1]);

  fid = MEDfileOpen(argv[1],MED_ACC_RDONLY);
  if (fid < 0)
    fprintf(stderr,">> ERREUR a l'ouverture du fichier [%s] \n",argv[1]);

  /*
   * Une fois le fichier ouvert on peut avoir acces au numero de version complet
   */
  ret = MEDfileNumVersionRd(fid, &majeur, &mineur, &release);
  EXIT_IF(ret < 0,"Lecture du numéro de version",NULL);

  fprintf(stdout,"- Ce fichier a ete créé avec MED V"IFORMAT"."IFORMAT"."IFORMAT" \n",majeur,mineur,release);

  ret = MEDfileClose(fid);
  EXIT_IF(ret < 0,"Fermeture du fichier",argv[1]);

  return 0;
}
