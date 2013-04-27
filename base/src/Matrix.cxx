/*
 * Matrix.cxx
 *
 *  Created on: 13 April. 2013
 *      Authors: CDMATH
 */

#include "Matrix.hxx"
#include "Vector.hxx"

using namespace std;

Matrix::Matrix()
{
	_numberOfRows = 0;
	_numberOfColumns = 0;
	_values=NULL;
}

Matrix::Matrix(int dim)
{
	_numberOfRows = dim;
	_numberOfColumns = dim;
    _values = new DoubleTab[dim];
	for(int i=0; i<dim; i++)
		_values[i] = DoubleTab(dim,0.);
}

Matrix::Matrix(int numberOfRows, int numberOfColumns)
{
	_numberOfRows = numberOfRows;
	_numberOfColumns = numberOfColumns;
    _values = new DoubleTab[numberOfRows];
	for(int i=0; i<numberOfRows; i++)
		_values[i] = DoubleTab(numberOfColumns,0.);
}

Matrix::~Matrix()
{
	for(int i=0; i<_numberOfRows; i++)
		_values[i].DoubleTab::~DoubleTab();
}

Matrix::Matrix(const Matrix& matrix)
{
	_numberOfRows = matrix.getNumberOfRows();
	_numberOfColumns = matrix.getNumberOfColumns();

	_values = new DoubleTab[_numberOfRows];
	for(int i=0; i<_numberOfRows; i++)
		_values[i] = matrix[i];
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

DoubleTab&
Matrix::operator[](int i)
{
	return _values[i];
}
const DoubleTab&
Matrix::operator[](int i) const
{
	return _values[i];
}

DoubleTab&
Matrix::operator()(int i)
{
	return _values[i];
}
const DoubleTab&
Matrix::operator()(int i) const
{
	return _values[i];
}

double&
Matrix::operator()(int i, int j)
{
	return _values[i][j];
}
const double&
Matrix::operator()(int i, int j) const
{
	return _values[i][j];
}

Matrix&
Matrix::operator = (const Matrix& matrix)
{
	this->Matrix::~Matrix();
	_numberOfRows = matrix.getNumberOfRows();
	_numberOfColumns = matrix.getNumberOfColumns();

	_values = new DoubleTab[_numberOfRows];
	for(int i=0; i<_numberOfRows; i++)
		_values[i] = matrix[i];
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
			cout<<(*this)[i][j];
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
			sum=sum+_values[i][j]*vector[j];
		res[i] = sum;
	}
	return res;
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
		throw "Matrix does not symmetric!!!";

	bool res = true;

	int dim = _numberOfRows;

	for(int i=0; i<dim-1; i++)
		for(int j=i+1; j<dim; j++)
			if(_values[i][j] != _values[j][i])
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
			res[j][i] = _values[i][j];
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
            	res[r][c++] = _values[i][j];
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
	{
		Matrix matrix=getMatrixU();
		double res=1.;
		for (int i=0;i<matrix.getNumberOfRows();i++)
			res*=matrix(i,i);
		return res;
	}else
	{
		Matrix matrix;

	   double res = 0.0;
	   int dim = _numberOfRows;
	   if(dim==1)
		  return _values[0][0];

	   for(int i=0; i<dim; i++)
	   {
		   matrix = this->partMatrix(i,0);
		   res += ( coefficient(i)*this->_values[i][0]*(matrix.determinant() ) );
	   }
	   return res;
	}
}

Matrix
Matrix::getMatrixU(void) const
{
	int dim=_numberOfRows;
	Matrix U(dim);
	for (int j=0; j<dim-1; j++)
	{
		for (int i=j+1; i<dim; i++)
		{
			U[i][j] = U[i][j] / U[j][j];
			for (int k=j+1; k<dim; k++)
			{
				U[i][k] += -U[i][j] * U[j][k];
			}
			U[i][j] = 0;
		}
	}
	return U;
}

Matrix
Matrix::getMatrixL(void) const
{
	int dim=_numberOfRows;
	Matrix L(dim);
	int j;
    for (j=0; j<dim-1; j++)
    {
    	int i;
    	for (i=j+1; i<dim; i++)
    	{
    		L[i][j] = L[i][j] / L[j][j];
			for (int k=j+1; k<dim; k++)
				L[i][k] += -L[i][j] * L[j][k];
    	}
    	for ( i=j+1; i<dim; i++)
    		L[j][i] = 0;
    	L[j][j] = 1;
    }
	L[j][j] = 1;
	return L;
}

Matrix
operator+ (const Matrix& matrix1, const Matrix& matrix2)
{
  int numberOfRows = matrix1.getNumberOfRows();
  int numberOfColumns = matrix1.getNumberOfColumns();
  Matrix res(numberOfRows, numberOfColumns);
  for (int i=0; i<numberOfRows; i++)
	  res[i] = matrix1[i] + matrix2[i];
   return res;
}

Matrix
operator- (const Matrix& matrix1, const Matrix& matrix2)
{
  int numberOfRows = matrix1.getNumberOfRows();
  int numberOfColumns = matrix1.getNumberOfColumns();
  Matrix res(numberOfRows, numberOfColumns);
  for (int i=0; i<numberOfRows; i++)
	  res[i] = matrix1[i] - matrix2[i];
   return res;
}


Matrix operator*(const Matrix& M, const Matrix& N)
{
   int rows = M.getNumberOfRows();
   int columns = M.getNumberOfColumns();
   Matrix res(rows,columns);

   Matrix L = N.transpose();

   for(int i=0; i<rows; i++)
	   for( int j=0; j<columns; j++)
		   res[i][j] = M[i] * L[j];
   return res;
}

Matrix
operator* (double value , const Matrix& matrix )
{
	int numberOfRows = matrix.getNumberOfRows();
	int numberOfColumns = matrix.getNumberOfColumns();
	Matrix res(numberOfRows, numberOfColumns);
	for (int i=0; i<numberOfRows; i++)
		  res[i] = matrix[i]*value;
   return res;
}

Matrix
operator* (const Matrix& matrix, double value )
{
	int numberOfRows = matrix.getNumberOfRows();
	int numberOfColumns = matrix.getNumberOfColumns();
	Matrix res(numberOfRows, numberOfColumns);
	for (int i=0; i<numberOfRows; i++)
		  res[i] = matrix[i]*value;
   return res;
}

Matrix
operator/ (const Matrix& matrix, double value)
{
	int numberOfRows = matrix.getNumberOfRows();
	Matrix res = matrix;
	for(int i=0; i<numberOfRows; i++)
		res[i] = res[i] / value;
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
			out<<"\t"<<matrix[i][j];
		}
		out<<endl;
	}
	return out;
}
