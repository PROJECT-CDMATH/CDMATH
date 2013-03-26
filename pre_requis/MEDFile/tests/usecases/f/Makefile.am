include $(top_srcdir)/config/automake.common

DEFAULT_INCLUDES = -I. -I$(srcdir)

if ENABLE_FORTRAN

TESTS_ENVIRONMENT = eval H5DUMP=@H5DUMP@ SRCDIR=$(srcdir) SRCDIR="$(abs_srcdir)" BUILDDIR="$(abs_builddir)" \
		EXECDIR="$(abs_builddir)" CP=${CP} CPYMED="no"

AM_LDFLAGS+= $(FTLDFLAGS)
LIBS += $(FTLIBS)
AM_LIBTOOLFLAGS = --tag=F77

testfdir = $(bindir)/testf

if WITHF90

mycheckPROGRAMS = \
UsesCase_MEDmesh_2 \
UsesCase_MEDmesh_3 \
UsesCase_MEDmesh_5 \
UsesCase_MEDmesh_11\
UsesCase_MEDfield_2\
UsesCase_MEDfield_3\
UsesCase_MEDfield_5\
UsesCase_MEDfield_6

myTESTS_LEC = \
UsesCase_MEDmesh_2.sh \
UsesCase_MEDmesh_3.sh \
UsesCase_MEDmesh_5.sh \
UsesCase_MEDmesh_11.sh\
UsesCase_MEDfield_2.sh\
UsesCase_MEDfield_3.sh\
UsesCase_MEDfield_5.sh\
UsesCase_MEDfield_6.sh

UsesCase_MEDmesh_2_SOURCES  = UsesCase_MEDmesh_2.f90
UsesCase_MEDmesh_3_SOURCES  = UsesCase_MEDmesh_3.f90
UsesCase_MEDmesh_5_SOURCES  = UsesCase_MEDmesh_5.f90
UsesCase_MEDmesh_11_SOURCES = UsesCase_MEDmesh_11.f90
UsesCase_MEDfield_2_SOURCES = UsesCase_MEDfield_2.f90
UsesCase_MEDfield_3_SOURCES = UsesCase_MEDfield_3.f90
UsesCase_MEDfield_5_SOURCES = UsesCase_MEDfield_5.f90
UsesCase_MEDfield_6_SOURCES = UsesCase_MEDfield_6.f90

endif

TESTS_LEC = $(myTESTS_LEC)

TESTS_ECR = \
UsesCase_MEDmesh_1.sh \
UsesCase_MEDmesh_4.sh \
UsesCase_MEDmesh_10.sh \
UsesCase_MEDfield_1.sh \
UsesCase_MEDfield_4.sh

TESTS = $(TESTS_ECR) $(TESTS_LEC)

EXTRA_DIST = \
UsesCase_MEDmesh_1.sh \
UsesCase_MEDmesh_4.sh \
UsesCase_MEDmesh_10.sh \
UsesCase_MEDfield_1.sh \
UsesCase_MEDfield_4.sh $(myTESTS_LEC)

LDADD = $(top_builddir)/src/libmed.la

testf_PROGRAMS = \
UsesCase_MEDmesh_1 \
UsesCase_MEDmesh_4 \
UsesCase_MEDmesh_10 \
UsesCase_MEDfield_1 \
UsesCase_MEDfield_4 $(mycheckPROGRAMS)

UsesCase_MEDmesh_1_SOURCES  = UsesCase_MEDmesh_1.f 
UsesCase_MEDmesh_4_SOURCES  = UsesCase_MEDmesh_4.f
UsesCase_MEDmesh_10_SOURCES = UsesCase_MEDmesh_10.f
UsesCase_MEDfield_1_SOURCES = UsesCase_MEDfield_1.f
UsesCase_MEDfield_4_SOURCES = UsesCase_MEDfield_4.f

CLEANFILES= \
UsesCase_MEDmesh_1.med \
UsesCase_MEDmesh_4.med \
UsesCase_MEDmesh_10.med \
UsesCase_MEDfield_1.med \
UsesCase_MEDfield_4.med \
UsesCase_MEDmesh_1.dump \
UsesCase_MEDmesh_4.dump \
UsesCase_MEDmesh_10.dump\
UsesCase_MEDfield_1.dump\
UsesCase_MEDfield_4.dump

SUBDIRS = dumps.ref .

endif
