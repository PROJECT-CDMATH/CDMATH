# thermique1d makefile

CC = g++
# Adapt the following line(s) to your own system:
PETSCDIR = /usr/lib/petscdir/3.4.2/include
MPIDIR = /usr/lib/openmpi/include
ICDMATHDIR = ~/workspace/cdmath/cdmath_install/include/
IFLAG = -I$(PETSCDIR) -I$(MPIDIR) -I$(ICDMATHDIR) -I.
# Adapt the following line to your own system:
LDIR = ~/workspace/cdmath/cdmath_install/lib/
LFLAG = -L$(LDIR)
LIBS  =-linterpkernel -lmedC -lmedloader -lmedcoupling -lbase -lmesh -llinearsolver
OBJ = main.o

all: $(OBJ)
	$(CC) -o main $^ $(IFLAG) $(LFLAG) $(LIBS)
	
%.o: %.cxx
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAG) $(LFLAG) $(LIBS)
	
.PHONY: clean

clean:
	rm -f *.o *~ core $(INCDIR)/*~

sweep:
	rm -f *.vtu
	rm -f *.pvd
