CDMATH
======

CDMATH est une boîte à outils conçue pour les numériciens travaillant sur la modélisation thermo-hydraulique et désirant se concentrer sur un calcul de plus haut niveau. La bibliothèque est maintenue et utilisée par [CDMATH](http://cdmath.jimdo.com), un groupe de travail portant le même nom. La bibliothèque est le plus facile à installer sur Ubuntu 12.04, 14.04 et 14.10, sur Debian Jessie, ainsi que sur Fedora 20, 21 et 22.


Télécharger les binaires de CDMATH depuis les dépôts
----------------------------------------------------
Si vous êtes sous Ubuntu 14.04, vous pouvez simplement ajouter notre  [dépôt Ubuntu](https://launchpad.net/~cdmath/+archive/ubuntu/cdmath) à votre système.

De façon similaire, si vous êtes sous Fedora 20, 21 ou 22, alors vous pouvez simplement télécharger un paquet RPM depuis notre [dépôt Fedora](https://build.opensuse.org/project/repositories/home:ArthurTalpaert).


Télécharger les sources de CDMATH pour les compiler
---------------------------------------------------
Si vous êtes sous un autre système, ou bien si vous préférez compiler les sources les plus récentes pour tirer bénéfice de nos dernières améliorations, veuillez suiver les instructions ci-dessous.

Créez un répertoire pour les sources. Par exemple :
* `mkdir ~/workspace/cdmath`
* `cd ~/workspace/cdmath`

Téléchargez depuis GitHub. Par exemple :
* `git clone https://github.com/PROJECT-CDMATH/CDMATH.git cdmath_src`


Initialiser l'environnement pour la compilation de CDMATH
---------------------------------------------------------
Dépendances. La liste suivante de paquets est suffisante pour Ubuntu 14.04, Ubuntu 14.10 et Debian Jessie :
 - `cmake`
 - `g++` ou un autre compilateur C++
 - `libhdf5-dev`
 - `libopenmpi-dev`, en particulier si vous avez besoin d'utiliser l'option de compilation `-DMEDFILE_USE_MPI=ON`.
 - `petsc-dev`, si vous désirez compiler un solver linéaire basé sur CDMATH. PETSc doit obligatoirement être à une sous-version mineure de la version 3.4. *, c'est-à-dire surtout *pas* plus récente. Nous avons choisi la version 3.4. * de PETSc (disponible depuis le 13 mai 2013) car elle est facilement installable depuis les dépôts des distributions principales. Utilisez l'option de compilation `-DCDMATH_WITH_PETSC=ON`.
 - `python-dev`, `python-numpy` et `swig`, si vous désirez générer des exécutables et des bibliothèqus Python pour CDMATH. Utilisez l'option de compilation `-DCDMATH_WITH_PYTHON=ON`.
 - `libcppunit-dev`, si vous désirez générer des tests unitaires. Utilisez l'option de compilation `-DCDMATH_WITH_TESTS=ON`.
 - `doxygen`, `graphviz` et `mscgen`, si vous désirez générer une documentation agréable dans `~/workspace/cdmath/cdmath_install/doc/`. Utilisez l'option de compilation `-DCDMATH_WITH_DOCUMENTATION=ON`.
 - `rpm`, si vous désirez générer des paquets d'installation RPM. Utilisez l'option de compilation `-DCDMATH_WITH_PACKAGE=ON`.
 
 Répertoires. Créez les répertoires suggérés pour la compilation (*build*) et l'installation :
* `cd ~/workspace/cdmath`
* `mkdir cdmath_build`
* `mkdir cdmath_install`
* `cd cdmath_build`


Compiler et installer CDMATH
----------------------------
Générez les fichiers *makefile* pour une version minimum :
* `cmake ../cdmath_src/ -DCMAKE_INSTALL_PREFIX=../cdmath_install -DCMAKE_BUILD_TYPE=Release`

Ou bien générer les fichiers *makefile* avec toutes les options activées :
* `cmake ../cdmath_src/ -DCMAKE_INSTALL_PREFIX=../cdmath_install -DCMAKE_BUILD_TYPE=Release -G"Eclipse CDT4 - Unix Makefiles" -D_ECLIPSE_VERSION=4.3 -DMEDFILE_USE_MPI=ON -DCDMATH_WITH_PETSC=ON -DCDMATH_WITH_PYTHON=ON -DCDMATH_WITH_TESTS=ON -DCDMATH_WITH_DOCUMENTATION=ON -DCDMATH_WITH_PACKAGE=ON`

Compilez et installez :
* `make`
* `make install`

Notes pour les options de compilation :
* Eclipse: Les options Cmake `-G"Eclipse CDT4 - Unix Makefiles" -D_ECLIPSE_VERSION=4.3` créent des fichiers de projet si vous voulez développer CDMATH avec Eclipse Kepler ou plus récent.
* HDF5: Sur certains systèmes (pas Ubuntu 14.04), vous pouvez avoir besoin d'utiler aussi l'option de compilation `-DHDF5_ROOT_DIR=/chemin/vers/la/bibliothèque/hdf5`.
* MPI: Sur certains systèmes (pas Ubuntu 14.04), vous pouvez avoir besoin d'utiler aussi l'option de compilation `-DMPI_ROOT_DIR=/chemin/vers/la/bibliothèque/mpi`. Vous pouvez aussi avoir besoin d'initialiser la variable d'environnement `export MPI_ROOT_DIR=/chemin/vers/la/bibliothèque/mpi`. De plus, ur certains systèmes (pas Ubuntu 14.04)ur certains systèmes (pas Ubuntu 14.04), l'option de compilation `-DMEDFILE_USE_MPI=ON` peut être obligatoire et être initialisée à `ON`.
* PETSc: Sur certains systèmes (pas Ubuntu 14.04), vous pouvez avoir besoin d'utiler aussi l'option de compilation `-DPETSC_DIR=/chemin/vers/l'installation/de/petsc-3.4.5/ -DPETSC_ARCH=arch-linux2-c-opt`. Adaptez en fonction de votre système, car cela peut aussi être `-DPETSC_DIR=/chemin/vers/l'installation/de/petsc-3.4.5_install/` par exemple. Vous pouvez aussi avoir besoin d'utiliser `export PETSC_DIR=/chemin/vers/les/sources/compilées/petsc-3.4.5/bin/` si vous avez compilé PETSc depuis les sources comme expliqué par [la documentation officielle](http://www.mcs.anl.gov/petsc/documentation/installation.html).

