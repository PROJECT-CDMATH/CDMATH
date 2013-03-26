## For having included files in DISTribution & INSTallation (public headers)
include_HEADERS =  med.hf med.hf77 med.hf90 med_parameter.hf med_proto.h \
		 medC_win_dll.h MEDerreur.hxx med_exit_if.h med_err.h

## For having included files in  INSTallation (public headers)
nodist_include_HEADERS = med.h med_utils.h 

## For having included files in DISTribution only (private headers)
noinst_HEADERS = med_outils.h  med_misc.h  \
	med_hdfi.h med_versioned_proto.h med_versioned_proto_en.h \
	med_versioned_hdfi.h med_versioned_cfi.h med_versioned.h \
	MEDversionedApi3.hxx MEDgetGeometryTypeFromName.hxx MEDmodeAcces.hxx \
	MEDfileVersion.hxx MEDfieldChecked.hxx med_vargs.h

# Ce fichier ne doit pas être dans la distribution, il doit être généré
#nobase_include_HEADERS = med/med_config.h

SUBDIRS = 2.3.6 . 

CLEANFILES = med_config.h med.h med_utils.h 
MAINTAINERCLEANFILES = med_config.h.in Makefile.in
EXTRA_DIST = CMakeLists.txt

