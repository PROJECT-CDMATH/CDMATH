# amr makefile

CC = g++
# Adapt the following line to your own system:
CDMATHDIR = ../..
IFLAG = -I$(CDMATHDIR)/include -I.
LFLAG = -L$(CDMATHDIR)/lib
LIBS  =-linterpkernel -lmedC -lmedloader -lmedcoupling -lbase -lmesh -llinearsolver
PARAFLAG = -fopenmp
EXTRAFLAGS = -g $(PARAFLAG) #-Wall -Wextra

ODIR = build
_OBJ = main.o AMR.o IterativeProblem.o HeatSolver.o AdvectionSolver.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: $(OBJ)
	$(CC) -o main $^ $(IFLAG) $(LFLAG) $(LIBS) $(EXTRAFLAGS)
	
$(ODIR)/%.o: %.cxx
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAG) $(LFLAG) $(LIBS) $(EXTRAFLAGS)
	
.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~

sweep:
	mv *.vtu output/
	mv *.pvd output/
	mv *.vti output/
	
doc:
	doxygen Doxyfile
