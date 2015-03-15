CDMATH
======

CDMATH is a CFD toolbox designed for numerical analysts who work on the representation of thermal-hydraulics and who would prefer to focus on high-level computation. The software is maintained and used by [CDMATH](http://cdmath.jimdo.com), a collaborative workgroup with the same name. The software is easiest to install on Ubuntu 12.04, 14.04 and 14.10, on Debian Jessie, as well as on Fedora 19, 20 and 21.


Download binary CDMATH from repositories
----------------------------------------
If you are on Ubuntu 14.04, you can simply add our [Ubuntu repository](https://launchpad.net/~cdmath/+archive/ubuntu/ppa) to your system.

Similarly, if you are on Fedora 19, 20 or 21, then you can also simply download an RPM package from our [Fedora repository](https://build.opensuse.org/project/repositories/home:ArthurTalpaert).


Download CDMATH sources to compile
----------------------------------
If you are on another system, or if you prefer to compile the latest sources to benefit from our continuous improvement, please follow the instructions hereunder.

Create your source directory. For instance:
* `mkdir ~/workspace/cdmath`
* `cd ~/workspace/cdmath`

Download from GitHub. For instance:
* `git clone https://github.com/PROJECT-CDMATH/CDMATH.git cdmath_src`


Set environment for the compilation of CDMATH
---------------------------------------------
Dependencies. The following packages list is sufficient on Ubuntu 14.04, Ubuntu 14.10 and Debian Jessie:
 - `cmake`
 - `g++` or another C++ compiler
 - `libhdf5-dev`
 - `libopenmpi-dev`, in particular if you need to use the compilation option `-DMEDFILE_USE_MPI=ON`.
 - `petsc-dev`, if you want to compile a CDMATH-based linear solver. PETSc should mandatorily be at a minor subversion of version 3.4. * , that is to say *not* more recent. We have chosen the version 3.4 of PETSc (released on 13 May 2013), as it is easily installable from the main distributions repositories. Use the compilation option `-DCDMATH_WITH_PETSC=ON`.
 - `python-dev`, `python-numpy` and `swig`, if you want to generate Python executables and libraries of CDMATH. Use the compilation option `-DCDMATH_WITH_PYTHON=ON`.
 - `libcppunit-dev`, if you want to generate unit tests. Use the compilation option `-DCDMATH_WITH_TESTS=ON`.
 - `doxygen`, `graphviz` and `mscgen`, if you want to generate a nice documentation in `~/workspace/cdmath/cdmath_install/doc/`. Use the compilation option `-DCDMATH_WITH_DOCUMENTATION=ON`.
 - `rpm`, if you want to generate RPM installation packages. Use the compilation option `-DCDMATH_WITH_PACKAGE=ON`.

Directories. Create the suggested build and installation folders:
* `cd ~/workspace/cdmath`
* `mkdir cdmath_build`
* `mkdir cdmath_install`
* `cd cdmath_build`


Compile and install CDMATH
--------------------------
Generate makefiles for a minimum version:
* `cmake ../cdmath_src/ -DCMAKE_INSTALL_PREFIX=../cdmath_install -DCMAKE_BUILD_TYPE=Release`

Or generate makefiles for an all-options version:
* `cmake ../cdmath_src/ -DCMAKE_INSTALL_PREFIX=../cdmath_install -DCMAKE_BUILD_TYPE=Release -G"Eclipse CDT4 - Unix Makefiles" -D_ECLIPSE_VERSION=4.3 -DMEDFILE_USE_MPI=ON -DCDMATH_WITH_PETSC=ON -DCDMATH_WITH_PYTHON=ON -DCDMATH_WITH_TESTS=ON -DCDMATH_WITH_DOCUMENTATION=ON -DCDMATH_WITH_PACKAGE=ON`

Compile and install:
* `make`
* `make install`

Notes for compilation options:
* Eclipse: The Cmake options `-G"Eclipse CDT4 - Unix Makefiles" -D_ECLIPSE_VERSION=4.3` create project files if you want to develop CDMATH with Eclipse Kepler or higher.
* HDF5: On some systems (not Ubuntu 14.04), you may have to use the compilation option `-DHDF5_ROOT_DIR=/path/to/hdf5/library` too.
* MPI: On some systems (not Ubuntu 14.04), you may have to use the compilation option `-DMPI_ROOT_DIR=/path/to/mpi/library` too. You may also have to set the environment variable `export MPI_ROOT_DIR=/path/to/mpi/library`. Moreover, on some systems (not Ubuntu 14.04), the compilation option `-DMEDFILE_USE_MPI=ON` may be mandatory and be set to `ON`.
* PETSc: On some systems (not Ubuntu 14.04), you may have to use the compilation options `-DPETSC_DIR=/path/to/petsc/installation/petsc-3.4.5/ -DPETSC_ARCH=arch-linux2-c-opt`. Adapt according to your system, as it can also be `-DPETSC_DIR=/path/to/petsc/installation/petsc-3.4.5_install/` for instance. You may also have to use `export PETSC_DIR=/path/to/compiled/source/petsc-3.4.5/bin/` if you compiled PETSc from the sources as explained on [the official documentation](http://www.mcs.anl.gov/petsc/documentation/installation.html).


Use CDMATH
----------
To use CDMATH with your C++ code `main.cxx`:
 * C++ libraries: `export LD_LIBRARY_PATH=~/workspace/cdmath/cdmath_install/lib`
 * To know how to include the right libraries for compilation, see the makefiles of the examples. They include the list `-linterpkernel -lmedC -lmedloader -lmedcoupling -lbase -lmesh -llinearsolver`.

To use CDMATH with your Python code `main.py`:
 * C++ libraries: `export LD_LIBRARY_PATH=~/workspace/cdmath/cdmath_install/lib`
 * Python libraries: `export PYTHONPATH=~/workspace/cdmath/cdmath_install/lib/cdmath:~/workspace/cdmath/cdmath_install/bin/cdmath`


Create Linux installation packages for CDMATH
---------------------------------------------
After popular request, here is how you can create packages for Ubuntu 14.04 and Red Hat-based Linux distributions:

1. Download CDMATH as explained hereabove.
2. Set the environment as explained hereabove (in particular, make sure you have `rpm` installed).
3. Generate a makefile with `cmake -DCMAKE_INSTALL_PREFIX=../cdmath_install -DCMAKE_BUILD_TYPE=Release -DCDMATH_WITH_PACKAGE=ON ../cdmath_src/` and eventually other options (documentation, tests, swig, etc).
4. Compile with `make package`.

You will then find a Debian package in the build directory; you may install it on Ubuntu 14.04. You will also find an RPM package, which you may install on Red Hat-based distributions. This way, the packages you generate may include all the compilation options you want.

Unfortunately, the Debian package may be said to be of “bad quality” for Debian standards as far as ownership is concerned. This is true and due to limitations in CMake/CPack. The package should still install nonetheless.
