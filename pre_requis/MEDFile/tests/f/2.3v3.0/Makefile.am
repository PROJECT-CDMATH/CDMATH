include $(top_srcdir)/config/automake.common

DEFAULT_INCLUDES = -I. -I$(srcdir)

#AM_CFLAGS = -DDEF_LECT_ECR
#AM_CFLAGS = -DDEF_LECT_AJOUT

AM_LDFLAGS+= $(FTLDFLAGS)
LIBS += $(FTLIBS)
AM_LIBTOOLFLAGS = --tag=F77

test2_SOURCES= test2.f
noinst_PROGRAMS = test2

if WITHF90

mycheckPROGRAMS = test5 test7 test9 test11 test11b test13 test15 test17 test30

myTESTS_LEC = ../test5.sh ../test7.sh ../test9.sh ./test11.sh ../test11b.sh ../test13.sh ../test15.sh ../test17.sh ../test30.sh

test11_SOURCES= test11.f90
noinst_PROGRAMS += test11

endif


TESTS_ENVIRONMENT = eval H5DUMP=@H5DUMP@ SRCDIR="$(abs_srcdir)" BUILDDIR="$(abs_builddir)" \
		EXECDIR="$(abs_builddir)/../" CP=${CP} CPYMED="no" AWK=${AWK}

TESTS_LEC = ./test2.sh ../test3.sh  ../test20.sh ../test22.sh ../test24.sh ../test26.sh ../test28.sh ../test32.sh ../test33.sh $(myTESTS_LEC)

TESTS = $(TESTS_LEC)

EXTRA_DIST = test2.sh test11.sh
LDADD = $(top_builddir)/src/libmed.la

CLEANFILES = \
./test10.dump\
./test12.dump\
./test14.dump\
./test16.dump\
./test18.dump\
./test19.dump\
./test1.dump\
./test20.dump\
./test21.dump\
./test23.dump\
./test25.dump\
./test27.dump\
./test29.dump\
./test31.dump\
./test2.dump\
./test4.dump\
./test6.dump\
./test8.dump \
./test10.med\
./test12.med\
./test14.med\
./test16.med\
./test18.med\
./test19.med\
./test1.med\
./test20.med\
./test20-0.med\
./test21.med\
./test23.med\
./test25.med\
./test27.med\
./test29.med\
./test2.med\
./test31.med\
./test4.med\
./test6.med\
./test8.med



SUBDIRS = dumps.ref .

all-local:
	${LN_S} -f $(abs_srcdir)/../2.3.6/dumps.ref/*.med . && chmod 644 *.med
