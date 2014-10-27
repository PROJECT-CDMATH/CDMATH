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

	~Vector ( void ) ;

	double& operator () ( int i ) ;

	double operator () ( int i ) const ;

	double& operator [] ( int i ) ;

	double operator [] ( int i ) const ;

	double norm() const ;

	double operator* (const Vector& vector) const ;

	friend Matrix operator^(const Vector& vector1, const Vector& vector2);

	friend Vector operator+ (const Vector& vector1, const Vector& vector2);

	friend Vector operator- (const Vector& vector1, const Vector& vector2);

	friend Vector operator* (double value , const Vector& vector ) ;

	friend Vector operator* (const Vector& vector, double value ) ;

	friend Vector operator/ (const Vector& vector, double value) ;

};

#endif /* VECTOR_HXX_ */
