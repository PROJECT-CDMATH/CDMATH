# Copyright (C) 2012-2014  CEA/DEN, EDF R&D
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
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

INCLUDE(${SWIG_USE_FILE})

ADD_DEFINITIONS(${PYTHON_DEFINITIONS})

SET_SOURCE_FILES_PROPERTIES(MEDCoupling.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(MEDCoupling.i PROPERTIES SWIG_DEFINITIONS "-shadow")
SET(SWIG_MODULE_MEDCoupling_EXTRA_FLAGS ${SWIG_EXTRA_FLAGS_FOR_NUMPYANDSCIPY})

SET (MEDCoupling_SWIG_DPYS_FILES
    MEDCouplingCommon.i
    MEDCouplingMemArray.i
    MEDCouplingFieldDiscretization.i
    MEDCouplingFinalize.i
    MEDCouplingTypemaps.i)   

INCLUDE_DIRECTORIES(
  ${PYTHON_INCLUDE_DIRS}
  ${PTHREAD_INCLUDE_DIR} # pthread dependancy due to python2.7 library
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../MEDCoupling
  ${CMAKE_CURRENT_SOURCE_DIR}/../INTERP_KERNEL
  ${CMAKE_CURRENT_SOURCE_DIR}/../INTERP_KERNEL/Bases
  ${CMAKE_CURRENT_SOURCE_DIR}/../INTERP_KERNEL/Geometric2D
  ${CMAKE_CURRENT_SOURCE_DIR}/../INTERP_KERNEL/ExprEval
  ${CMAKE_CURRENT_SOURCE_DIR}/../INTERP_KERNEL/GaussPoints
  )

# _ABR_ Ensure dependency mechanism on all SWIG files and headers
SET (SWIG_MODULE_MEDCoupling_EXTRA_DEPS ${MEDCoupling_SWIG_DPYS_FILES} 
    ${medcoupling_HEADERS_HXX} ${medcoupling_HEADERS_TXX}
    ${interpkernel_HEADERS_HXX} ${interpkernel_HEADERS_TXX})

SWIG_ADD_MODULE(MEDCoupling python MEDCoupling.i)
SWIG_LINK_LIBRARIES(MEDCoupling ${PYTHON_LIBRARIES} ${PLATFORM_LIBS} medcoupling)

SET_SOURCE_FILES_PROPERTIES(MEDCouplingRemapper.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(MEDCouplingRemapper.i PROPERTIES SWIG_DEFINITIONS "-shadow")
SET(SWIG_MODULE_MEDCouplingRemapper_EXTRA_FLAGS ${SWIG_EXTRA_FLAGS_FOR_NUMPYANDSCIPY})

# _ABR_ Ensure dependency mechanism on all SWIG files and headers
SET (SWIG_MODULE_MEDCouplingRemapper_EXTRA_DEPS ${MEDCoupling_SWIG_DPYS_FILES}
    ${medcoupling_HEADERS_HXX} ${medcoupling_HEADERS_TXX}
    ${interpkernel_HEADERS_HXX} ${interpkernel_HEADERS_TXX})

SWIG_ADD_MODULE(MEDCouplingRemapper python MEDCouplingRemapper.i)
SWIG_LINK_LIBRARIES(MEDCouplingRemapper ${PYTHON_LIBRARIES} medcouplingremapper)

IF(WIN32)
  SET_TARGET_PROPERTIES(_MEDCouplingRemapper PROPERTIES DEBUG_OUTPUT_NAME _MEDCouplingRemapper_d)
  SET_TARGET_PROPERTIES(_MEDCoupling PROPERTIES DEBUG_OUTPUT_NAME _MEDCoupling_d)
ENDIF(WIN32)
INSTALL(TARGETS ${SWIG_MODULE_MEDCoupling_REAL_NAME} ${SWIG_MODULE_MEDCouplingRemapper_REAL_NAME} DESTINATION ${SALOME_INSTALL_PYTHON})

SET(PYFILES_TO_INSTALL ${CMAKE_CURRENT_BINARY_DIR}/MEDCoupling.py ${CMAKE_CURRENT_BINARY_DIR}/MEDCouplingRemapper.py)
INSTALL_AND_COMPILE_PYTHON_FILE("${PYFILES_TO_INSTALL}" ${SALOME_INSTALL_SCRIPT_PYTHON})

INSTALL(FILES MEDCoupling.i MEDCouplingCommon.i MEDCouplingRefCountObject.i MEDCouplingMemArray.i MEDCouplingFieldDiscretization.i MEDCouplingTimeDiscretization.i MEDCouplingFinalize.i MEDCouplingRemapper.i MEDCouplingTypemaps.i MEDCouplingDataArrayTypemaps.i DESTINATION ${SALOME_INSTALL_HEADERS})
INSTALL(FILES MEDCouplingBasicsTest.py MEDCouplingRemapperTest.py MEDCouplingDataForTest.py MEDCouplingNumPyTest.py DESTINATION ${SALOME_INSTALL_SCRIPT_PYTHON})
INSTALL(FILES MEDCouplingExamplesTest.py DESTINATION ${SALOME_INSTALL_SCRIPT_PYTHON})

#SALOME_GENERATE_TESTS_ENVIRONMENT(tests_env)

#ADD_TEST(MEDCouplingBasicsTest ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/MEDCouplingBasicsTest.py)
#SET_TESTS_PROPERTIES(MEDCouplingBasicsTest PROPERTIES ENVIRONMENT "${tests_env}")
#ADD_TEST(MEDCouplingExamplesTest ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/MEDCouplingExamplesTest.py)
#SET_TESTS_PROPERTIES(MEDCouplingExamplesTest PROPERTIES ENVIRONMENT "${tests_env}")
#ADD_TEST(MEDCouplingRemapperTest ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/MEDCouplingRemapperTest.py)
#SET_TESTS_PROPERTIES(MEDCouplingRemapperTest PROPERTIES ENVIRONMENT "${tests_env}")

#IF(NUMPY_FOUND)
#  ADD_TEST(MEDCouplingNumPyTest ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/MEDCouplingNumPyTest.py)
#  SET_TESTS_PROPERTIES(MEDCouplingNumPyTest PROPERTIES ENVIRONMENT "${tests_env}")
#ENDIF(NUMPY_FOUND)
