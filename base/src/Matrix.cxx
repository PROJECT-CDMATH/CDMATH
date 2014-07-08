/*
 * Matrix.cxx
 *
 *  Created on: 13 April. 2013
 *      Authors: CDMATH
 */

#include "Matrix.hxx"
#include "Vector.hxx"

#include "MEDCouplingMatrix.hxx"

using namespace ParaMEDMEM;
using namespace std;

Matrix::Matrix()
{
	_numberOfRows = 0;
	_numberOfColumns = 0;
	_mat=NULL;
}

Matrix::Matrix(int dim)
{
	_numberOfRows = dim;
	_numberOfColumns = dim;
	_mat=DenseMatrix::New(dim,dim);
}

Matrix::Matrix(int numberOfRows, int numberOfColumns)
{
	_numberOfRows = numberOfRows;
	_numberOfColumns = numberOfColumns;
	_mat=DenseMatrix::New(numberOfRows,numberOfColumns);
}

Matrix::~Matrix()
{
	_numberOfRows = 0;
	_numberOfColumns = 0;
	if (_mat)
		_mat->decrRef();
}

Matrix::Matrix(const Matrix& matrix)
{
	_numberOfRows = matrix.getNumberOfRows();
	_numberOfColumns = matrix.getNumberOfColumns();
    _mat=static_cast<DenseMatrix *>(matrix.getDenseMatrix()->deepCpy());
}

//----------------------------------------------------------------------
DenseMatrix*
Matrix::getDenseMatrix ( void )  const
//----------------------------------------------------------------------
{
    return _mat ;
}

int
Matrix::getNumberOfRows() const
{
	return _numberOfRows ;
}

int
Matrix::getNumberOfColumns() const
{
	return _numberOfColumns ;
}

double&
Matrix::operator()(int i, int j)
{
	return _mat->getData()->getPointer()[i+_numberOfRows*j];
}

const double&
Matrix::operator()(int i, int j) const
{
	return _mat->getData()->getPointer()[i+_numberOfRows*j];
}

void
Matrix::setDensiteMatrix (const DenseMatrix* mat)
{
    _mat=static_cast<DenseMatrix *>(mat->deepCpy());

}

Matrix&
Matrix::operator = (const Matrix& matrix)
{
	this->Matrix::~Matrix();
	_numberOfRows = matrix.getNumberOfRows();
	_numberOfColumns = matrix.getNumberOfColumns();

	_mat=DenseMatrix::New(_numberOfRows,_numberOfColumns);
    _mat=static_cast<DenseMatrix *>(matrix.getDenseMatrix()->deepCpy());

    return (*this);
}

void
Matrix::view() const
{
	for (int i=0; i<_numberOfRows;i++)
	{
		for (int j=0;j<_numberOfColumns; j++)
		{
			cout.width(6);
			cout.precision(6);
			cout<<(*this)(i,j);
		}
	cout<<endl;
	}
}

Vector
Matrix::operator* (const Vector& vector) const
{
	Vector res(_numberOfRows);
	for(int i=0; i<_numberOfRows; i++)
	{
		double sum=0.;
		for(int j=0; j<_numberOfColumns; j++)
			sum=sum+(*this)(i,j)*vector(j);
		res(i) = sum;
	}
	return res;
}

Matrix&
Matrix::operator*= (const Matrix& matrix)
{
	(*this)=(*this)*matrix;
	return (*this);
}

bool
Matrix::isSquare() const
{
	if(_numberOfRows == _numberOfColumns)
		return true;
	return false;
}

bool
Matrix::isSymmetric() const
{
	if( ! isSquare() )
		throw "isSymmetric::Matrix is not square!!!";

	bool res = true;

	int dim = _numberOfRows;

	for(int i=0; i<dim-1; i++)
		for(int j=i+1; j<dim; j++)
			if((*this)(i,j) != (*this)(j,i))
			{
				res = false;
				break;
			}
	return res;
}

Matrix
Matrix::transpose() const
{
	Matrix res(_numberOfColumns, _numberOfRows);
	for(int i=0; i<_numberOfRows; i++)
		for(int j=0; j<_numberOfColumns; j++)
			res(j,i) = _mat->getData()->getPointer()[i+_numberOfRows*j];
   return res;
}

