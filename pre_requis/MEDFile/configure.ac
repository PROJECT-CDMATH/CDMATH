# Process this file with autoconf to produce a configure script.

## Initialize Autoconf & defined package name...
##Lors de la sortie d'une nouvelle release mettre à jour les champs suivants
##Mettre à jour le numéro de package
AC_INIT([MED Fichier],3.2.0,[eric.fayolle@edf.fr],med)

##Lors de la sortie d'une nouvelle release mettre à jour les champs suivants
##Ne pas oubliez de modifier  -version-info x:y:z dans src/Makefile.am tools/medimport/Makefile.am
##Modifiez si necessaire les numéros de versions dans la documentation
##Ne pas oublier d'utiliser la dernière version des autotools
##Mettre à jour le ChangeLog et le README
##Mettre à jour la table de versionement des APIs internes (en C et en F)
##Mettre à jour les tests de compatibilité dans MEDfileCompatibility (chgt de majeur/mineur)
MED_NUM_MAJEUR=3
MED_NUM_MINEUR=2
MED_NUM_RELEASE=0
AC_SUBST(MED_NUM_MAJEUR)
AC_SUBST(MED_NUM_MINEUR)
AC_SUBST(MED_NUM_RELEASE)

HDF_VERSION_REF=5-1.8.10
AC_SUBST(HDF_VERSION_REF)

## Require Automake 1.7.9 or better.
## AM_INIT_AUTOMAKE(1.7.9) 

## Guess we have a correct src directory by verifying we get the following file
AC_CONFIG_SRCDIR(src/ci/MEDmeshCr.c)


## Use the config directory for libtool stuff ...
AC_CONFIG_AUX_DIR(config)
AC_SUBST(ac_aux_dir)
AC_CONFIG_MACRO_DIR([config])


## Initialize Automake
AM_INIT_AUTOMAKE([dist-bzip2 tar-ustar serial-tests])

##
AC_CONFIG_HEADER(include/med_config.h)

dnl Ne pas activer ici (cross compiling)
dnl AC_CHECK_SIZEOF_FORTRAN(integer)
dnl AC_CHECK_SIZEOF_FORTRAN(integer*4)
dnl AC_CHECK_SIZEOF_FORTRAN(integer*8)


## Check For HDF5
## -> MED_ENABLE_PMED
##    --> AX_PROG_CC_MPI
##    --> MED_CHECK_F90
##        -->AX_PROG_FC_MPI
MED_CHECK_HDF5


## Configure le choix du compilateur Fortran 90 et active les tests correspondants
AM_CONDITIONAL([WITHF90],[ test -n "$FC" ])
AC_SUBST(FC)
AC_ARG_VAR([FTLDFLAGS],[Fortran flags for linking fortran tests]) 
AC_ARG_VAR([FTLIBS],[Fortran flags for linking fortran tests]) 


## Check if user want to disable explicit error messages
MED_CHECK_MESGERR

## Check For MED2.x Objects Not Readable By New3.x API
MED_ENABLE_CHECK23

# Checks for programs.
test "x$ac_cv_prog_cc_c99" = "no" || AC_DEFINE(HAVE_CC_C99,[1],[The C99 capabilities of C compiler.])

MED_CHECK_F77
if test x"$enable_fortran" = xyes; then
 AC_F77_WRAPPERS
 AC_F77_LIBRARY_LDFLAGS
fi

## Choose med_int type between {int,long}
MED_CHECK_TYPEOF_MEDINT

