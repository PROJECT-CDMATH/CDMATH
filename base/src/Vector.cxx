#include "Vector.hxx"

#include <cmath>

//----------------------------------------------------------------------
Vector::Vector( void )
//----------------------------------------------------------------------
{

}

//----------------------------------------------------------------------
Vector::~Vector( void )
//----------------------------------------------------------------------
{

}

//----------------------------------------------------------------------
Vector::Vector( int numberOfRows ):
Matrix(numberOfRows,1)
//----------------------------------------------------------------------
{

}

//----------------------------------------------------------------------
const Matrix&
Vector::operator= ( const Matrix& matrix )
//----------------------------------------------------------------------
{
    return Matrix::operator=(matrix);
}

double&
Vector::operator[](int i)
{
	return Matrix::operator()(i,0);
}
const double&
Vector::operator[](int i) const
{
	return Matrix::operator()(i,0);
}

double&
Vector::operator()(int i)
{
	return Matrix::operator()(i,0);
}
const double&
Vector::operator()(int i) const
{
	return Matrix::operator()(i,0);
}

double
Vector::operator* (const Vector& vector) const
{
	double res=0.;
	int dim=getNumberOfRows();
	for(int i=0; i<dim; i++)
	{
		res=res+Matrix::operator()(i,0)*vector[i];
	}
	return res;
}

double Vector::norm() const
{
	double norm = 0.0;
	int dim=getNumberOfRows();
	for(int i=0; i<dim; i++)
		norm += Matrix::operator()(i,0)*Matrix::operator()(i,0);
	return sqrt(norm);
}
