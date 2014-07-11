/*
 * LinearSolver.cxx
 *
 *  Created on: 15 avr. 2013
 *      Author: mekkas
 */

#include "LinearSolver.hxx"

#include "CdmathException.hxx"

#include <string>
#include <cmath>


#include <petscvec.h>
#include <petscmat.h>
#include <petscksp.h>

using namespace std;

LinearSolver::LinearSolver ( void )
{
	_tol=1.E-15;
	_numberMaxOfIter=0;
	_nameOfMethod="";
	_residu=1.E30;
	_convergence=false;
	_numberOfIter=0;
	_isSingular=false;
	_nameOfPc="";
}

void
LinearSolver::setTolerance(double tol)
{
	_tol=tol;
}

void
LinearSolver::setNumberMaxOfIter(int numberMaxOfIter)
{
	_numberMaxOfIter=numberMaxOfIter;
}

void
LinearSolver::setNameOfMethod(string nameOfMethod)
{
	_nameOfMethod=nameOfMethod;
	if ((_nameOfPc.size()>0 && (_nameOfMethod.compare("GMRES")!=0 && _nameOfMethod.compare("BICG")!=0)))
	{
		string msg="LinearSolver::LinearSolver : preconditioner "+_nameOfPc+" is not yet implemented.\n";
		msg+="The preconditioners implemented are : LU for GMRES and BICG methods.";
	    throw CdmathException(msg);
	}
}

void
LinearSolver::setNameOfPc(string nameOfPc)
{
	_nameOfPc=nameOfPc;
	if ((_nameOfPc.size()>0 && (_nameOfMethod.compare("GMRES")!=0 && _nameOfMethod.compare("BICG")!=0)))
	{
		string msg="LinearSolver::LinearSolver : preconditioner "+_nameOfPc+" is not yet implemented.\n";
		msg+="The preconditioners implemented are : LU for GMRES and BICG methods.";
	    throw CdmathException(msg);
	}
}

LinearSolver::LinearSolver( const Matrix& matrix, const Vector& vector, int numberMaxOfIter, double tol, string nameOfMethod )
{
	_tol=tol;
	_nameOfMethod=nameOfMethod;
	_numberMaxOfIter=numberMaxOfIter;
	_matrix=matrix;
	_vector=vector;
	_residu=1.E30;
	_convergence=false;
	_numberOfIter=0;
	_isSingular=false;
	_nameOfPc="";
}

LinearSolver::LinearSolver( const Matrix& matrix, const Vector& vector, int numberMaxOfIter, double tol, string nameOfMethod, string pc )
{
	_tol=tol;
	_nameOfMethod=nameOfMethod;
	_nameOfPc=pc;
	_numberMaxOfIter=numberMaxOfIter;
	_matrix=matrix;
	_vector=vector;
	_residu=1.E30;
	_convergence=false;
	_numberOfIter=0;
	_isSingular=false;
	if ((_nameOfPc.size()>0 && (_nameOfMethod.compare("GMRES")!=0 && _nameOfMethod.compare("BICG")!=0)))
	{
		string msg="LinearSolver::LinearSolver : preconditioner "+_nameOfPc+" is not yet implemented.\n";
		msg+="The preconditioners implemented are : LU for GMRES and BICG methods.";
	    throw CdmathException(msg);
	}
}

bool
LinearSolver::isSingular( void ) const
{
	return _isSingular;
}

int
LinearSolver::getNumberOfIter( void ) const
{
	return _numberOfIter;
}

bool
LinearSolver::getStatus( void ) const
{
	return _convergence;
}

double
LinearSolver::getResidu( void ) const
{
	return _residu;
}

double
LinearSolver::getTolerance(void) const
{
	return _tol;
}

int
LinearSolver::getNumberMaxOfIter(void) const
{
	return _numberMaxOfIter;
}

void
LinearSolver::setMatrix(const Matrix& matrix)
{
	_matrix=matrix;
}

void
LinearSolver::setSndMember(const Vector& vector)
{
	_vector=vector;
}

void
LinearSolver::setSingularity(bool sing)
{
	_isSingular=sing;
}

Matrix
LinearSolver::getMatrix(void) const
{
	return _matrix;
}

Vector
LinearSolver::getSndMember(void) const
{
	return _vector;
}

string
LinearSolver::getNameOfMethod(void) const
{
	return _nameOfMethod;
}

string
LinearSolver::getNameOfPc(void) const
{
	return _nameOfPc;
}

LinearSolver::LinearSolver ( const LinearSolver& LS )
{
	_tol=LS.getTolerance();
	_nameOfMethod=LS.getNameOfMethod();
	_numberMaxOfIter=LS.getNumberMaxOfIter();
	_matrix=LS.getMatrix();
	_vector=LS.getSndMember();
	_residu=LS.getResidu();
	_convergence=LS.getStatus();
	_numberOfIter=LS.getNumberOfIter();
	_isSingular=LS.isSingular();
	_nameOfPc=LS.getNameOfPc();
}

