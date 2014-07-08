# Copyright (C) 2013  CEA/DEN, EDF R&D, OPEN CASCADE
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#
# Author: Adrien Bruneton
#

# HDF5 detection for med-file
#
# --- HDF5 specificities ----
#  MPI root directory used for HDF5 compilation is exposed into MPI_ROOT_DIR_EXP
#

# 1. Load environment or any previously detected HDF5
IF(DEFINED ENV{HDF5_ROOT_DIR})
  FILE(TO_CMAKE_PATH "$ENV{HDF5_ROOT_DIR}" _HDF5_ROOT_DIR_ENV)
  SET(_dflt_value "${_HDF5_ROOT_DIR_ENV}")
ELSE()
  # will be blank if no HDF5 was previously loaded
  SET(_dflt_value "${HDF5_ROOT_DIR_EXP}")
ENDIF()

#   Make cache entry 
SET(HDF5_ROOT_DIR "${_dflt_value}" CACHE PATH "Path to HDF5 directory")

# 2. Find package - config mode first (i.e. looking for XYZ-config.cmake)
IF(EXISTS "${HDF5_ROOT_DIR}")
  # Hope to find direclty a CMake config file there  
  IF(WIN32)
    SET(_CONF_DIR "${HDF5_ROOT_DIR}/cmake/hdf5") 
  ELSE()
    SET(_CONF_DIR "${HDF5_ROOT_DIR}/share/cmake/hdf5") 
  ENDIF()
  # Try find_package in config mode with a hard-coded guess. This
  # has the priority.
  FIND_PACKAGE(HDF5 CONFIG COMPONENTS C QUIET PATHS "${_CONF_DIR}"
      NO_CMAKE_BUILDS_PATH NO_CMAKE_PACKAGE_REGISTRY NO_CMAKE_SYSTEM_PACKAGE_REGISTRY)
    
  IF (NOT (HDF5_FOUND OR hdf5_FOUND))  
    SET(CMAKE_PREFIX_PATH "${HDF5_ROOT_DIR}")
  ELSE()
    MESSAGE(STATUS "Found HDF5 in CONFIG mode!")
    # Official CMake FindXXX module says HDF5_INCLUDE_DIR is deprecated
    # but as of HDF5-1.8.10 this is still the only thing found in the config file ... 
    SET(HDF5_INCLUDE_DIRS "${HDF5_INCLUDE_DIR}")
  ENDIF()
ENDIF()


# Otherwise try the standard way (module mode, with the standard CMake Find*** macro):
INCLUDE(medMacros)
SALOME_FIND_PACKAGE(MedfileHDF5 HDF5 MODULE)
SET(MEDFILEHDF5_FOUND ${HDF5_FOUND})

IF (HDF5_FOUND)
  MESSAGE(STATUS "Found HDF5 version " ${HDF5_VERSION} " -- parallel flag is set to " ${HDF5_ENABLE_PARALLEL})
  # 3. Set the root dir which was finally retained 
  # For HDF5 this is the parent of the
  # include directory:
  LIST(LENGTH HDF5_INCLUDE_DIRS _tmp_len)
  IF(_tmp_len)
    LIST(GET HDF5_INCLUDE_DIRS 0 _tmp_ROOT_DIR)
  ELSE()
    SET(_tmp_ROOT_DIR "${HDF5_INCLUDE_DIRS}")
  ENDIF()
  
  # Go up one level:
  GET_FILENAME_COMPONENT(_tmp_ROOT_DIR "${_tmp_ROOT_DIR}" PATH)

  # 4. Warn if CMake found something not located under ENV(XYZ_ROOT_DIR)
  IF(DEFINED ENV{HDF5_ROOT_DIR})
    SALOME_CHECK_EQUAL_PATHS(_res "${_tmp_ROOT_DIR}" "${_HDF5_ROOT_DIR_ENV}")
    IF(NOT _res)
      MESSAGE(WARNING "HDF5 was found, but not at the path given by the "
"environment HDF5_ROOT_DIR! Is the variable correctly set?"
"The two paths are: ${_tmp_ROOT_DIR}   and   ${_HDF5_ROOT_DIR_ENV}")
    ELSE()
      MESSAGE(STATUS "HDF5 found directory matches what was specified in the HDF5_ROOT_DIR, all good!")    
    ENDIF()
  ENDIF()

  # 5. Conflict detection
  # 5.1  From another prerequisite using HDF5
  IF(HDF5_ROOT_DIR_EXP)
      SALOME_CHECK_EQUAL_PATHS(_res "${_tmp_ROOT_DIR}" "${HDF5_ROOT_DIR_EXP}") 
      IF(NOT _res)
         MESSAGE(WARNING "Warning: HDF5: found version conflicts with a previously found HDF5!"
                          "The two paths are " ${_tmp_ROOT_DIR} " vs " ${HDF5_ROOT_DIR_EXP})
      ELSE()
          MESSAGE(STATUS "HDF5 directory matches what was previously exposed by another prereq, all good!")
      ENDIF()        
  ENDIF()

  # 6. Expose MPI configuration to the rest of the world
  IF(HDF5_ENABLE_PARALLEL)
      # HDF5 was compiled with MPI support
      # Unfortunately HDF5 doesn't expose its MPI configuration easily ...
      # We sniff the properties of the HDF5 target which should also be there:
      GET_PROPERTY(_lib_lst TARGET hdf5 PROPERTY IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG)
      FOREACH(s ${_lib_lst})
        STRING(FIND "${s}" "mpi." _res)   # should cover WIN(?) and LINUX
        IF(_res GREATER -1)
          GET_FILENAME_COMPONENT(_tmp "${s}" PATH)     # go up to levels
          GET_FILENAME_COMPONENT(MPI_ROOT_DIR_EXP "${_tmp}" PATH)
          BREAK()
        ENDIF()
      ENDFOREACH()
  ENDIF(HDF5_ENABLE_PARALLEL)
    
  # Make some advanced stuff hidden by default:
  MARK_AS_ADVANCED(FORCE HDF5_INCLUDE_DIR HDF5_INCLUDE_DIRS HDF5_LIB HDF5_DIR)
  
  # As of HDF5-1.8.11 the following is still not set in the exported CONFIG
  # of the official hdf5-config.cmake. We add it ourselves:
  IF(NOT DEFINED HDF5_LIBRARIES)
    SET(HDF5_LIBRARIES "hdf5")
  ENDIF()
  
  SET(HDF5_ROOT_DIR ${_tmp_ROOT_DIR})
  
ELSE(HDF5_FOUND)
  MESSAGE(STATUS "HDF5 was not found.")  
ENDIF(HDF5_FOUND)
