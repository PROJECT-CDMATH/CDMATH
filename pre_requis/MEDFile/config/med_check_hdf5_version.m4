dnl  This file is part of MED.
dnl
dnl  COPYRIGHT (C) 1999 - 2012  EDF R&D, CEA/DEN
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

AC_DEFUN([AC_CHECK_HDF5_VERSION], [
 AC_CACHE_CHECK([HDF version is ok], [med_cv_hdf_version_isok],[
  AC_LANG_SAVE()
  AC_LANG([C])
  AC_RUN_IFELSE([
     AC_LANG_SOURCE([dnl
#include "H5public.h"
#include <stdio.h>

int main() {
 FILE *f=fopen ("conftestval","w");
 if (!f) return 1;

 int majeur=H5_VERS_MAJOR;
 int mineur=H5_VERS_MINOR;

 if ( 100*majeur+10*mineur < 170 )
        fprintf(f,"no");
 else
        fprintf(f,"yes");

 return 0;
}
      ])
  ],
  [med_cv_hdf_version_isok=`cat conftestval`
   test "x$med_cv_hdf_version_isok" = "xyes" && AC_MSG_RESULT([yes]) || AC_MSG_FAILURE([HDF version must be at least 1.8.x .])
  ],
  [AC_MSG_FAILURE([Can't determine HDF version.])],
  [med_cv_hdf_version_isok=yes
   AC_MSG_WARN([Can't compile med_check_hdf5_version test, assuming HDF version is ok]);
  ]
  )
  rm -f conftestval conftestf.$ac_objext
  AC_LANG_RESTORE()
 ])
 AC_DEFINE_UNQUOTED(MED_HDF_VERSION_ISOK,
                     [$med_cv_hdf_version_isok],
                     [The hdf version from H5public.h is ok for MED.])
])
