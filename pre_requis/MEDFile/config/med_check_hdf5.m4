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

AC_DEFUN([_MED_DEFINE_HDF5_ARGS],[
## Guess where as hdf5 is located by the HDF5HOME environement variable
## or by the --with-hdf5 option
 AC_ARG_WITH([hdf5],
             AC_HELP_STRING([--with-hdf5=<path>],
	                   [Use <path> for HDF5 path.]),
	    [],
[withval=yes])
])

AC_DEFUN([_MED_BEFORE_ENABLE_PMED], [
AC_REQUIRE([_MED_DEFINE_HDF5_ARGS])

dnl ## If user hasn't provided hdf5 path try using "$HDF5HOME" 
    test "x$withval" = "xyes" && hdf5home=${HDF5HOME} || hdf5home=$withval
    if test ! "x$withval" = "xno"; then

dnl ## If user hasn't provided hdf5 path and "$HDF5HOME"=="" use system path /usr 
       test -z "$hdf5home" && hdf5home="/usr/"
dnl #If user specified a specific hdf5home, be careful to not detect an hdf5 package in the system 
       if test ! -d "$hdf5home" ; then
         AC_MSG_ERROR([The directory |$hdf5home| doesn't exist, please verify either HDF5HOME or --with-hdf5=<path>])
       fi

       if test ! -d "$hdf5home/lib" ; then
         AC_MSG_ERROR([The directory |$hdf5home/lib| doesn't exist, please verify either HDF5HOME or --with-hdf5=<path>])
       fi

       if test ! -d "$hdf5home/include" ; then
         AC_MSG_ERROR([The directory |$hdf5home/include| doesn't exist, please verify either HDF5HOME or --with-hdf5=<path>])
       fi

    else
## In case user explicitly ask to not use hdf5 !
      AC_MSG_WARN([Can't compile MED without hdf5])
      AC_MSG_ERROR([either use HDF5HOME env. var. or --with-hdf5=<path>])
    fi
])


AC_DEFUN([MED_CHECK_HDF5],dnl
[

AC_REQUIRE([_MED_BEFORE_ENABLE_PMED])
AC_REQUIRE([MED_ENABLE_PMED])

      LDFLAGS_save="$LDFLAGS"
      LIBS_save="$LIBS"
      CPPFLAGS_save="$CPPFLAGS"

      LDFLAGS="-L$hdf5home/lib $LDFLAGS"
      CPPFLAGS="-I$hdf5home/include $CPPFLAGS "

      AX_ABSOLUTE_HEADER([H5public.h])
      HDF5_ABS_PATH="$gl_cv_absolute_H5public_h"
      HDF5_USED_VER="unknown"
      if test -n "$HDF5_ABS_PATH" ; then 
         HDF5_USED_VER=` grep H5_VERS_INFO $HDF5_ABS_PATH | sed  's/\([[^"]]*\)\(".*"\)\([[^"]]*\)/\2/g' `
      fi
      AX_ABSOLUTE_HEADER([H5Ipublic.h])

      HDF5_TYPEDEF_HERR_T=` sed -n '/^[[ \t]]*typedef .*herr_t[[ \t]]*;/p' $HDF5_ABS_PATH `
      HDF5_TYPEDEF_HID_T=` sed -n '/^[[ \t]]*typedef .*hid_t[[ \t]]*;/p' $gl_cv_absolute_H5Ipublic_h `
      HDF5_TYPEDEF_HSIZE_T=` sed -n '/^[[ \t]]*typedef .*hsize_t[[ \t]]*;/p' $HDF5_ABS_PATH `

#      AC_CHECK_LIB([m],[sin],[],[AC_MSG_ERROR([Can't find C math library.])],[])
#      AC_CHECK_LIB([z],[compress],[],[AC_MSG_ERROR([Can't find z library.])],[])
      AC_CHECK_LIB([hdf5],[H5open],[],[AC_MSG_ERROR([either use HDF5HOME env. var. or --with-hdf5=<path>])],[-lm -lz])

      AC_PATH_PROG([H5DUMP],[h5dump],[no],[$hdf5home/bin$PATH_SEPARATOR$PATH$PATH_SEPARATOR])
      if test "X$H5DUMP" = "Xno"; then
         AC_MSG_ERROR([Can't find h5dump, please verify your env. var. PATH])
      fi



#      AC_DEFINE(H5_USE_16_API,[],[Using HDF5-1.6 API compatibility with HDF5 1.8 libraries])
      HDF5_CPPFLAGS="-DH5_USE_16_API"


dnl TODO : VERIFIER LES SOUS CHAINES !
dnl        if test ! AS_DIRNAME(["$hdf5home"]) = AS_DIRNAME(["$HDF5_ABS_PATH"]) ; then
dnl          AC_MSG_WARN([BE CAREFUL, we use $hdf5home as HDF5HOME but found H5public.h in $HDF5_ABS_PATH ])
dnl        fi
dnl On ne veut pas forcément utiliser le h5dump de $hdf5home/bin (cross compilation)
dnl        if test ! AS_DIRNAME(["$hdf5home"]) = AS_DIRNAME(["$H5DUMP"]) ; then
dnl          AC_MSG_WARN([BE CAREFUL, we use $hdf5home as HDF5HOME but found h5dump in $H5DUMP ])
dnl        fi

      HDF5_CPPFLAGS="$HDF5_CPPFLAGS -I$hdf5home/include"
      HDF5_LDFLAGS="-L$hdf5home/lib"
      LDFLAGS="$LDFLAGS_save"
      CPPFLAGS="$CPPFLAGS_save"
      LIBS="$LIBS_save"

      AC_SUBST(HDF5_CPPFLAGS)
      AC_SUBST(HDF5_LDFLAGS)
#La ligne suivante est inutile car effectuée par AC_CHECK_PROG 
#      AC_SUBST(H5DUMP)
      AC_SUBST([HDF5_USED_VER])
      HDF5_LIBS="-lhdf5"
      AC_SUBST([HDF5_LIBS])

      AC_SUBST(HDF5_TYPEDEF_HERR_T)
      AC_SUBST(HDF5_TYPEDEF_HID_T)
      AC_SUBST(HDF5_TYPEDEF_HSIZE_T)
])