Vector
LinearSolver::solve( void )
{
	PetscInitialize(0,(char ***)"", PETSC_NULL, PETSC_NULL);
    /* matrix to mat */
	Mat A;
    int numberOfRows=getMatrix().getNumberOfRows();
    int numberOfColumns=getMatrix().getNumberOfColumns();

    MatCreate(PETSC_COMM_WORLD, &A);
	MatSetSizes(A, PETSC_DECIDE, PETSC_DECIDE, numberOfRows, numberOfColumns);
	MatSetType(A,MATSEQDENSE);

	PetscScalar    *a;
	PetscMalloc(numberOfRows*numberOfColumns*sizeof(PetscScalar),&a);
	for (int i=0;i<numberOfRows;i++)
		for (int j=0;j<numberOfColumns;j++)
			a[i+j*numberOfRows]=_matrix(i,j);

    MatSeqDenseSetPreallocation(A,a);

	//Assemblage final
	MatAssemblyBegin(A, MAT_FINAL_ASSEMBLY);
	MatAssemblyEnd(A, MAT_FINAL_ASSEMBLY);

	Vec B=vectorToVec(getSndMember());

	KSP ksp;
	KSPCreate(PETSC_COMM_WORLD, &ksp);
	KSPSetOperators(ksp,A,A,SAME_NONZERO_PATTERN);
	KSPSetTolerances(ksp,getTolerance(),PETSC_DEFAULT,PETSC_DEFAULT,getNumberMaxOfIter());
	PC prec;
	KSPGetPC(ksp,&prec);
	if (_nameOfMethod.compare("GMRES")==0)
	{
	   KSPSetType(ksp,KSPGMRES);
	   if (_nameOfPc.compare("LU")==0) PCSetType(prec,PCLU);
	}
	else if (_nameOfMethod.compare("LGMRES")==0)
		KSPSetType(ksp,KSPLGMRES);
	else if (_nameOfMethod.compare("CG")==0)
		KSPSetType(ksp,KSPCG);
	else if (_nameOfMethod.compare("BCG")==0)
		KSPSetType(ksp,KSPBCGSL);
	else if (_nameOfMethod.compare("CR")==0)
		KSPSetType(ksp,KSPCR);
	else if (_nameOfMethod.compare("CGS")==0)
		KSPSetType(ksp,KSPCGS);
	else if (_nameOfMethod.compare("BICG")==0)
	{
		KSPSetType(ksp,KSPBICG);
		if (_nameOfPc.compare("LU")==0) PCSetType(prec,PCLU);
	}
	else if (_nameOfMethod.compare("GCR")==0)
		KSPSetType(ksp,KSPGCR);
	else if (_nameOfMethod.compare("LSQR")==0)
		KSPSetType(ksp,KSPLSQR);
	else if (_nameOfMethod.compare("CHOLESKY")==0)
		PCSetType(prec,PCCHOLESKY);
	else if (_nameOfMethod.compare("LU")==0)
		PCSetType(prec,PCLU);
	else
	{
		string msg="Vector LinearSolver::solve( void ) : The method "+_nameOfMethod+" is not yet implemented.\n";
		msg+="The methods implemented are : GMRES, BICG, CG, CHOLESKY, LU, BCG, LGMRES, LSQR, CR, CGS and GCR.\n";
		msg+="The preconditioners implemented are : LU for GMRES and BICG methods.";
	    throw CdmathException(msg);
	}
	PetscInt its;
	PetscReal rtol,abstol,dtol;
	PetscInt maxits;

	Vec X;
	VecDuplicate(B,&X);

	if (isSingular())
	{
		MatNullSpace nullsp;
		MatNullSpaceCreate(PETSC_COMM_WORLD, PETSC_TRUE, 0, PETSC_NULL, &nullsp);
		KSPSetNullSpace(ksp, nullsp);
		MatNullSpaceDestroy(&nullsp);
	}
	KSPSolve(ksp,B,X);


	KSPGetResidualNorm(ksp,&rtol);

	KSPGetIterationNumber(ksp,&its);
	_numberOfIter=(int)its;
	_residu=(double)rtol;
	KSPConvergedReason reason;
	KSPGetConvergedReason(ksp,&reason);
	if (reason>=0 )
          _convergence=true;
        
	Vector X1=vecToVector(X);

	MatDestroy(&A);
	VecDestroy(&B);
	VecDestroy(&X);
	KSPDestroy(&ksp);
	PetscFree(a);

	return X1;
}

Vec
LinearSolver::vectorToVec(const Vector& vec) const
{
	PetscInitialize(0,(char ***)"", PETSC_NULL, PETSC_NULL);
    int numberOfRows=vec.getNumberOfRows();
	Vec X;

	VecCreate(PETSC_COMM_WORLD,&X);
	VecSetSizes(X,PETSC_DECIDE,numberOfRows);
	VecSetFromOptions(X);
	for (int i=0; i<numberOfRows; i++)
	{
		double value = vec(i);
		VecSetValues(X,1,&i,&value,ADD_VALUES);
	}

	VecAssemblyBegin(X);
	VecAssemblyEnd(X);

    return X;
}

Vector
LinearSolver::vecToVector(const Vec& vec) const
{
	PetscInitialize(0,(char ***)"", PETSC_NULL, PETSC_NULL);
	PetscInt numberOfRows;

	VecGetSize(vec,&numberOfRows);

	Vector X(numberOfRows);
	double value;

	for (int i=0; i<numberOfRows; i++)
	{
		VecGetValues(vec,1,&i,&value);
		X(i)=value;
	}

    return X;
}

//----------------------------------------------------------------------
const LinearSolver&
LinearSolver::operator= ( const LinearSolver& linearSolver )
//----------------------------------------------------------------------
{
	_tol=linearSolver.getTolerance();
	_numberMaxOfIter=linearSolver.getNumberMaxOfIter();
	_nameOfMethod=linearSolver.getNameOfMethod();
	_residu=linearSolver.getResidu();
	_convergence=linearSolver.getStatus();
	_numberOfIter=linearSolver.getNumberOfIter();
	_isSingular=linearSolver.isSingular();
	_matrix=linearSolver.getMatrix();
	_vector=linearSolver.getSndMember();
	_nameOfPc=linearSolver.getNameOfPc();
	return *this;
}
