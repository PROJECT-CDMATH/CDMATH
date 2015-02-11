cmake_minimum_required (VERSION 2.8)

# Project name
project (CDMATH)
set (CDMATH_VERSION_MAJOR 0)
set (CDMATH_VERSION_MINOR 2)

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

find_package (HDF5 REQUIRED)
find_package (PETSc 3.4 EXACT REQUIRED)

# Swig interface
if (CMAKE_CDMATH_SWIG)
   find_package (PYTHON REQUIRED)
   find_package (SWIG REQUIRED)
   
   add_subdirectory (${CDMATH_SWIG_DIR})
endif (CMAKE_CDMATH_SWIG)

add_subdirectory (${PRE_REQUIS_DIR})
add_subdirectory (${BASE_DIR})
add_subdirectory (${MESH_DIR})

add_subdirectory (${LINEARSOLVER_DIR})

# Documentation
if (CMAKE_CDMATH_DOCUMENTATION)
  find_package (Doxygen)
  if (NOT DOXYGEN_FOUND)
    message (FATAL_ERROR "Doxygen is needed to build the documentation. Please install it correctly.")
  endif (NOT DOXYGEN_FOUND)

  configure_file (Doxyfile.in ${PROJECT_BINARY_DIR}/Doxyfile  @ONLY IMMEDIATE)

  add_custom_target (doc ALL COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
  							  SOURCES ${PROJECT_BINARY_DIR}/Doxyfile)

  install (DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/html DESTINATION share/doc/cdmath)
endif (CMAKE_CDMATH_DOCUMENTATION)
 
# Tests
if (CMAKE_CDMATH_TESTS)
  set (CMAKE_EXE_LINKER_FLAGS "-pg -fprofile-arcs -ftest-coverage -lgcov")
  enable_testing ()
  find_package (CPPUNIT REQUIRED)
  add_subdirectory (${TESTS_DIR})
endif (CMAKE_CDMATH_TESTS)

# Examples
set (examples_dir ${CDMATH_SOURCE_DIR}/examples)
add_subdirectory (${examples_dir})

# Packaging
if (CMAKE_CDMATH_PACKAGE)
  include (CPackLists.txt)
endif (CMAKE_CDMATH_PACKAGE)
