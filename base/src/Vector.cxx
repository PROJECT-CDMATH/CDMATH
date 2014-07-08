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

double
Vector::operator* (const Vector& vector) const
{
	double res=0.;
	int dim=getNumberOfRows();
	for(int i=0; i<dim; i++)
	{
		res=res+Matrix::operator()(i,0)*vector(i);
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

Vector
operator+ (const Vector& vector1, const Vector& vector2)
{
  int numberOfRows = vector1.getNumberOfRows();
  Vector res(numberOfRows);
  for (int i=0; i<numberOfRows; i++)
	  res(i) = vector1(i) + vector2(i);
   return res;
}

Vector
operator- (const Vector& vector1, const Vector& vector2)
{
  int numberOfRows = vector1.getNumberOfRows();
  Vector res(numberOfRows);
  for (int i=0; i<numberOfRows; i++)
	  res(i) = vector1(i) - vector2(i);
   return res;
}


Vector
operator* (double value , const Vector& vector )
{
	int numberOfRows = vector.getNumberOfRows();
	Vector res(numberOfRows);
	for (int i=0; i<numberOfRows; i++)
		  res(i) = vector(i)*value;
   return res;
}

Vector
operator* (const Vector& vector, double value )
{
	int numberOfRows = vector.getNumberOfRows();
	Vector res(numberOfRows);
	for (int i=0; i<numberOfRows; i++)
		  res(i) = vector(i)*value;
   return res;
}

Vector
operator/ (const Vector& vector, double value)
{
	int numberOfRows = vector.getNumberOfRows();
	Vector res(numberOfRows);
	for (int i=0; i<numberOfRows; i++)
		  res(i) = vector(i)/value;
   return res;
}
