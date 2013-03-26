INCLUDE(CheckIncludeFile)
IF(CMAKE_Fortran_COMPILER_WORKS)
  INCLUDE(FortranCInterface)
ENDIF(CMAKE_Fortran_COMPILER_WORKS)
INCLUDE(CheckFunctionExists)
INCLUDE(CheckTypeSize)

###############################################################################
##Define contants
###############################################################################

SET(PACKAGE_NAME "\"MED Fichier\"")
SET(MED_CHECK_23FORMAT 1)
SET(MED_HAVE_FORTRAN 1)
SET(MESGERR 1)
SET(PACKAGE \"${PROJECT_NAME}\")
SET(PACKAGE_BUGREPORT \"eric.fayolle@edf.fr\")
SET(PACKAGE_STRING "\"MED Fichier ${MED_STR_VERSION}\"")
SET(PACKAGE_TARNAME "${PROJECT_NAME}")
SET(PACKAGE_URL \"\")
SET(PACKAGE_VERSION \"${MED_STR_VERSION}\")
SET(VERSION \"${MED_STR_VERSION}\")

###############################################################################
## TEMPORARY HARDCODED FLAGS, BECAUSE CMAKE HAS NO TOOLS TO 
## THEIR DETERMINATION.
##  !!! SHOULD BE REMOVED AFTER FUTURE IMPROVEMENTS !!!
###############################################################################
IF(NOT WINDOWS)
   SET(HAVE_CC_C99 1)
   SET(STDC_HEADERS 1)
ENDIF(NOT WINDOWS)
SET(HAVE__BOOL 1)

###############################################################################
## Macro to set compilation flag for target
###############################################################################
MACRO(MED_SET_COMPILE_FLAGS lib)
   STRING(LENGTH "${ARGN}" len)
   IF(${len})
     SET(libflags "${ARGN}")
     FOREACH(f ${libflags})
     SET(flags "${flags} ${f}")
     ENDFOREACH(f ${libflags})
     SET_TARGET_PROPERTIES(${lib} PROPERTIES COMPILE_FLAGS ${flags})
   ENDIF(${len})
   SET(flags "")
ENDMACRO(MED_SET_COMPILE_FLAGS)

###############################################################################
## Macro to check header files
###############################################################################
MACRO(MED_CHECK_HEADERS)

#Define list of headers
SET(H_LIST "H5public;dlfcn;inttypes;malloc;memory;pwd;stdbool;stdint;stdlib")
SET(H_LIST "${H_LIST};stdbool;stdint;stdlib;strings;string;sys/stat")
SET(H_LIST "${H_LIST};sys/timeb;sys/time;sys/types;unistd;stdio")

#Check each header
FOREACH(H_FILE ${H_LIST})
   SET(STR_FILE "${H_FILE}")  
   STRING(REPLACE "/" "_"  STR_FILE ${STR_FILE})
   STRING(TOUPPER  "${STR_FILE}" STR_FILE)
   SET(H_FILE_FLAG "HAVE_${STR_FILE}_H")
   CHECK_INCLUDE_FILE("${H_FILE}.h" ${H_FILE_FLAG})
ENDFOREACH(H_FILE ${H_LIST})

ENDMACRO(MED_CHECK_HEADERS)

###############################################################################
## Macro to check methods files
###############################################################################
MACRO(MED_CHECK_FUNCTIONS)

#Define list of functions
SET(F_LIST "cuserid;ftime;geteuid;getpwuid;gettimeofday")

#Check each header
FOREACH(F_FUNC ${F_LIST})
   STRING(TOUPPER   "${F_FUNC}" STR_F_FUNC)
   SET(F_FUNC_FLAG "HAVE_${STR_F_FUNC}")
   CHECK_FUNCTION_EXISTS("${F_FUNC}" ${F_FUNC_FLAG})
ENDFOREACH(F_FUNC ${F_LIST})

ENDMACRO(MED_CHECK_FUNCTIONS)

###############################################################################
## Macro to get sizeof Fortran INTEGER
###############################################################################
MACRO(CHECK_FORTRAN_INTEGER_SIZE)

SET(SIZEOF_FORTRAN_INTEGER 4 CACHE INTERNAL "Size of the default INTEGER type" FORCE)

IF(CMAKE_Fortran_COMPILER_SUPPORTS_F90)
    FOREACH(_TEST_SIZE 1 2 4 8 16 32)
       SET(_TEST_FILE ${CMAKE_CURRENT_BINARY_DIR}/testFortranIntegerSize${_TEST_SIZE}.f90)
       FILE( WRITE ${_TEST_FILE}
       "
       PROGRAM check_size
       INTEGER*${_TEST_SIZE}, TARGET :: a
       INTEGER, POINTER :: pa
       pa => a
       END PROGRAM
       ")
       TRY_COMPILE( SIZEOF_INTEGER ${CMAKE_CURRENT_BINARY_DIR} ${_TEST_FILE} )
       IF(SIZEOF_INTEGER)
          MESSAGE(STATUS "Testing default INTEGER*${_TEST_SIZE} - found")
          SET(SIZEOF_FORTRAN_INTEGER ${_TEST_SIZE})
          BREAK()
       ENDIF(SIZEOF_INTEGER)
    ENDFOREACH(_TEST_SIZE 1 2 4 8 16 32)
ENDIF(CMAKE_Fortran_COMPILER_SUPPORTS_F90)
ENDMACRO(CHECK_FORTRAN_INTEGER_SIZE)

###############################################################################
## Macro to check sizeof
###############################################################################
MACRO(MED_CHECK_SIZE)
#int
CHECK_TYPE_SIZE(int SIZEOF_INT)
#long
CHECK_TYPE_SIZE(long SIZEOF_LONG)
#Fortran INTEGER 
CHECK_FORTRAN_INTEGER_SIZE()

IF(${SIZEOF_FORTRAN_INTEGER} EQUAL 8)
    SET(HAVE_F77INT64 1)
    IF(${SIZEOF_LONG} EQUAL 8)
        SET(LONG_OR_INT long)
    ELSE(${SIZEOF_LONG} EQUAL 8)
        MESSAGE(FATAL_ERROR "Size of C type long expected to be eight bytes")
    ENDIF(${SIZEOF_LONG} EQUAL 8)
ELSE(${SIZEOF_FORTRAN_INTEGER} EQUAL 8)
    IF(${SIZEOF_FORTRAN_INTEGER} EQUAL 4)
        IF(${SIZEOF_INT} EQUAL 4)
           SET(LONG_OR_INT int)
        ELSE(${SIZEOF_INT} EQUAL 4)
           MESSAGE(FATAL_ERROR "Size of C type int expected to be four bytes")
        ENDIF(${SIZEOF_INT} EQUAL 4)
    ENDIF(${SIZEOF_FORTRAN_INTEGER} EQUAL 4)
ENDIF(${SIZEOF_FORTRAN_INTEGER} EQUAL 8)

ENDMACRO(MED_CHECK_SIZE)

###############################################################################
## Macro to get C <-> Fortran mangling
###############################################################################
MACRO(MED_C_FORTRAN_INTERFACE)
IF(CMAKE_Fortran_COMPILER_WORKS)

  FortranCInterface_HEADER(${CMAKE_CURRENT_BINARY_DIR}/include/FC.h)
  FILE(READ ${CMAKE_CURRENT_BINARY_DIR}/include/FC.h f_content)
  STRING(REPLACE "\n" ";" list_f_content ${f_content})
  
  FOREACH(ln ${list_f_content})
    IF(${ln} MATCHES "#define FortranCInterface_GLOBAL[^_].*")
       STRING(REPLACE "#define FortranCInterface_GLOBAL" ""  F77_FUNC "${ln}")
    ENDIF()

    IF("${ln}" MATCHES "#define FortranCInterface_GLOBAL_.*")
       STRING(REPLACE "#define FortranCInterface_GLOBAL_" "" F77_FUNC_ "${ln}")
    ENDIF()
  ENDFOREACH(ln ${list_f_content})

ELSE(CMAKE_Fortran_COMPILER_WORKS)

  IF(WIN32)
    SET(F77_FUNC "(name,NAME) NAME")
  ENDIF(WIN32)

ENDIF(CMAKE_Fortran_COMPILER_WORKS)

ENDMACRO(MED_C_FORTRAN_INTERFACE)

###############################################################################
## Macro to check possibility include time.h and sys/time.h headers together
###############################################################################
MACRO(MED_TIME_SYS_TIME)

SET(_TEST_FILE ${CMAKE_CURRENT_BINARY_DIR}/testTimeSysTime.c)
FILE( WRITE ${_TEST_FILE}
"
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>

#  ifdef __cplusplus
     extern "C"
#  endif

int
main ()
{
if ((struct tm *) 0)
return 0;
  ;
  return 0;
}
"
)

TRY_COMPILE( TIME_WITH_SYS_TIME ${CMAKE_CURRENT_BINARY_DIR} ${_TEST_FILE} )
IF(${TIME_WITH_SYS_TIME})
  SET(TIME_WITH_SYS_TIME 1)
ELSE(${TIME_WITH_SYS_TIME})
  SET(TIME_WITH_SYS_TIME 0)
ENDIF(${TIME_WITH_SYS_TIME})

ENDMACRO(MED_TIME_SYS_TIME)
