INCLUDE_DIRECTORIES(
  ${HDF5_INCLUDE_DIRS}
)

SET(medimport21x_SOURCES
  2.1.x/MAJ_21_22_chaine.c
  #2.1.x/MAJ_21_22_noeuds_maillagebuilt.cxx
  2.1.x/MAJ_21_22_maillages.c
  #2.1.x/MAJ_21_22_elements_maillagebuilt.cxx
  2.1.x/MAJ_21_22_profilsbuilt.cxx
  #2.1.x/MAJ_21_22_familles_maillagebuilt.cxx
  2.1.x/MAJ_21_22_chainebuilt.cxx
  2.1.x/MAJ_21_22_localisation_Gaussbuilt.cxx
  2.1.x/MAJ_21_22_elements_maillage.c
  2.1.x/MAJ_21_22_noeuds_maillage.c
  2.1.x/MED21datasetNumLirebuilt.cxx
  2.1.x/MED21attrNumLire.c
  2.1.x/MAJ_21_22_champs.c
  #2.1.x/MAJ_21_22_champsbuilt.cxx
  2.1.x/MAJ_21_22_profils.c
  2.1.x/MED21datasetNumLire.c
  2.1.x/MAJ_21_22_familles_maillage.c
  2.1.x/MED21attrNumLirebuilt.cxx
  2.1.x/MAJ_21_22_localisation_Gauss.c
  #2.1.x/MAJ_21_22_maillagesbuilt.cxx
)

SET(medimport231_SOURCES
  2.3.1/MED231datasetNumEcrire.c
  2.3.1/MED231champLireEtUnlink.c
  2.3.1/MAJ_231_232_maillagesbuilt.cxx
  2.3.1/MAJ_231_232_chaine.c
  2.3.1/MED231datasetNumEcrirebuilt.cxx
  2.3.1/MED231champNormaliserbuilt.cxx
  2.3.1/MAJ_231_232_chainebuilt.cxx
  2.3.1/MED231champLireEtUnlinkbuilt.cxx
  2.3.1/MAJ_231_232_champs.c
  2.3.1/MAJ_231_232_champsbuilt.cxx
  2.3.1/MED231datasetNumLire.c
  2.3.1/MAJ_231_232_maillages.c
  2.3.1/MED231champRefInfoEtRenMaa.c
  2.3.1/MED231champInfoEtRen.c
  2.3.1/MED231champInfoEtRenbuilt.cxx
  2.3.1/MED231datasetNumLirebuilt.cxx
  2.3.1/MED231champNormaliser.c
  2.3.1/MED231champRefInfoEtRenMaabuilt.cxx
)

SET(medimport236_SOURCES
  2.3.6/MED30linkWrbuilt.cxx
  2.3.6/MAJ_236_300_chainebuilt.cxx
  2.3.6/_MEDconvertStringDatasets.c
  2.3.6/MAJ_236_300_string_datasets.c
  2.3.6/MAJ_236_300_champs.c
  2.3.6/MAJ_236_300_fieldOnEntity.c
  2.3.6/MAJ_236_300_equivalence.c
  #2.3.6/MAJ_236_300_entitesbuilt.cxx
  2.3.6/MAJ_236_300_mesh_datasets.c
  2.3.6/MAJ_236_300_maillages.c
  2.3.6/MAJ_236_300_joint.c
  2.3.6/MED30linkWr.c
  #2.3.6/MAJ_236_300_equivalencebuilt.cxx
  #2.3.6/MAJ_236_300_jointbuilt.cxx
  #2.3.6/MAJ_236_300_maillagesbuilt.cxx
  #2.3.6/MAJ_236_300_fieldOnEntitybuilt.cxx
  #2.3.6/MAJ_236_300_champsbuilt.cxx
  2.3.6/MAJ_236_300_entites.c
  #2.3.6/MAJ_236_300_string_datasetsbuilt.cxx
  #2.3.6/_MEDconvertStringDatasetsbuilt.cxx
  #2.3.6/MAJ_236_300_mesh_datasetsbuilt.cxx
  2.3.6/MAJ_236_300_chaine.c
)

