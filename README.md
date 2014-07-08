CDMATH
======

CFD TOOLBOX

--------------------------------------------------------------------------------------------------
cd /home/moi/codes/ (par exemple)

- Checkout CDMATH : 

git@github.com:PROJECT-CDMATH/CDMATH.git

Ou récuperer un .zip à partir du site Github

-> le code CDMATH est récupéré

--------------------------------------------------------------------------------------------------
- Compilation :

cd /home/moi/codes/
mkdir build
mkdir install
cd build


Pour utiliser le module linearsolver (résolution des systèmes linéaires), le pre-requis PETSC (version 3.4.5) est nécessaire. Le chemin vers votre librairie PETSC doit être induiquer via la variable d'environnement PETSC_DIR (setenv ou export).

cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_INSTALL_PREFIX=/home/moi/codes/install -DCMAKE_BUILD_TYPE=DEBUG /home/moi/codes/CDMATH

Rajouter -DCMAKE_CDMATH_SWIG=ON si on veut utiliser la toolbox en Python

Rajouter -DCMAKE_CDMATH_TESTS=ON si on veut lancer les tests unitaires

make -j4

make -j4 install

--------------------------------------------------------------------------------------------------
Pour utiliser avec votre code main.cxx:
rajouter 
 * librarires: LD_LIBRARY_PATH /home/moi/codes/install/lib
 * include: /home/moi/codes/install/include

Pour utiliser avec votre code main.py:
rajouter 
 * librarires: LD_LIBRARY_PATH /home/moi/codes/install/lib
               PYTHONPATH /home/moi/codes/install/lib/cdmath:/home/moi/codes/install/bin/cdmath


