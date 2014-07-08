#
# MED_CHECK_SWIG( version, path = /usr )
# -------------------------------------
#
AC_DEFUN([_MED_DEFINE_SWIG_WITH],[

  AC_ARG_WITH([swig],
    AC_HELP_STRING([--with-swig@<:@=DIR@:>@], [set SWIG installation directory. @<:@]m4_default([$2], /usr)[@:>@]),
    [with_swig=$withval], [with_swig=no])

  AC_ARG_VAR([SWIGFLAGS],[The list of flags that should be passed to SWIG.])

])

AC_DEFUN([MED_CHECK_SWIG],
[
 AC_BEFORE([$0],[AX_PKG_SWIG])
 AC_BEFORE([$0],[AX_SWIG_PYTHON])
 AC_BEFORE([$0],[AX_PYTHON_DEVEL])
 AC_BEFORE([$0],[MED_CHECK_PYTHON])
 AC_REQUIRE([_MED_DEFINE_SWIG_WITH])

  WITH_SWIG=0

  SWIG='noswig() { echo "error: SWIG not available. Check configuration." ; return 1 ; } ; noswig'

  swig_default_path=m4_default([$2], /usr)
# echo "---------SWIG 1-$with_swig-----------------"
 AS_IF([test ! x${with_swig} = xno ],
 [  
    # we're trying to find the correct SWIG installation path
    swig_install_path=$swig_default_path
    if test ! x${with_swig} = xyes
    then
      swig_install_path=$with_swig
      PATH="${swig_install_path}/bin:${PATH}"
    fi

    AX_PKG_SWIG(m4_default([$1], 2.0.8),
    		[],
		[AC_MSG_ERROR([SWIG has been enable but can't find a valid version.])]
               )
# echo "---------SWIG 2-${ac_prog_swig_res}-----------------"
    # if test x${ac_prog_swig_res} = "xok" 
    # then
      #AX_SWIG_ENABLE_CXX
      #AX_SWIG_MULTI_MODULE_SUPPORT
#L'appel suivant axpythondevel est important car cette macro est required ds
#axswigpython et sera appeler par défaut si aucun appel explicite n'est détecté
    AX_PYTHON_DEVEL([>= '2.2'])
    AX_SWIG_PYTHON

    test -n "${SWIG}" && WITH_SWIG=1
    eval "${SWIG} -help >/dev/null 2>&1" && WITH_SWIG=1
    # fi
 ],[
 ])

  AC_SUBST(SWIG)

  # Propagate test into Makefiles
  AM_CONDITIONAL(WITH_SWIG, test $WITH_SWIG = 1)
])

# _MED_SWIG_VERSION
# ----------------
#
AC_DEFUN([_MED_SWIG_VERSION],
[
  AC_REQUIRE([AC_PROG_AWK])
  AC_MSG_CHECKING([Swig version])
  med_swig_version="undefined"
  if ${SWIG} -version >conftest.swigversion 2>&1; then
    med_swig_version=`grep Version conftest.swigversion | cut -d " " -f 3`
  fi
  AC_MSG_RESULT([$med_swig_version])
])