Matrix
Matrix::partMatrix(int row, int column) const
{
   int r = 0;
   int c = 0;
   Matrix res(_numberOfRows-1, _numberOfColumns-1);

   for (int i=0; i<_numberOfRows; i++)
   {
      c = 0;
      if(i != row)
      {
         for(int j=0; j<_numberOfColumns; j++)
            if(j != column)
            	res(r,c++) = _mat->getData()->getPointer()[i+_numberOfRows*j];
         r++;
      }
   }
   return res;
}

int
Matrix::coefficient(int index) const
{
	if(! (index % 2) )
		return (1);
	return (-1);
}

double
Matrix::determinant() const
{
	if( ! isSquare() )
		throw "isSymmetric::Matrix is not square!!!";
	else
	{
		Matrix matrix;

	   double res = 0.0;
	   int dim = _numberOfRows;
	   if(dim==1)
		  return (*this)(0,0);

	   for(int i=0; i<dim; i++)
	   {
		   matrix = this->partMatrix(i,0);
		   res += ( coefficient(i)*(*this)(i,0)*(matrix.determinant() ) );
	   }
	   return res;
	}
}

Matrix
operator+ (const Matrix& matrix1, const Matrix& matrix2)
{
  int numberOfRows = matrix1.getNumberOfRows();
  int numberOfColumns = matrix1.getNumberOfColumns();
  Matrix res(numberOfRows, numberOfColumns);
  res.setDensiteMatrix(matrix1.getDenseMatrix()->Add(matrix1.getDenseMatrix(),matrix2.getDenseMatrix()));

   return res;
}

Matrix
operator- (const Matrix& matrix1, const Matrix& matrix2)
{
  int numberOfRows = matrix1.getNumberOfRows();
  int numberOfColumns = matrix1.getNumberOfColumns();
  Matrix res(numberOfRows, numberOfColumns);
  res.setDensiteMatrix(matrix1.getDenseMatrix()->Substract(matrix1.getDenseMatrix(),matrix2.getDenseMatrix()));
   return res;
}

Matrix
operator*(const Matrix& matrix1, const Matrix& matrix2)
{
	int rows = matrix1.getNumberOfRows();
	int columns = matrix2.getNumberOfColumns();
	Matrix res(rows,columns);
	for (int i=0; i<rows; i++)
	{
		for (int j=0; j<columns; j++)
		{
			double sum=0.;
			for (int k=0; k<columns; k++)
				sum+=matrix1(i,k)*matrix2(k,j);
			res(i,j)=sum;
		}
	}
   return res;
}

Matrix
operator* (double value , const Matrix& matrix )
{
	int numberOfRows = matrix.getNumberOfRows();
	int numberOfColumns = matrix.getNumberOfColumns();
	Matrix res(numberOfRows, numberOfColumns);
    for (int i=0; i<numberOfRows; i++)
    	for (int j=0; j<numberOfColumns; j++)
    		res(i,j)=value*matrix(i,j);
	return res;
}

Matrix
operator* (const Matrix& matrix, double value )
{
	int numberOfRows = matrix.getNumberOfRows();
	int numberOfColumns = matrix.getNumberOfColumns();
	Matrix res(numberOfRows, numberOfColumns);
    for (int i=0; i<numberOfRows; i++)
    	for (int j=0; j<numberOfColumns; j++)
    		res(i,j)=value*matrix(i,j);
	return res;
}

Matrix
operator/ (const Matrix& matrix, double value)
{
	int numberOfRows = matrix.getNumberOfRows();
	int numberOfColumns = matrix.getNumberOfColumns();
	Matrix res(numberOfRows, numberOfColumns);
    for (int i=0; i<numberOfRows; i++)
    	for (int j=0; j<numberOfColumns; j++)
    		res(i,j)=matrix(i,j)/value;
	return res;
}

Matrix&
Matrix::operator+= (const Matrix& matrix)
{
	(*this)=(*this)+matrix;
	return (*this);
}

Matrix&
Matrix::operator-= (const Matrix& matrix)
{
	(*this)=(*this)-matrix;
	return (*this);
}

Matrix&
Matrix::operator*= (double value)
{
	(*this)=(*this)*value;
	return (*this);
}

Matrix&
Matrix::operator/= (double value)
{
	(*this)=(*this)/value;
	return (*this);
}

ostream&
operator<<(ostream& out, const Matrix& matrix)
{
	for (int i=0; i<matrix.getNumberOfRows();i++)
	{
		for (int j=0;j<matrix.getNumberOfColumns(); j++)
		{
			out.width(6);
			out.precision(6);
			out<<"\t"<<matrix(i,j);
		}
		out<<endl;
	}
	return out;
}
