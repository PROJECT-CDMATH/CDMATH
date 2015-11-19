find_path (CDMATH_DIR include/CdmathException.hxx
  HINTS ENV CDMATH_DIR
  PATHS
  /usr
  $ENV{HOME}/cdmath
  $ENV{HOME}/workspace/cdmath_install
  DOC "CDMATH Directory")

set(CDMATH_INCLUDES ${CDMATH_DIR}/include)
set(CDMATH_LIBRARIES ${CDMATH_DIR}/lib)
