/*
 * LinearSolver.hxx
 *
 *  Created on: 15 avr. 2013
 *      Author: CDMATH
 */

#ifndef LINEARSOLVER_HXX_
#define LINEARSOLVER_HXX_


/**
 * LinearSolver class is defined by
 * -
 * -
 */

#include "Matrix.hxx"
#include "Vector.hxx"
#include <iostream>

class LinearSolver
{
    public: //----------------------------------------------------------------
	/**
	 * default constructor
	 */
	LinearSolver ( void ) ;

	LinearSolver ( const Matrix& matrix, const Vector& vector, int numberMaxOfIter, double tol, std::string name ) ;

	LinearSolver ( const LinearSolver& LS ) ;

	double getTolerance( void ) const ;

	int getNumberMaxOfIter( void ) const ;

	int getNumberOfIter( void ) const ;

	bool getStatus( void ) const ;

	double getResidu( void ) const ;

	Matrix getMatrix( void ) const ;

	Vector getSndMember( void ) const ;

	std::string getName(void) const ;

	Vector solve( void ) ;

    private: //----------------------------------------------------------------

	Vector conjugateGradient( void );

	Vector Bicgstab( void );

	Matrix _matrix;

	Vector _vector;

	int _numberMaxOfIter;

	double _tol;

	bool _convergence;

	int _numberOfIter;

	double _residu;

	std::string _name;

};


#endif /* LINEARSOLVER_HXX_ */
