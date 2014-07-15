/*
 * LinearSolver.hxx
 *
 *  Created on: 13 April. 2013
 *      Authors: CDMAT
 */

#ifndef LINEARSOLVER_HXX_
#define LINEARSOLVER_HXX_

#include <petsc.h>
#include "Matrix.hxx"
#include "Vector.hxx"

#include <string>

class LinearSolver
{
    public: //----------------------------------------------------------------

	LinearSolver ( void ) ;

	~LinearSolver ( void ) ;

	LinearSolver( const Matrix& matrix, const Vector& vector, int numberMaxOfIter, double tol, std::string method  );

	LinearSolver( const Matrix& matrix, const Vector& vector, int numberMaxOfIter, double tol, std::string method, std::string pc  );

	const LinearSolver& operator= ( const LinearSolver& linearSolver ) ;

	LinearSolver ( const LinearSolver& LS ) ;

	int getNumberOfIter( void ) const ;

	bool getStatus( void ) const ;

	double getResidu( void ) const ;

	double getTolerance( void ) const ;

	int getNumberMaxOfIter( void ) const ;

	void setTolerance(double tol) ;

	void setNumberMaxOfIter(int numberMaxOfIter) ;

	Matrix getMatrix( void ) const ;

	Vector getSndMember( void ) const ;

	std::string getNameOfMethod( void ) const ;

	std::string getNameOfPc( void ) const ;

	Vector solve( void ) ;

	Vec vectorToVec( const Vector& vector ) const ;

	Vector vecToVector(const Vec& vec) const ;

	void setMatrix(const Matrix& matrix) ;

	void setSndMember(const Vector& vector) ;

	void setSingularity(bool sing) ;

	bool isSingular( void ) const;

    private: //----------------------------------------------------------------

	void setLinearSolver(const Matrix& matrix, const Vector& vector) ;
	Mat getPetscMatrix() const ;
	Vec getPetscVector() const ;
	KSP getPetscKsp() const ;
	PC getPetscPc() const ;

	double _tol;
	int _numberMaxOfIter;
	std::string _nameOfMethod;
	std::string _nameOfPc;
	double _residu;
	bool _convergence;
	bool _isSingular;
	int _numberOfIter;
	Matrix _matrix;
	Vector _vector;
	Mat _mat;
	Vec _smb;
	PC _prec;
	KSP _ksp;

};

#endif /* LINEARSOLVER_HXX_ */

