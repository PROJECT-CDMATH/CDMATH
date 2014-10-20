CDMATH
======

CDMATH is a CFD toolbox designed for numerical analysts who work on the representation of thermal-hydraulics and who would prefer to focus on high-level computation.


Download CDMATH
---------------
Create your source directory. For instance:
* `mkdir ~/workspace/cdmath`
* `cd ~/workspace/cdmath`

Download from GitHub. For instance:
* `git clone https://github.com/PROJECT-CDMATH/CDMATH.git cdmath_src`



Set environment for the compilation of CDMATH
---------------------------------------------
Dependencies. The following packages list is sufficient on Ubuntu 14.04:
 - `cmake`
 - `libhdf5-dev`
 - `petsc-dev`. This should already include `libopenmpi-dev`, which is necessary if you use the compilation option `-DMEDFILE_USE_MPI=ON`.
 - `python-dev` and `swig`, if you want to generate Python executables and libraries of CDMATH. Use the compilation option `-DCMAKE_CDMATH_SWIG=ON`.
 - `libcppunit-dev`, if you want to generate unit tests. Use the compilation option `-DCMAKE_CDMATH_TESTS=ON`.
 - `doxygen`, `graphviz` and `mscgen`, if you want to generate a nice documentation in `~/workspace/cdmath/cdmath_install/doc/`. Use the compilation option `-DCMAKE_CDMATH_DOCUMENTATION=ON`.
Some users reported that they need `valgrind-dev` and `numpy` on other systems (Fedora), but this has not been confirmed.

If necessary, you may have to set PETSc's directory. This is was not necessary on Ubuntu 14.04. However, if you prefered to compile PETSc from the sources as explained on [the official documentation](http://www.mcs.anl.gov/petsc/documentation/installation.html), then use the following:
* `export PETSC_DIR=/path/to/compiled/source/petsc-3.5.2/bin/`.

Create the suggested build and installation folders:
* `cd ~/workspace/cdmath`
* `mkdir cdmath_build`
* `mkdir cdmath_install`
* `cd cdmath_build`


Compile and install CDMATH
--------------------------
Generate makefiles for a minimum version:
* `cmake -DCMAKE_INSTALL_PREFIX=../cdmath_install -DCMAKE_BUILD_TYPE=Release ../cdmath_src/`

Or generate makefiles for an all-options version:
* `cmake -G"Eclipse CDT4 - Unix Makefiles" -D_ECLIPSE_VERSION=4.3 -DCMAKE_INSTALL_PREFIX=../cdmath_install -DCMAKE_BUILD_TYPE=Release -DCMAKE_CDMATH_SWIG=ON -DCMAKE_CDMATH_TESTS=ON -DCMAKE_CDMATH_DOCUMENTATION=ON -DMEDFILE_USE_MPI=ON ../cdmath_src/`

On some systems (not Ubuntu 14.04), you may have to use the compilation option `-DHDF5_ROOT_DIR=/path/to/hdf5/library` too. Similarly, on some systems (not Ubuntu 14.04), you may have to use the compilation option `-DMPI_ROOT_DIR=/path/to/mpi/library` too. The Cmake options `-G"Eclipse CDT4 - Unix Makefiles" -D_ECLIPSE_VERSION=4.3` create project files if you want to develop CDMATH with Eclipse Kepler or higher.

Compile and install:
* `make -j4` # Where “4” is the number of processors you have.
* `make -j4 install`


Use CDMATH
----------
To use CDMATH with your C++ code `main.cxx`:
 * C++ libraries: `export LD_LIBRARY_PATH=~/workspace/cdmath/cdmath_install/lib`
 * To know how to include the right libraries for compilation, see the makefiles of the examples. They include the list `-linterpkernel -lmedC -lmedloader -lmedcoupling -lbase -lmesh -llinearsolver`.

To use CDMATH with your Python code `main.py`:
 * C++ libraries: `export LD_LIBRARY_PATH=~/workspace/cdmath/cdmath_install/lib`
 * Python libraries: `export PYTHONPATH=~/workspace/cdmath/cdmath_install/lib/cdmath:~/workspace/cdmath/cdmath_install/bin/cdmath`