AX_PROG_CXX_MPI(test "x$mpi_wanted_test" = "xyes")
CXX_DIRNAME=`AS_DIRNAME(["$CXX"])`
CXX_BASENAME=`AS_BASENAME(["$CXX"])`
AC_CHECK_PROG([CXXOK],[$CXX_BASENAME],[yes],[no],[$PATH$PATH_SEPARATOR$CXX_DIRNAME$PATH_SEPARATOR],[])
if test "X$CXXOK" = "Xno"; then
  AC_MSG_ERROR([Can't find a valid CXX compiler, please verify your env. var. PATH])
fi

## Provide MED2.3 API
MED_ENABLE_API23

# Libtool
AC_LIBTOOL_WIN32_DLL
LT_INIT


## LOOKING FOR diff program
AC_CHECK_PROG([DIFF],[diff],[yes],[no],[],[])

if test "X$DIFF" = "Xno"; then
  AC_MSG_ERROR([Can't find diff command, please verify your env. var. PATH])
fi

## LOOKING FOR sed program
AC_CHECK_PROG([SED],[sed],[yes],[no],[],[])

if test "X$SED" = "Xno"; then
  AC_MSG_ERROR([Can't find sed command, please verify your env. var. PATH])
fi

## LOOKING FOR awk program
AC_PROG_AWK
if test "X$AWK" = "X"; then
  AC_MSG_ERROR([Can't find awk command, please verify your env. var. PATH])
fi

## Check for ln -s
AC_PROG_LN_S

## Check for cp  
AC_PATH_PROG([CP],[cp],[],[$PATH])

if test "X$CP" = "X"; then
   AC_MSG_WARN([I don't find cp command in your PATH! - You coudn't use make check ! -])
fi
AC_SUBST(CP)

## Check for tk interpreter : we don't check version
## (for next realese :-)
AC_PATH_PROG([WISH],[wish],[],[$PATH])

if test "X$WISH" = "X"; then
   AC_MSG_WARN([I don't find TK interpreter in your PATH! - You coudn't use graphical dump ! -])
fi
AC_SUBST(WISH)


# Checks for header files.
AC_HEADER_STDC
AC_HEADER_TIME
AC_CHECK_HEADERS([malloc.h sys/time.h sys/timeb.h])
AC_HEADER_STDBOOL
AC_CHECK_HEADERS([sys/types.h pwd.h unistd.h])
AC_CHECK_FUNCS([getpwuid geteuid cuserid])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.
##AC_FUNC_MALLOC
AC_CHECK_FUNCS([ftime gettimeofday])

#Check For Swig (it is not needed for user as long as they don't do make distclean)
#If enabled check for python devel
MED_CHECK_SWIG

#Activer l'utilisation de l'API python.
MED_CHECK_PYTHON

# Initialize Autotest
AC_CONFIG_TESTDIR([tests/python])

## Configure Doxygen
## TODO : Créer une macro spécifique.
AC_SUBST_FILE(DOXYMEDFILE)
DOXYMEDFILE=$srcdir/doc/dox/MEDfile.als

AC_SUBST_FILE(DOXYMEDMESH)
DOXYMEDMESH=$srcdir/doc/dox/MEDmesh.als

AC_SUBST_FILE(DOXYMEDSUPPORTMESH)
DOXYMEDSUPPORTMESH=$srcdir/doc/dox/MEDsupportMesh.als

AC_SUBST_FILE(DOXYMEDFILTER)
DOXYMEDFILTER=$srcdir/doc/dox/MEDfilter.als

AC_SUBST_FILE(DOXYMEDLIBRARY)
DOXYMEDLIBRARY=$srcdir/doc/dox/MEDlibrary.als

AC_SUBST_FILE(DOXYMEDFIELD)
DOXYMEDFIELD=$srcdir/doc/dox/MEDfield.als

AC_SUBST_FILE(DOXYMEDFAMILY)
DOXYMEDFAMILY=$srcdir/doc/dox/MEDfamily.als

AC_SUBST_FILE(DOXYMEDEQUIVALENCE)
DOXYMEDEQUIVALENCE=$srcdir/doc/dox/MEDequivalence.als

AC_SUBST_FILE(DOXYMEDLINK)
DOXYMEDLINK=$srcdir/doc/dox/MEDlink.als

AC_SUBST_FILE(DOXYMEDJOINT)
DOXYMEDJOINT=$srcdir/doc/dox/MEDjoint.als

AC_SUBST_FILE(DOXYMEDLOCALIZATION)
DOXYMEDLOCALIZATION=$srcdir/doc/dox/MEDlocalization.als

AC_SUBST_FILE(DOXYMEDINTERP)
DOXYMEDINTERP=$srcdir/doc/dox/MEDinterp.als

AC_SUBST_FILE(DOXYMEDSTRUCTELEMENT)
DOXYMEDSTRUCTELEMENT=$srcdir/doc/dox/MEDstructElement.als

AC_SUBST_FILE(DOXYMEDPARAMETER)
DOXYMEDPARAMETER=$srcdir/doc/dox/MEDparameter.als

AC_SUBST_FILE(DOXYMEDPROFILE)
DOXYMEDPROFILE=$srcdir/doc/dox/MEDprofile.als

AC_SUBST_FILE(DOXYALIASES)
DOXYALIASES=./doc/dox/aliases.als

LT_OUTPUT

## Process a configuration summary
MED_SUMMARY

AC_CONFIG_FILES([Makefile
	   config/Makefile
	   include/2.3.6/Makefile
	   include/2.3.6/med.h
	   include/2.3.6/med23v30.h
	   include/2.3.6/med_utils.h
	   include/Makefile
	   include/H5public_extract.h
	   include/med.h
	   include/med_utils.h
           src/libmed3.settings
           src/Makefile
	   src/ci/Makefile
           src/fi/Makefile
	   src/cfi/Makefile
	   src/misc/Makefile
	   src/hdfi/Makefile 
           src/2.3.6/Makefile
	   src/2.3.6/ci/Makefile
           src/2.3.6/fi/Makefile
	   src/2.3.6/cfi/Makefile
	   src/2.3.6/misc/Makefile
	   src/2.3.6/hdfi/Makefile 
	   tests/Makefile
           tests/python/Makefile
           tests/python/atlocal
           tests/c/Makefile
	   tests/c/dumps.ref/Makefile
           tests/c/3.1.0/Makefile
           tests/c/3.1.0/dumps.ref/Makefile
           tests/c/3.0.8/Makefile
           tests/c/3.0.8/dumps.ref/Makefile
           tests/c/2.3.1/Makefile
	   tests/c/2.3.1/dumps.ref/Makefile
           tests/c/2.3.2/Makefile
	   tests/c/2.3.2/dumps.ref/Makefile
           tests/c/2.3.6/Makefile
           tests/c/2.3.6/dumps.ref/Makefile
           tests/c/2.3.1v3.0/Makefile
           tests/c/2.3.1v3.0/dumps.ref/Makefile
           tests/c/2.3.2v3.0/Makefile
           tests/c/2.3.2v3.0/dumps.ref/Makefile
           tests/c/2.3v3.0/Makefile
           tests/c/2.3v3.0/dumps.ref/Makefile
	   tests/f/Makefile
	   tests/f/dumps.ref/Makefile
           tests/f/3.1.0/Makefile
           tests/f/3.1.0/dumps.ref/Makefile
           tests/f/3.0.8/Makefile
           tests/f/3.0.8/dumps.ref/Makefile
           tests/f/2.3.1/Makefile
           tests/f/2.3.1/dumps.ref/Makefile
           tests/f/2.3.2/Makefile
           tests/f/2.3.2/dumps.ref/Makefile
           tests/f/2.3.6/Makefile
           tests/f/2.3.6/dumps.ref/Makefile
           tests/f/2.3v3.0/Makefile
           tests/f/2.3v3.0/dumps.ref/Makefile
           tests/f/2.3.1v3.0/Makefile
           tests/f/2.3.1v3.0/dumps.ref/Makefile
           tests/f/2.3.2v3.0/Makefile
           tests/f/2.3.2v3.0/dumps.ref/Makefile
	   tests/parallel/Makefile
	   tests/parallel/dumps.ref/Makefile
	   tests/parallel/f/Makefile
	   tests/usecases/Makefile
	   tests/usecases/c/Makefile
	   tests/usecases/c/dumps.ref/Makefile
	   tests/usecases/f/Makefile
           tests/usecases/f/dumps.ref/Makefile
	   tests/unittests/Makefile
	   tests/unittests/c/Makefile
	   tests/unittests/c/dumps.ref/Makefile
	   tests/unittests/c/MEDinterp/Makefile
	   tests/unittests/c/MEDinterp/dumps.ref/Makefile
	   tests/unittests/c/MEDmesh/Makefile
	   tests/unittests/c/MEDmesh/dumps.ref/Makefile
	   tests/unittests/f/Makefile
	   tests/unittests/f/dumps.ref/Makefile
           tools/Makefile
	   tools/mdump/Makefile
           tools/mdump/xmdump2
           tools/mdump/xmdump3
           tools/medconforme/Makefile
           tools/medimport/Makefile
           tools/medimport/include/Makefile
           tools/medimport/2.3.1/Makefile
           tools/medimport/2.1.x/Makefile
           tools/medimport/2.3.6/Makefile
           tools/medimport/3.0.0/Makefile
           tools/medimport/3.1.0/Makefile
           doc/Makefile
           doc/dox/Makefile
           doc/dox/aliases.als
           doc/dox/Doxyfile.cfg
	   doc/gif/Makefile
	   doc/html/Makefile
	   doc/html/root.html
	   doc/html.dox/Makefile
	   doc/jpg/Makefile
	   doc/png/Makefile
	   python/Makefile
	   CMakeLists.txt])


AC_OUTPUT

cat src/libmed3.settings


