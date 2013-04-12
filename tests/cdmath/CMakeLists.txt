INCLUDE_DIRECTORIES(
  ${CPPUNIT_INCLUDE_DIRS}
  ${HDF5_INCLUDE_DIRS}
  ${MED3_INCLUDE_DIRS}
  ${PRE_REQUIS_DIR}/MEDLoader
  ${PRE_REQUIS_DIR}/MEDCoupling
  ${PRE_REQUIS_DIR}/INTERP_KERNEL
  ${PRE_REQUIS_DIR}/INTERP_KERNEL/Bases
  ${MESH_DIR}/inc 
  ${MED_COUPLING_DIR}
  )

SET(TestMesh_SOURCES
  NodeTests.cxx
  PointTests.cxx
  CellTests.cxx
  FaceTests.cxx
  FieldTests.cxx
  MeshTests.cxx
  DoubleTabTests.cxx
  IntTabTests.cxx
  )

SET(MAILLAGE_MED
  maillage.med
  )

file(COPY ${MAILLAGE_MED} DESTINATION ${CMAKE_BINARY_DIR}/tests) 

FILE(GLOB TestMesh_SOURCES . *.cxx)
ADD_LIBRARY(TestMesh SHARED ${TestMesh_SOURCES})
TARGET_LINK_LIBRARIES(TestMesh mesh medcoupling ${HDF5_LIBS} ${MED3_LIBS} medloader)

INSTALL(TARGETS TestMesh DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

