/*
 * LinearSolver.cxx
 *
 *  Created on: 15 avr. 2013
 *      Author: mekkas
 */

#include <string>
#include <cmath>

#include <petscvec.h>
#include <petscmat.h>
#include <petscksp.h>

#include "LinearSolver.hxx"
#include "CdmathException.hxx"

#include "SparseMatrix.hxx"

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
    _mat=NULL;
    _smb=NULL;
    _ksp=NULL;
    _prec=NULL;
}

LinearSolver::~LinearSolver ( void )
{
    MatDestroy(&_mat);
    VecDestroy(&_smb);
    KSPDestroy(&_ksp);
}

void
LinearSolver::setTolerance(double tol)
{
    _tol=tol;
    KSPSetTolerances(_ksp,tol,PETSC_DEFAULT,PETSC_DEFAULT,getNumberMaxOfIter());
}

void
LinearSolver::setNumberMaxOfIter(int numberMaxOfIter)
{
    _numberMaxOfIter=numberMaxOfIter;
    KSPSetTolerances(_ksp,getTolerance(),PETSC_DEFAULT,PETSC_DEFAULT,numberMaxOfIter);
}

LinearSolver::LinearSolver( const GenericMatrix& matrix, const Vector& vector, int numberMaxOfIter, double tol, std::string nameOfMethod )
{
    _tol=tol;
    _nameOfMethod=nameOfMethod;
    _numberMaxOfIter=numberMaxOfIter;
    _residu=1.E30;
    _convergence=false;
    _numberOfIter=0;
    _isSingular=false;
    _nameOfPc="";
    setLinearSolver(matrix,vector);
}

LinearSolver::LinearSolver( const GenericMatrix& matrix, const Vector& vector, int numberMaxOfIter, double tol, std::string nameOfMethod, std::string pc )
{
    _tol=tol;
    _nameOfMethod=nameOfMethod;
    _nameOfPc=pc;
    _numberMaxOfIter=numberMaxOfIter;
    _residu=1.E30;
    _convergence=false;
    _numberOfIter=0;
    _isSingular=false;

    if ((_nameOfPc.compare("LU")==0 && (_nameOfMethod.compare("GMRES")!=0 && _nameOfMethod.compare("BICG")!=0)))
    {
        string msg="LinearSolver::LinearSolver : preconditioner "+_nameOfPc+" is not yet implemented.\n";
        msg+="The preconditioners implemented are : LU for GMRES and BICG methods.";
        throw CdmathException(msg);
    }

    if (_nameOfPc.compare("ILU")==0 && matrix.isSparseMatrix()==false)
    {
        string msg="LinearSolver::LinearSolver : preconditioner "+_nameOfPc+" is not compatible with dense matrix.\n";
        throw CdmathException(msg);
    }

    if (_nameOfPc.compare("ILU")==0 && (_nameOfMethod.compare("LU")==0 || _nameOfMethod.compare("CHOLESKY")==0) )
    {
        string msg="LinearSolver::LinearSolver : preconditioner "+_nameOfPc+" is not compatible with "+_nameOfMethod+".\n";
        throw CdmathException(msg);
    }

    setLinearSolver(matrix,vector);
}

