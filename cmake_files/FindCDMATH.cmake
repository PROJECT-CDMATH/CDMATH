find_path (CDMATH_DIR include/CdmathException.hxx
  HINTS ENV CDMATH_DIR
  PATHS
  /usr
  $ENV{HOME}/cdmath
  $ENV{HOME}/workspace/cdmath_install
  DOC "CDMATH Directory")
message (STATUS "Found CDMATH: ${CDMATH_DIR}")

# Include directories
# This sets the variable ${CDMATH_INCLUDES}.
set(CDMATH_INCLUDES ${CDMATH_DIR}/include)

# CDMATH libraries against which to link
# This sets the variable ${CDMATH_LIBRARIES}.
find_library (INTERPKERNEL_LIB interpkernel PATHS ${CDMATH_DIR}/lib)
find_library (MEDC_LIB medC PATHS ${CDMATH_DIR}/lib)
find_library (MEDLOADER_LIB medloader PATHS ${CDMATH_DIR}/lib)
find_library (MEDCOUPLING_LIB medcoupling PATHS ${CDMATH_DIR}/lib)
find_library (BASE_LIB base PATHS ${CDMATH_DIR}/lib)
find_library (MESH_LIB mesh PATHS ${CDMATH_DIR}/lib)
find_library (LINEARSOLVER_LIB linearsolver PATHS ${CDMATH_DIR}/lib)
set (CDMATH_LIBRARIES
    ${INTERPKERNEL_LIB} ${MEDC_LIB} ${MEDLOADER_LIB} ${MEDCOUPLING_LIB} ${BASE_LIB} ${MESH_LIB} ${LINEARSOLVER_LIB})
