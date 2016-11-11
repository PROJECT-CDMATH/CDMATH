dnl  This file is part of MED.
dnl
dnl  COPYRIGHT (C) 1999 - 2016  EDF R&D, CEA/DEN
dnl  MED is free software: you can redistribute it and/or modify
dnl  it under the terms of the GNU Lesser General Public License as published by
dnl  the Free Software Foundation, either version 3 of the License, or
dnl  (at your option) any later version.
dnl
dnl  MED is distributed in the hope that it will be useful,
dnl  but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl  GNU Lesser General Public License for more details.
dnl
dnl  You should have received a copy of the GNU Lesser General Public License
dnl  along with MED.  If not, see <http://www.gnu.org/licenses/>.
dnl

AC_DEFUN([_MED_DEFINE_F90_ARGS],[
## Guess where as f90prog is defined by the F90 environement variable
## or by the --with-f90 option
 AC_ARG_WITH([f90],
             AC_HELP_STRING([--with-f90=<compiler>],
 	                   [Use <compiler> for F90 compiler]),
 	    [
#echo "---------1------ withval : ${withval} -----------------"
dnl AC_MSG_WARN([-- MACRO APPELEE 1 ----])
            ], [withval=no
#echo "---------2------ withval : ${withval} -----------------"
dnl AC_MSG_WARN([-- MACRO APPELEE 2 ----])
])
#echo "---------F90-1------ withval : ${withval} -----------------"
#echo "---------F90-2------ withval : ${withval} -----------------"

])
 
AC_DEFUN([_MED_BEFORE_FC], [
  AC_REQUIRE([_MED_DEFINE_F90_ARGS])
#echo "---------1------ enable_fortran : $enable_fortran -----------------"
#echo "---------1------ med_check_f90 : ${med_check_f90} -----------------"

test x"$withval" = xyes && f90prog=${F90} 

#echo "---------2------ enable_fortran : $enable_fortran -----------------"
#echo "---------2------ med_check_f90 : ${med_check_f90} -----------------"

test ! x"$withval" = xyes && test ! x"$withval" = xno && f90prog=$withval

#echo "---------3------ enable_fortran : $enable_fortran -----------------"
#echo "---------3------ med_check_f90 : ${med_check_f90} -----------------"

med_check_f90="no"

#echo "---------4------ enable_fortran : $enable_fortran -----------------"
#echo "---------4------ med_check_f90 : ${med_check_f90} -----------------"

(test ! x"$withval" = xno||(test x"$withval" = xno &&(test ! x"$FC" = x||test ! x"$MPIFC" = x )))&&med_check_f90="yes"

test x"$enable_fortran" = xno && med_check_f90="no"
#echo "---------5------ enable_fortran : $enable_fortran -----------------"
#echo "---------5------ med_check_f90 : ${med_check_f90} -----------------"
])

AC_DEFUN([_MED_CALLING_FC], [

  AX_PROG_FC_MPI(test "x$mpi_wanted_test" = "xyes",[],[
  if test x"$mpi_wanted_test" = xyes && test x"$med_check_f90" = xyes ; then
    AC_MSG_ERROR([MPI F90 compiler requested, but couldn't use MPI.])
  fi
  ])
#echo "--------- Inside0 ------ FC : |${FC}| -----------------"
#echo "---------5------ med_check_f90 : ${med_check_f90} -----------------"
  test x"$FC" = x && test x"$med_check_f90" = xyes && AC_MSG_ERROR([Can't find a valid FC compiler, check your configure switches or the FC var...])
])

AC_DEFUN_ONCE([MED_CHECK_F90],dnl
[
AC_BEFORE([$0],[AC_PROG_FC])
AC_BEFORE([$0],[AC_PROG_F77])
AC_REQUIRE([_MED_BEFORE_FC])

_MED_CALLING_FC

# echo "--------- Inside1 ------ FC : ${FC} -----------------"

AS_IF([test  x"$FC" != x && test x"$med_check_f90" = xyes ],
[
## Looking for "$f90prog" presence 
# echo "--------- Inside2 ------ withval : ${withval} -----------------"
  AC_FC_LIBRARY_LDFLAGS
  ##AC_FC_WRAPPERS Inutile car pas d'interface spécifique f90 

  if test -z "$FC" ; then
    AC_MSG_ERROR([Can't find $f90prog command, please verify your env. var. PATH])
  fi 

if test x"$mpi_wanted_test" = xno; then
    AC_LANG_PUSH([Fortran])
    AC_MSG_NOTICE([testing a F90 compilation])
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM(,[[
      print *,"Testing a F90 compilation..."
]])],
        [ AC_MSG_RESULT(ok)],
        [ AC_MSG_RESULT(ko)
          AC_MSG_ERROR([Can't find a valid FC compiler, check your configure switches or the FC var...])
        ]
    )
    AC_LANG_POP([Fortran])
fi

# Pour l'affichage du Summary
   BUILD_F90TESTS_CONDITIONAL_TRUE=
   BUILD_F90TESTS_CONDITIONAL_FALSE='#'
],[
   FC="" 
# Pour l'affichage du Summary
   BUILD_F90TESTS_CONDITIONAL_TRUE='#'
   BUILD_F90TESTS_CONDITIONAL_FALSE=
])

AC_SUBST([BUILD_F90TESTS_CONDITIONAL_TRUE])
AC_SUBST([BUILD_F90TESTS_CONDITIONAL_FALSE])

])