SET(medimporttop_SOURCES
  libmedimport.c
  MAJ_version.c
)
SET_SOURCE_FILES_PROPERTIES(${medimport21x_SOURCES} PROPERTIES COMPILE_FLAGS "-I${CMAKE_CURRENT_SOURCE_DIR}/include ${HDF5_DEFINITIONS} -I${CMAKE_CURRENT_BINARY_DIR}/../../include/2.3.6 -I${CMAKE_CURRENT_SOURCE_DIR}/../../include/2.3.6 -I${CMAKE_CURRENT_SOURCE_DIR}/include -DMED_API_23")

SET_SOURCE_FILES_PROPERTIES(${medimport231_SOURCES} PROPERTIES COMPILE_FLAGS "${HDF5_DEFINITIONS} -I${CMAKE_CURRENT_BINARY_DIR}/../../include/2.3.6 -I${CMAKE_CURRENT_SOURCE_DIR}/../../include/2.3.6 -I${CMAKE_CURRENT_SOURCE_DIR}/include -DMED_API_23")

SET_SOURCE_FILES_PROPERTIES(${medimport236_SOURCES} PROPERTIES COMPILE_FLAGS "${HDF5_DEFINITIONS} -I${CMAKE_CURRENT_BINARY_DIR}/../../include -I${CMAKE_CURRENT_SOURCE_DIR}/include -I${CMAKE_CURRENT_SOURCE_DIR}/../../include")

SET_SOURCE_FILES_PROPERTIES(${medimporttop_SOURCES} PROPERTIES COMPILE_FLAGS "${HDF5_DEFINITIONS} -I${CMAKE_CURRENT_BINARY_DIR}/../../include -I${CMAKE_CURRENT_SOURCE_DIR}/include -I${CMAKE_CURRENT_SOURCE_DIR}/../../include")
SET_SOURCE_FILES_PROPERTIES(medimportcxx.cxx PROPERTIES COMPILE_FLAGS "${HDF5_DEFINITIONS} -I${CMAKE_CURRENT_SOURCE_DIR}/include")

SET(medimport_SOURCES
  ${medimport21x_SOURCES}
  ${medimporttop_SOURCES}
  ${medimport231_SOURCES}
  ${medimport236_SOURCES}
)
ADD_LIBRARY(medimportengine SHARED ${medimport_SOURCES})
TARGET_LINK_LIBRARIES(medimportengine medC)
SET(medimportengine_COMPILE_CFLAGS ${PLATFORM_CFLAGS})
SET(medimportengine_COMPILE_CFLAGS ${medimportengine_COMPILE_CFLAGS} -DMED_MESGERR_FOR_TOOLS ${HDF5_DEFINITIONS})
MED_SET_COMPILE_FLAGS(medimportengine "${medimportengine_COMPILE_CFLAGS}")
SET_TARGET_PROPERTIES(medimportengine PROPERTIES OUTPUT_NAME medimport)


ADD_EXECUTABLE(medimport medimportcxx.cxx)
SET(medimport_COMPILE_CFLAGS ${PLATFORM_CFLAGS})
SET(medimport_COMPILE_CFLAGS ${medimport_COMPILE_CFLAGS} -DMED_MESGERR_FOR_TOOLS ${HDF5_DEFINITIONS})
MED_SET_COMPILE_FLAGS(medimport "${medimport_COMPILE_CFLAGS}")
TARGET_LINK_LIBRARIES(medimport medimportengine)

INSTALL(TARGETS medimport DESTINATION bin )
INSTALL(TARGETS medimportengine DESTINATION lib)
INSTALL(FILES include/medimport_win_dll.h include/MEDimport.h include/MEDimport.hxx include/medimportcxx_win_dll.h DESTINATION include)
