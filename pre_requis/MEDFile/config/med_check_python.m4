#
# MED_CHECK_PYTHON
# ---------------
#

AC_DEFUN([_MED_DEFINE_PYTHON_ENABLE],[

 AC_ARG_ENABLE(python, [AS_HELP_STRING([--enable-python],
    [Install the MED python library.  Default: enabled])
 ],[
# echo "---------PYTHON 1-$enable_python----------------------"
 enable_python=$enableval
# echo "---------PYTHON 1b-$enable_python----------------------"
 ],[
# echo "---------PYTHON 2-$enable_python----------------------"
 enable_python=yes
# echo "---------PYTHON 2b-$enable_python----------------------"

 ])
])

AC_DEFUN([MED_CHECK_PYTHON], [
 AC_BEFORE([$0],[AM_PATH_PYTHON])
 AC_REQUIRE([_MED_DEFINE_PYTHON_ENABLE])

 test x"$WITH_SWIG" = x1 && test x"$enable_python" = xno && AC_MSG_ERROR([python is needed because swig is activated.])
# test x"$WITH_SWIG" = x0 && test x"$enable_python" = xyes && test ! x"$LONG_OR_INT" == "xint" && AC_MSG_ERROR([Either use --disable-python or --with-swig=[yes|<swigdir>]. Swig is needed because embedded python interfaces has been generated for --with-med_int=int.])
 test x"$WITH_SWIG" = x0 && test x"$enable_python" = xyes && test x"$BUILD_WITH_PMED" == "xyes" && AC_MSG_ERROR([Either use --disable-python or --with-swig=[yes|<swigdir>]. Swig is needed because embedded python interfaces has been generated without MPI.])

 AS_IF([test x$enable_python = xyes || test x"$WITH_SWIG" = x1],
 [
  # echo "---------PYTHON 3- $enable_python -------- $WITH_SWIG --------------"
  AM_PATH_PYTHON([2.2],[
      test ! x"$WITH_SWIG" = x1 && AX_PYTHON_DEVEL([>= '2.2'])	 
      # echo "---------PYTHON 4- $pythonexists ----------------------"
      AC_SUBST([python_bin],
               [`basename ${PYTHON}`] )
               
      AC_SUBST([PYTHON_PREFIX],
               [`${PYTHON} -c "import sys; print (sys.prefix)"`] )
      
      AC_SUBST([python_includedir],
               [`${PYTHON} -c "import sys; print (sys.prefix + \"/include/python\" + str(sys.version_info[[0]]) + \".\" + str(sys.version_info[[1]]))"`] )
      
      AC_SUBST([python_libdir],
               [`${PYTHON} -c "import sys; print (sys.prefix + \"/lib/python\" + str(sys.version_info[[0]]) + \".\" + str(sys.version_info[[1]]) + \"/m4\")"`] )
      
      AC_SUBST([python_version],
               [`${PYTHON} -c "import sys; print (str(sys.version_info[[0]]) + \".\" + str(sys.version_info[[1]]))"`] )
  ],[AC_MSG_FAILURE([PYTHON interpreter requested, but couldn't find python >=2.2])
  ])

  # Pour l'affichage du Summary
  BUILD_PYTHONTESTS_CONDITIONAL_TRUE=
  BUILD_PYTHONTESTS_CONDITIONAL_FALSE='#'
 ],[
  # Pour l'affichage du Summary
  BUILD_PYTHONTESTS_CONDITIONAL_TRUE='#'
  BUILD_PYTHONTESTS_CONDITIONAL_FALSE=
 ])
 
 AC_SUBST([BUILD_PYTHONTESTS_CONDITIONAL_TRUE])
 AC_SUBST([BUILD_PYTHONTESTS_CONDITIONAL_FALSE])


 AM_CONDITIONAL([WITH_PYTHON],[test "x$enable_python" = "xyes" ])
 test x"$enable_python" = xyes && AC_DEFINE(MED_HAVE_PYTHON,1,
   [Define if the Python part of MED library must be installed.])
 test ! x"$enable_python" = xyes && AC_DEFINE(MED_HAVE_PYTHON,0,
   [Define if the Python part of MED library must be installed.])

])
