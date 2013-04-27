/*
 * Vector.hxx
 *
 *  Created on: 15 avr. 2013
 *      Author: CDMATH
 */

#ifndef VECTOR_HXX_
#define VECTOR_HXX_

#include "Matrix.hxx"

class Vector: public Matrix
{
    public: //----------------------------------------------------------------

	Vector ( void ) ;

	Vector( int numberOfRows ) ;

	const Matrix& operator= ( const Matrix& matrix ) ;

	~Vector ( void ) ;

	double& operator [] ( int i ) ;

	const double& operator [] ( int i ) const ;

	double& operator () ( int i ) ;

	const double& operator () ( int i ) const ;

	double norm() const ;

	double operator* (const Vector& vector) const ;


};

#endif /* VECTOR_HXX_ */
