include $(top_srcdir)/config/automake.common

DEFAULT_INCLUDES = -I. -I$(srcdir)

AUTOMAKE_OPTIONS = foreign

if ENABLE_FORTRAN
SUBDIRS 	 = 2.3.6 ci misc hdfi fi cfi .
else
SUBDIRS 	 = 2.3.6 ci misc hdfi .
endif

##include_HEADERS	 = med.h

#TODO : Déplacer MEDaffiche.c
lib_LTLIBRARIES   = libmed.la libmedC.la 
libmed_la_SOURCES =  MEDiterators.c
nodist_EXTRA_libmed_la_SOURCES = dummy.cxx

#TODO : Créer une release pour une nvlle bilbiothèque de l'ancienne API libmed23 compilée conditionellement

# info libtool/versioning:
# 3. If the library source code has changed at all since the last
#    update, then increment REVISION (`C:R:A' becomes `C:r+1:A').
# 4. If any interfaces have been added, removed, or changed since the
#    last update, increment CURRENT, and set REVISION to 0.
# 5. If any interfaces have been added since the last public release,
#    then increment AGE.
# 6. If any interfaces have been removed or changed since the last
#    public release, then set AGE to 0.
#
# A la mise à jour du mineur de la bibliothèque med, on a x+1:0:z+1 pour la version libtool 
# ce qui permet aux programmes liés à l'ancienne bibliothèque de mineur-1
# d'utiliser la nouvelle bibliothèque pour lire/modifier les anciens fichiers
# et en créer de nouveaux en version mineur courante.
# Les anciennes bibliotèques refuseront de lire un fichier dont le mineur est supérieur à celui 
# de la bibliothèque 
#
#TODO : Changer le versionement libtool
libmed_la_LDFLAGS = -no-undefined -version-info 8:0:7 @HDF5_LDFLAGS@ @HDF5_LIBS@
if ENABLE_FORTRAN
libmed_la_SOURCES += MEDiteratorsF.f
#Regénère les objets fortran si le compilateur Fortran a changé dans le Makefile
MEDiteratorsF.lo: Makefile
libmed_la_LIBADD  = 2.3.6/libmed23.la 2.3.6/libmedVV.la ci/libci.la fi/libfi.la cfi/libcfi.la misc/libmisc.la hdfi/libhdfi.la $(FLIBS)
else
libmed_la_LIBADD  = 2.3.6/libmed23.la 2.3.6/libmedVV.la ci/libci.la misc/libmisc.la hdfi/libhdfi.la
endif

libmedC_la_SOURCES = MEDiterators.c
#Astuce pour forcer la reconstruction de la bibliothèque en cas de changement du Makefile (ex:dépendance)
MEDiterators.lo: Makefile
nodist_EXTRA_libmedC_la_SOURCES = dummy.cxx
#TODO : Changer le versionement libtool
libmedC_la_LDFLAGS = -no-undefined -version-info 8:0:7 @HDF5_LDFLAGS@ @HDF5_LIBS@
libmedC_la_LIBADD  = 2.3.6/libmedC23.la 2.3.6/libmedVV.la ci/libci.la misc/libmisc.la hdfi/libhdfi.la

settingsdir = $(libdir)
nodist_settings_DATA = libmed3.settings
libmed3.settings: Makefile

EXTRA_DIST = CMakeLists.txt  libmed3.settings.in
CLEANFILES = libmed3.settings


