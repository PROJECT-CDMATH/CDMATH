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


#include <stdio.h>
#include <med_config.h>
#include <med_misc.h>
#include <med_utils.h>

extern MedFuncType getVersionedApi3(const char * const keycharpart,
				    const char * const keynumpart);


/*  La gestion suivante suppose de ne pas utiliser des versions > 9.9.9 */
/*  ex 9.10.9 ne peut pas être géré. */
MedFuncType _MEDversionedApi3( const char * const key,
			       const med_int majeur,const  med_int mineur,const  med_int release) {

  MedFuncType func=(MedFuncType) NULL;
  char        _version[4]="", _version236[4]="236";
  int         _n=0;
  int         _fversionMM  = 100*majeur+10*mineur;
  int         _fversionMMR = _fversionMM+release;
  int         _litminor    = mineur;
  const int   _lminminor   = 0;

  if ( _fversionMMR <  220 ) {
    MESSAGE("Cette bibliothèque MED n'est pas capable de lire un fichier MED de version < 2.2.0");
    MESSAGE("La version demandée est :");
    ISCRUTE_int(_fversionMMR);
    goto QUIT;
  }

  if ( _fversionMM > 100*MED_NUM_MAJEUR+10*MED_NUM_MINEUR ) {
    MESSAGE("Cette bibliothèque MED n'est pas capable de lire un fichier MED dont le mineur de la version "
	    "excède celui de la bibliothèque.");
    MESSAGE("La version demandée est :");ISCRUTE_int(_fversionMMR);
    goto QUIT;
  }



  /* Dans la bibliothèque 3.0 toutes les APIs en lecture/info
   *  sont versionées en 2.3.6. Celles-ci sont capables de compatibilité
   * ascendante des fichiers 2.2.0 à 2.3.6 */
  if ( _fversionMMR <= 236 ) { func=getVersionedApi3(key,_version236); goto QUIT;}


  /*  - Recherche du dernier numéro mineur disponible pour la routine versionné
      dans la bibliothèque MED actuelle  versionMMR 100*MED_NUM_MAJEUR+10*MED_NUM_MINEUR
      -A partir de la 2.4.0, on oblige le developpeur à versionner
      uniquement des routines en x.y.0 ;  x.y+1.* et x.y.* étant incompatible
      et  x.y.0 et x.y.a étant compatible, x.y.a n'apparait pas dans la table
      de versionement
  */

  /*
   * Lorsqu'une routine non versionnée doit être versionnée, la nouvelle
   * implémentation est numérotée en "MED_NUM_MAJEUR"MED_NUM_MINEUR" et l'ancienne
   * en "MED_NUM_MAJEUR".0
   *
   */

  /*
   * La recherche d'une routine compatible se fait de façon décroissante
   * à partir du mineur du fichier jusqu'à 0 ou une poentielle rupture de compatibilité
   * (le mineur a changé)
   *
   */


  /*Rem (_fversionMMR > 290) car cette version est une version de developpement  et aucun fichier
    de ce numéro ne devrait être en circulation.*/
  if ( (_fversionMMR > 290) ||
       (_fversionMM <= (100*MED_NUM_MAJEUR+10*MED_NUM_MINEUR) )
       ) {

    /* Recherche décroissante à partir du numéro de release de la bibliothèque */
    while ( ( func == (MedFuncType)NULL) && (_litminor >= _lminminor ) ) {

#ifdef PPRO_NT
      _n = _snprintf(_version,4,"%d%d%d",(int) majeur,_litminor,0);
#else
      _n = snprintf(_version,4,"%d%d%d",(int) majeur,_litminor,0);
#endif
      if ( (_n < 0) || (_n > 3) ) {
	MESSAGE("Impossible d'obtenir un numéro de version valide : ");
	_version[3]='\0';SSCRUTE(_version);
	break;
      }

      func=getVersionedApi3(key,_version);
      --_litminor;
    }
    goto QUIT;
  }


 QUIT:

  if (func == NULL) {
    MESSAGE("Impossible d'obtenir une implémentation de : ");
    SSCRUTE(key);
    MESSAGE("en version :");
    ISCRUTE_int(_fversionMMR);
    MESSAGE("Vérifiez votre fichier .med");
  }

  return func;
}


