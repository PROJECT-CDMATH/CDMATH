cmake_minimum_required (VERSION 2.8)

# Project name
project (CDMATH)
set (CDMATH_VERSION_MAJOR 1)
set (CDMATH_VERSION_MINOR 0)

# Project options
option (CDMATH_WITH_PETSC "Compile CDMATH with PETSc linking." OFF)
option (CDMATH_WITH_DOCUMENTATION "Generate documentation for CDMATH use." OFF)
option (CDMATH_WITH_PYTHON "Compile Python interface for CDMATH." OFF)
option (CDMATH_WITH_PACKAGE "Generate RPM, Debian and tarball packages." OFF)
option (CDMATH_WITH_TESTS "Compile unit testing." OFF)

# Base directories
set (BASE_DIR ${CDMATH_SOURCE_DIR}/base)
set (MESH_DIR ${CDMATH_SOURCE_DIR}/mesh)
set (LINEARSOLVER_DIR ${CDMATH_SOURCE_DIR}/linearsolver)
set (CDMATH_SWIG_DIR ${CDMATH_SOURCE_DIR}/swig)
set (PRE_REQUIS_DIR ${CDMATH_SOURCE_DIR}/pre_requis)
set (INTERP_KERNEL_DIR ${PRE_REQUIS_DIR}/INTERP_KERNEL)
set (MED_COUPLING_DIR ${PRE_REQUIS_DIR}/MEDCoupling)
set (MED_COUPLING_SWIG_DIR ${PRE_REQUIS_DIR}/MEDCoupling_Swig)
set (MED_LOADER_DIR ${PRE_REQUIS_DIR}/MEDLoader)
set (MED3_DIR ${PRE_REQUIS_DIR}/MEDFile)

set (TESTS_DIR ${CDMATH_SOURCE_DIR}/tests)
list (APPEND CMAKE_MODULE_PATH "${CDMATH_SOURCE_DIR}/cmake_files")

set (MACHINE PCLINUX)
set (MED3_DEFINITIONS "-D${MACHINE}")
set (MED_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/pre_requis/MEDFile)
set (MED3_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/pre_requis/MEDFile/include ${CMAKE_CURRENT_BINARY_DIR}/pre_requis/MEDFile/include)
set (MED3_LIB ${CMAKE_CURRENT_BINARY_DIR}/pre_requis/MEDFile/src/libmedC.so)
set (MED3_LIBS ${MED3_LIB})

# HDF5
find_package (HDF5 REQUIRED)
set (CPACK_DEBIAN_PACKAGE_DEPENDS "libhdf5-dev")
set (CPACK_RPM_PACKAGE_REQUIRES "hdf5-devel")

if (CDMATH_WITH_PETSC)
  find_package (PETSc 3.4 EXACT REQUIRED)
  set (CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_DEPENDS}, petsc-dev (>= 3.4), petsc-dev (<< 3.5)")
  set (CPACK_RPM_PACKAGE_REQUIRES "${CPACK_RPM_PACKAGE_REQUIRES}, petsc >= 3.4, petsc < 3.5") # This is not fully working yet.
endif ()

# Swig interface
if (CDMATH_WITH_PYTHON)
   find_package (PYTHON REQUIRED)
   find_package (SWIG REQUIRED)
   set (CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_DEPENDS}, python-dev, python-numpy, swig")
   set (CPACK_RPM_PACKAGE_REQUIRES "${CPACK_RPM_PACKAGE_REQUIRES}, python-devel, numpy, swig")
   add_subdirectory (${CDMATH_SWIG_DIR})
endif ()

add_subdirectory (${PRE_REQUIS_DIR})
add_subdirectory (${BASE_DIR})
add_subdirectory (${MESH_DIR})

if (CDMATH_WITH_PETSC)
    add_subdirectory (${LINEARSOLVER_DIR})
endif ()

# Documentation
if (CDMATH_WITH_DOCUMENTATION)
  find_package (Doxygen)
  set (CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_DEPENDS}, doxygen, graphviz, mscgen")
  set (CPACK_RPM_PACKAGE_REQUIRES "${CPACK_RPM_PACKAGE_REQUIRES}, doxygen, graphviz, mscgen")
  if (NOT DOXYGEN_FOUND)
    message (FATAL_ERROR "Doxygen is needed to build the documentation. Please install it correctly.")
  endif (NOT DOXYGEN_FOUND)
  configure_file (Doxyfile.in ${PROJECT_BINARY_DIR}/Doxyfile  @ONLY IMMEDIATE)
  add_custom_target (doc ALL COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
  							  SOURCES ${PROJECT_BINARY_DIR}/Doxyfile)
  install (DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/html DESTINATION share/doc/cdmath)
endif ()
 
# Tests
if (CDMATH_WITH_TESTS)
  set (CMAKE_EXE_LINKER_FLAGS "-pg -fprofile-arcs -ftest-coverage -lgcov")
  enable_testing ()
  find_package (CPPUNIT REQUIRED)
  add_subdirectory (${TESTS_DIR})
endif ()

# Examples
set (examples_dir ${CDMATH_SOURCE_DIR}/examples)
add_subdirectory (${examples_dir})

# Packaging
if (CDMATH_WITH_PACKAGE)
  include (CPackLists.txt)
endif ()
