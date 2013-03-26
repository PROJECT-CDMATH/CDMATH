CDMAT
=====

CFD TOOLBOX

--------------------------------------------------------------------------------------------------
cd /home/moi/codes/ (par exemple)

- Chekout CDMATH : 

git@github.com:PROJETC-CDMATH/CDMAT.git

Ou recuperer un .zip a partir du site github

-> le code CDMATH est recupere

--------------------------------------------------------------------------------------------------
- Compilation :

cd /home/moi/codes/
mkdir build
mkdir install
cd build

cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_INSTALL_PREFIX=/home/moi/codes/install -DCMAKE_BUILD_TYPE=DEBUG /home/moi/codes/CDMATH

Rajouter -DCMAKE_CDMATH_SWIG=ON si on veut utiliser la toolbox en python

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


