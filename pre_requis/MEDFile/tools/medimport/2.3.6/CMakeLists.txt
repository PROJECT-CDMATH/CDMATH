SET(medimport236_SOURCES
  MED30linkWrbuilt.cxx
  MAJ_236_300_chainebuilt.cxx
  _MEDconvertStringDatasets.c
  MAJ_236_300_string_datasets.c
  MAJ_236_300_champs.c
  MAJ_236_300_fieldOnEntity.c
  MAJ_236_300_equivalence.c
  #MAJ_236_300_entitesbuilt.cxx
  MAJ_236_300_mesh_datasets.c
  MAJ_236_300_maillages.c
  MAJ_236_300_joint.c
  MED30linkWr.c
  #MAJ_236_300_equivalencebuilt.cxx
  #MAJ_236_300_jointbuilt.cxx
  #MAJ_236_300_maillagesbuilt.cxx
  #MAJ_236_300_fieldOnEntitybuilt.cxx
  #MAJ_236_300_champsbuilt.cxx
  MAJ_236_300_entites.c
  #MAJ_236_300_string_datasetsbuilt.cxx
  #_MEDconvertStringDatasetsbuilt.cxx
  #MAJ_236_300_mesh_datasetsbuilt.cxx
  MAJ_236_300_chaine.c
)

# On Win platform, we need to make sure symbols are properly exported in
# the intermediary OBJECT targets
# The definition "-D<target>_EXPORTS" is normally added automatically by CMake
# when building Win DLL,
# but this doesn't work when using intermediary targets (ADD_LIBRARY(... OBJECT ...)
IF(WIN32)
  ADD_DEFINITIONS(-Dmedimport_EXPORTS)
ENDIF()

ADD_LIBRARY(_medimport236 OBJECT ${medimport236_SOURCES})

ADD_LIBRARY(_medimport236_static OBJECT ${medimport236_SOURCES})
MED_SET_DEFINITIONS(_medimport236_static MED3_USESTATIC)
