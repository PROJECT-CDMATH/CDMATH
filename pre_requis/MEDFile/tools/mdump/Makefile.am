include $(top_srcdir)/config/automake.common

DEFAULT_INCLUDES = -I. -I$(srcdir)
AM_CPPFLAGS+= -DMED_MESGERR_FOR_TOOLS

bin_PROGRAMS = mdump
mdump_SOURCES = mdump.c MED23meshIterators.c MED23fieldIterators.c
nodist_EXTRA_mdump_SOURCES = dummy.cxx
mdump_LDADD  = $(top_builddir)/src/libmed.la

EXTRA_DIST = xmdump.in CMakeLists.txt
bin_SCRIPTS = xmdump

bin_PROGRAMS += mdump3
mdump3_SOURCES = mdump3.c MED23meshIterators.c MED23fieldIterators.c
nodist_EXTRA_mdump3_SOURCES = dummy.cxx
mdump3_LDADD  = $(top_builddir)/src/libmed.la

EXTRA_DIST += xmdump3.in
bin_SCRIPTS += xmdump3
