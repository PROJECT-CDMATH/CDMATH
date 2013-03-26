
SET(TestMesh_SOURCES
  MeshTests.py
  )

SET(MAILLAGE_MED
  maillage.med
  )

file(COPY ${MAILLAGE_MED} DESTINATION ${CMAKE_BINARY_DIR}/tests) 


