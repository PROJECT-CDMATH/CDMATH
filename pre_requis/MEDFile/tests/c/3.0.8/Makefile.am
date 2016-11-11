include $(top_srcdir)/config/automake.common

DEFAULT_INCLUDES = -I. -I$(srcdir)

AM_CFLAGS = -DDEF_LECT_ECR
#AM_CFLAGS = -DDEF_LECT_AJOUT

#noinst_PROGRAMS = test10 test11 
#nodist_EXTRA_test10_SOURCES = dummy.cxx
#nodist_EXTRA_test11_SOURCES = dummy.cxx

LDADD = $(top_builddir)/src/libmedC.la
#EXTRA_DIST = test10.sh test11.sh test31.sh


TESTS_ENVIRONMENT = eval H5DUMP=@H5DUMP@ SRCDIR="$(abs_srcdir)" BUILDDIR="$(abs_builddir)" \
		EXECDIR="$(abs_builddir)/../" CP=${CP} CPYMED="yes" AWK=${AWK}

TESTS_LEC = ../test3.sh ../test5.sh ../test7.sh ../test9.sh \
	../test11.sh ../test11b.sh ../test13.sh ../test15.sh ../test17.sh \
	../test22.sh ../test24.sh ../test26.sh ../test28.sh \
	../test30.sh ../test32.sh ../test33.sh

TESTS_ECR = ../test1.sh ../test2.sh ../test4.sh ../test6.sh \
	../test8.sh ../test10.sh ../test12.sh ../test14.sh ../test16.sh \
	../test18.sh ../test20.sh ../test21.sh ../test23.sh \
	../test25.sh ../test27.sh ../test29.sh ../test31.sh

# ../test19.sh

TESTS =  $(TESTS_ECR) $(TESTS_LEC)

EXTRA_DIST = test.med.ref

CLEANFILES = \
./test1.med\
./test2.med\
./test2-med.hdf\
./test4.med\
./test6.med\
./test8.med\
./test10.med\
./test10-med.hdf\
./test12.med\
./test14.med\
./test16.med\
./test18.med\
./test19.med\
./test20.med\
./test20-0.med\
./test20b.med\
./test21.med\
./test23.med\
./test25.med\
./test27.med\
./test29.med\
./test31.med\
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
./test8.dump

SUBDIRS = dumps.ref .