void
LinearSolver::setLinearSolver(const GenericMatrix& matrix, const Vector& vector)
{
    PetscInitialize(0,(char ***)"", PETSC_NULL, PETSC_NULL);
    setMatrix(matrix);
    setSndMember(vector);

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
LinearSolver::setMatrix(const GenericMatrix& matrix)
{
    /* matrix to mat */
    int numberOfRows=matrix.getNumberOfRows();
    int numberOfColumns=matrix.getNumberOfColumns();

    if (matrix.isSparseMatrix())
    {
        const SparseMatrix& Smat = dynamic_cast<const SparseMatrix&>(matrix);
//        int numberOfNonZeros=Smat.getNumberOfNonZeros();
    PetscInt    nnz[numberOfRows];
        IntTab iRows=Smat.getIndexRows();
    IntTab iColumns=Smat.getIndexColumns();
        for (int i=0;i<numberOfRows;i++)
            nnz[i]=iRows[i+1]-iRows[i];
        MatCreateSeqAIJ(MPI_COMM_SELF,numberOfRows,numberOfColumns,PETSC_DEFAULT,nnz,&_mat);
        DoubleTab values=Smat.getValues();
        for (int i=0;i<numberOfRows;i++)
        {
            PetscInt    cols[nnz[i]];
            PetscScalar    vals[nnz[i]];
            for (int j=0;j<nnz[i];j++)
            {
                cols[j]=iColumns[iRows[i]+j]-1;
                vals[j]=values[iRows[i]+j];
            }
            MatSetValues(_mat,1,
                        &i,
                        nnz[i],
                        cols,
                        vals,INSERT_VALUES);
        }
    } else
    {
        MatCreate(PETSC_COMM_WORLD, &_mat);
        MatSetSizes(_mat, PETSC_DECIDE, PETSC_DECIDE, numberOfRows, numberOfColumns);
        MatSetType(_mat,MATSEQDENSE);

        PetscScalar *a;
        PetscMalloc(numberOfRows*numberOfColumns*sizeof(PetscScalar),&a);
        for (int i=0;i<numberOfRows;i++)
            for (int j=0;j<numberOfColumns;j++)
                a[i+j*numberOfRows]=matrix(i,j);

        MatSeqDenseSetPreallocation(_mat,a);
    }
    //Assemblage final
    MatAssemblyBegin(_mat, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(_mat, MAT_FINAL_ASSEMBLY);

    KSPCreate(PETSC_COMM_WORLD, &_ksp);
    KSPSetOperators(_ksp,_mat,_mat,SAME_NONZERO_PATTERN);
    // If Petsc is 3.5 or higher:
    //KSPSetOperators(_ksp,_mat,_mat);

    KSPGetPC(_ksp,&_prec);
}

void
LinearSolver::setSndMember(const Vector& vector)
{
    _vector=vector;
    _smb=vectorToVec(vector);

}

void
LinearSolver::setSingularity(bool sing)
{
    _isSingular=sing;
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
    _vector=LS.getSndMember();
    _residu=LS.getResidu();
    _convergence=LS.getStatus();
    _numberOfIter=LS.getNumberOfIter();
    _isSingular=LS.isSingular();
    _nameOfPc=LS.getNameOfPc();
    _mat=LS.getPetscMatrix();
    _smb=LS.getPetscVector();
    _ksp=LS.getPetscKsp();
    _prec=LS.getPetscPc();
}

KSP
LinearSolver::getPetscKsp() const
{
    return _ksp;
}

Mat
LinearSolver::getPetscMatrix() const
{
    return _mat;
}

Vec
LinearSolver::getPetscVector() const
{
    return _smb;
}

PC
LinearSolver::getPetscPc() const
{
    return _prec;
}

Vector
LinearSolver::solve( void )
{

    PetscInitialize(0,(char ***)"", PETSC_NULL, PETSC_NULL);



    if (_nameOfMethod.compare("GMRES")==0)
    {
       KSPSetType(_ksp,KSPGMRES);
       if (_nameOfPc.compare("LU")==0) PCSetType(_prec,PCLU);
    }
    else if (_nameOfMethod.compare("LGMRES")==0)
        KSPSetType(_ksp,KSPLGMRES);
    else if (_nameOfMethod.compare("CG")==0)
        KSPSetType(_ksp,KSPCG);
    else if (_nameOfMethod.compare("BCG")==0)
        KSPSetType(_ksp,KSPBCGS);
    else if (_nameOfMethod.compare("CR")==0)
        KSPSetType(_ksp,KSPCR);
    else if (_nameOfMethod.compare("CGS")==0)
        KSPSetType(_ksp,KSPCGS);
    else if (_nameOfMethod.compare("BICG")==0)
    {
        KSPSetType(_ksp,KSPBICG);
        if (_nameOfPc.compare("LU")==0) PCSetType(_prec,PCLU);
    }
    else if (_nameOfMethod.compare("GCR")==0)
        KSPSetType(_ksp,KSPGCR);
    else if (_nameOfMethod.compare("LSQR")==0)
        KSPSetType(_ksp,KSPLSQR);
    else if (_nameOfMethod.compare("CHOLESKY")==0)
        PCSetType(_prec,PCCHOLESKY);
    else if (_nameOfMethod.compare("LU")==0)
        PCSetType(_prec,PCLU);
    else
    {
        string msg="Vector LinearSolver::solve( void ) : The method "+_nameOfMethod+" is not yet implemented.\n";
        msg+="The methods implemented are : GMRES, BICG, CG, CHOLESKY, LU, BCG, LGMRES, LSQR, CR, CGS and GCR.\n";
        msg+="The preconditioners implemented are : LU for GMRES and BICG methods.";
        throw CdmathException(msg);
    }

   if (_nameOfPc.compare("ILU")==0 && (_nameOfMethod.compare("LU")!=0 && _nameOfMethod.compare("CHOLESKY")!=0) ) PCSetType(_prec,PCILU);

    PetscInt its;
    PetscReal rtol,abstol,dtol;
    PetscInt maxits;

    Vec X;
    VecDuplicate(_smb,&X);

    if (isSingular())
    {
        MatNullSpace nullsp;
        MatNullSpaceCreate(PETSC_COMM_WORLD, PETSC_TRUE, 0, PETSC_NULL, &nullsp);
        KSPSetNullSpace(_ksp, nullsp);
        MatNullSpaceDestroy(&nullsp);
    }
    KSPSolve(_ksp,_smb,X);


    KSPGetResidualNorm(_ksp,&rtol);

    KSPGetIterationNumber(_ksp,&its);
    _numberOfIter=(int)its;
    _residu=(double)rtol;
    KSPConvergedReason reason;
    KSPGetConvergedReason(_ksp,&reason);
    if (reason>=0 )
          _convergence=true;

    Vector X1=vecToVector(X);

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
    for (PetscInt i=0; i<numberOfRows; i++)
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

    for (PetscInt i=0; i<numberOfRows; i++)
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
    _vector=linearSolver.getSndMember();
    _nameOfPc=linearSolver.getNameOfPc();
    _mat=linearSolver.getPetscMatrix();
    _smb=linearSolver.getPetscVector();
    _ksp=linearSolver.getPetscKsp();
    _prec=linearSolver.getPetscPc();
    return *this;
}
