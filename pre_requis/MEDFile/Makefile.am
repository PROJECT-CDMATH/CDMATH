ACLOCAL_AMFLAGS = -I config

AUX_DIST	= $(ac_aux_dir)/config.guess \
		  $(ac_aux_dir)/config.sub \
		  $(ac_aux_dir)/install-sh \
		  $(ac_aux_dir)/ltconfig \
		  $(ac_aux_dir)/ltmain.sh \
		  $(ac_aux_dir)/mdate-sh \
		  $(ac_aux_dir)/missing \
		  $(ac_aux_dir)/mkinstalldirs \
		  $(ac_aux_dir)/depcomp \
		  $(ac_aux_dir)/libtool.m4 \
		  $(ac_aux_dir)/ltoptions.m4\
		  $(ac_aux_dir)/ltsugar.m4\
		  $(ac_aux_dir)/ltversion.m4\
		  $(ac_aux_dir)/lt~obsolete.m4

##AUX_DIST_EXTRA  = $(ac_aux_dir)/

EXTRA_DIST = bootstrap COPYING COPYING.LESSER $(ac_aux_dir)/automake.common $(ac_aux_dir)/automake.common.2.3.6 \
config/cmake_files/FindHDF5.cmake \
config/cmake_files/medMacros.cmake \
config/cmake_files/FindMPI.cmake \
CMakeLists.txt


MAINTAINERCLEANFILES = Makefile.in aclocal.m4 configure config-h.in stamp-h.in config.cache config.h.in config.log config.status $(AUX_DIST)

# Process the following directories
SUBDIRS = include src tests tools doc

