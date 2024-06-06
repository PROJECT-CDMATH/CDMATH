/*
 * Matrix.cpp
 *
 *  Created on: 24 oct. 2014
 *      Author: amekkas
 */

#include "SparseMatrix.hxx"
#include "CdmathException.hxx"

#include <cstring>

using namespace std;

//----------------------------------------------------------------------
SparseMatrix::SparseMatrix()
//----------------------------------------------------------------------
{
	_effectNumberOfNonZeros=0;
	_numberOfColumns=0;
	_numberOfRows=0;
	_numberOfNonZeros=0;
	_flagNNZ=true;
	_blocNNZ=0.;
	_isSparseMatrix=true;
}

//----------------------------------------------------------------------
SparseMatrix::SparseMatrix( int numberOfRows, int numberOfColumns):_blocNNZ(10000)
//----------------------------------------------------------------------
{
	_flagNNZ=false;
	_effectNumberOfNonZeros=0;
	_numberOfRows = numberOfRows;
	_numberOfColumns=numberOfColumns;
	_numberOfNonZeros=0;
	_indexRows = IntTab(_numberOfRows+1,0);
	_indexColumns = IntTab(_blocNNZ,0);
	_values = DoubleTab(_blocNNZ,0.);
	_isSparseMatrix=true;
}

//----------------------------------------------------------------------
SparseMatrix::SparseMatrix( int numberOfRows, int numberOfColumns, int nnz )
//----------------------------------------------------------------------
{
	_effectNumberOfNonZeros=0;
	_flagNNZ=true;
	_blocNNZ=nnz;
	_numberOfRows = numberOfRows;
	_numberOfColumns=numberOfColumns;
	_numberOfNonZeros=nnz;
	_indexRows = IntTab(_numberOfRows+1,0);
	_indexColumns = IntTab(nnz,0);
	_values = DoubleTab(nnz,0.);
	_isSparseMatrix=true;
}

SparseMatrix
SparseMatrix::transpose() const
{
	SparseMatrix res(_numberOfColumns, _numberOfRows,_numberOfNonZeros);
	for(int i=0; i<_numberOfRows; i++)
		for(int j=0; j<_numberOfColumns; j++)
			res.setValue(i,j,(*this)(j,i));
   return (res);
}

int
SparseMatrix::getNumberOfNonZeros ( void ) const
{
	return (_numberOfNonZeros);
}

void
SparseMatrix::setValue( int i, int j, double value )
{
	double Val1 = (*this)(i,j);
	if(Val1 == value)
		return ;
	if(Val1 == 0.)
		insertValue(i,j,value);
	else
		replaceValue(i,j,value);
}

void
SparseMatrix::replaceValue(int i,int j,double value)
{

	double pos = _indexRows[i];
	double nbreElts = _indexRows[i+1] - _indexRows[i];
	int k=0;
	for(k=0;k<nbreElts;k++)
		if(_indexColumns[pos+k] >= j+1)
			break;
	double ind = pos+k;
	_values[ind] = value;
}

//----------------------------------------------------------------------
void
SparseMatrix::insertValue( int i, int j, double value )
//----------------------------------------------------------------------
{
	if(i>=_numberOfRows || j>=_numberOfColumns || i<0 || j<0)
	{
		string msg="double SparseMatrix::operator()(int i, int j) : i>number of rows or j>number of columns !";
	    throw CdmathException(msg);
	}

	if(!_flagNNZ)
	{
		_numberOfNonZeros+=1;
		if(_numberOfNonZeros>=_blocNNZ)
		{
			_values.resize(_blocNNZ);
			_indexColumns.resize(_blocNNZ);
		}
	}
	int pos = _indexRows[i];
	int NbElts = _indexRows[i+1] - _indexRows[i];
	int k;
	for(k=0;k<NbElts;k++)
	{
		if(_indexColumns[pos+k] >= j+1)
			break;
	}

	for(int l=i;l<_numberOfRows;l++)
		_indexRows[l+1]++;
	int index = pos+k;

	if(_effectNumberOfNonZeros-index>0)
	{
		memmove(&(_indexColumns.getPointer()[index+1]),&(_indexColumns.getPointer()[index]),(_effectNumberOfNonZeros-index)*sizeof(int));
		memmove(&(_values.getPointer()[index+1]),&(_values.getPointer()[index]),(_effectNumberOfNonZeros-index)*sizeof(double));
	}

	_indexColumns[index] = j+1;
	_values[index]=value;
	_effectNumberOfNonZeros++;
}

//----------------------------------------------------------------------
double
SparseMatrix::operator()( int i, int j ) const
//----------------------------------------------------------------------
{
	if(i>=_numberOfRows || j>=_numberOfColumns || i<0 || j<0)
	{
		string msg="double SparseMatrix::operator()(int i, int j) : i>number of rows or j>number of columns !";
	    throw CdmathException(msg);
	}
	int nbreElts = _indexRows[i+1] - _indexRows[i];
	int pos = _indexRows[i];
	double res=0.;
	for(int k=0;k<nbreElts;k++)
		if(_indexColumns[pos+k] == j+1)
			res=_values[pos+k];
	return (res);
}

//----------------------------------------------------------------------
SparseMatrix::~SparseMatrix()
//----------------------------------------------------------------------
{
}

//----------------------------------------------------------------------
IntTab
SparseMatrix::getIndexRows()
//----------------------------------------------------------------------
{
	return (_indexRows);
}

//----------------------------------------------------------------------
const IntTab&
SparseMatrix::getIndexRows() const
//----------------------------------------------------------------------
{
	return (_indexRows);
}

//----------------------------------------------------------------------
IntTab
SparseMatrix::getIndexColumns()
//----------------------------------------------------------------------
{
	return (_indexColumns);
}

//----------------------------------------------------------------------
const IntTab&
SparseMatrix::getIndexColumns() const
//----------------------------------------------------------------------
{
	return (_indexColumns);
}

void
SparseMatrix::setBlocNNZ(int blocNNZ)
{
	_blocNNZ=blocNNZ;
}

int
SparseMatrix::getBlocNNZ() const
{
	return (_blocNNZ);
}

bool
SparseMatrix::getFlagNNZ() const
{
	return (_flagNNZ);
}

//----------------------------------------------------------------------
SparseMatrix::SparseMatrix(const SparseMatrix& matrix)
//----------------------------------------------------------------------
{
	_numberOfRows = matrix.getNumberOfRows();
	_numberOfColumns = matrix.getNumberOfColumns();
	_numberOfNonZeros = matrix.getNumberOfNonZeros();
	_blocNNZ=matrix.getBlocNNZ();
	_flagNNZ=matrix.getFlagNNZ();
	_indexRows = matrix.getIndexRows();
	_indexColumns = matrix.getIndexColumns();
	_values = matrix.getValues();
	_isSparseMatrix=matrix.isSparseMatrix();
	_effectNumberOfNonZeros=matrix._effectNumberOfNonZeros;
}

//----------------------------------------------------------------------
const SparseMatrix&
SparseMatrix::operator= ( const SparseMatrix& matrix )
//----------------------------------------------------------------------
{
	_numberOfRows = matrix.getNumberOfRows();
	_numberOfColumns = matrix.getNumberOfColumns();
	_numberOfNonZeros = matrix.getNumberOfNonZeros();
	_blocNNZ=matrix.getBlocNNZ();
	_flagNNZ=matrix.getFlagNNZ();
	_indexRows = matrix.getIndexRows();
	_indexColumns = matrix.getIndexColumns();
	_values = matrix.getValues();
	_isSparseMatrix=matrix.isSparseMatrix();
	_effectNumberOfNonZeros=matrix._effectNumberOfNonZeros;
    return (*this);
}

SparseMatrix
operator+ (const SparseMatrix& matrix1, const SparseMatrix& matrix2)
{
	  int numberOfRows = matrix1.getNumberOfRows();
	  int numberOfColumns = matrix1.getNumberOfColumns();
	  int numberOfRows2 = matrix2.getNumberOfRows();
	  int numberOfColumns2 = matrix2.getNumberOfColumns();

	  if(numberOfRows2!=numberOfRows || numberOfColumns2!=numberOfColumns)
	  {
			string msg="Matrix Matrix::operator()+(const Matrix& matrix1, const Matrix& matrix2): number of rows or columns of the matrices is different!";
		    throw CdmathException(msg);
	  }
	  SparseMatrix res(numberOfRows, numberOfColumns);
	  for(int i=0;i<numberOfRows;i++)
		  for(int j=0;j<numberOfColumns;j++)
			  res.setValue(i,j,matrix1(i,j)+matrix2(i,j));
	  return (res);
}

SparseMatrix
operator- (const SparseMatrix& matrix1, const SparseMatrix& matrix2)
{
	  int numberOfRows = matrix1.getNumberOfRows();
	  int numberOfColumns = matrix1.getNumberOfColumns();
	  int numberOfRows2 = matrix2.getNumberOfRows();
	  int numberOfColumns2 = matrix2.getNumberOfColumns();

	  if(numberOfRows2!=numberOfRows || numberOfColumns2!=numberOfColumns)
	  {
			string msg="Matrix Matrix::operator()-(const Matrix& matrix1, const Matrix& matrix2): number of rows or columns of the matrices is different!";
		    throw CdmathException(msg);
	  }
	  SparseMatrix res(numberOfRows, numberOfColumns);
	  for(int i=0;i<numberOfRows;i++)
		  for(int j=0;j<numberOfColumns;j++)
			  res.setValue(i,j,matrix1(i,j)-matrix2(i,j));
	  return (res);
}

SparseMatrix
operator* (double value , const SparseMatrix& matrix )
{
	  SparseMatrix res(matrix);
	  DoubleTab t1=res.getValues();
	  t1*=value;
	  res.setValues(t1);
	  return (res);
}

SparseMatrix
operator* (const SparseMatrix& matrix, double value )
{
	  SparseMatrix res(matrix);
	  DoubleTab t1=res.getValues();
	  t1*=value;
	  res.setValues(t1);
	  return (res);
}

SparseMatrix
operator/ (const SparseMatrix& matrix, double value)
{
	  SparseMatrix res(matrix);
	  DoubleTab t1=res.getValues();
	  t1/=value;
	  res.setValues(t1);
	  return (res);
}

SparseMatrix
operator*(const SparseMatrix& matrix1, const SparseMatrix& matrix2)
{
	  int numberOfRows = matrix1.getNumberOfRows();
	  int numberOfColumns = matrix1.getNumberOfColumns();
	  int numberOfRows2 = matrix2.getNumberOfRows();
	  int numberOfColumns2 = matrix2.getNumberOfColumns();

	  if(numberOfColumns!=numberOfRows2)
	  {
			string msg="Matrix Matrix::operator()*(const Matrix& matrix1, const Matrix& matrix2): dimensions of the matrices are incompatible!";
		    throw CdmathException(msg);
	  }
	  SparseMatrix res(numberOfRows, numberOfColumns2);
	  for(int i=0;i<numberOfRows;i++)
	  {
		  for(int j=0;j<numberOfColumns2;j++)
		  {
			  double som=0.;
			  for(int k=0;k<numberOfColumns;k++)
				  som+=matrix1(i,k)*matrix2(k,j);
			  res.setValue(i,j,som);
		  }
	  }
	  return (res);
}

SparseMatrix&
SparseMatrix::operator*= (const SparseMatrix& matrix)
{
	  int numberOfRows2 = matrix.getNumberOfRows();
	  int numberOfColumns2 = matrix.getNumberOfColumns();

	  if(_numberOfColumns!=numberOfRows2)
	  {
			string msg="SparseMatrix SparseMatrix::operator()*(const Matrix& matrix1, const Matrix& matrix2): dimensions of the matrices are incompatible!";
		    throw CdmathException(msg);
	  }
	  SparseMatrix res(_numberOfRows, numberOfColumns2);
	  for(int i=0;i<_numberOfRows;i++)
	  {
		  for(int j=0;j<numberOfColumns2;j++)
		  {
			  double som=0.;
			  for(int k=0;k<_numberOfColumns;k++)
				  som+=(*this)(i,k)*matrix(k,j);
			  res.setValue(i,j,som);
		  }
	  }
	  (*this)=res;
	return (*this);
}

Vector
SparseMatrix::operator* (const Vector& vec) const
{
	Vector res(_numberOfRows);
	for(int i=0; i<_numberOfRows; i++)
	{
		double sum=0.;
		for(int j=0; j<_numberOfColumns; j++)
			sum+=(*this)(i,j)*vec(j);
		res(i) = sum;
	}
	return (res);
}

SparseMatrix&
SparseMatrix::operator+= (const SparseMatrix& matrix)
{
	_values+=matrix.getValues();
	return (*this);
}

SparseMatrix&
SparseMatrix::operator-= (const SparseMatrix& matrix)
{
	_values-=matrix.getValues();
	return (*this);
}

SparseMatrix&
SparseMatrix::operator*= (double value)
{
	_values*=value;
	return (*this);
}

SparseMatrix&
SparseMatrix::operator/= (double value)
{
	_values/=value;
	return (*this);
}

SparseMatrix
SparseMatrix::partMatrix(int row, int column) const
{
   int r = 0;
   int c = 0;
   SparseMatrix res(_numberOfRows-1, _numberOfColumns-1);

   for (int i=0; i<_numberOfRows; i++)
   {
      c = 0;
      if(i != row)
      {
         for(int j=0; j<_numberOfColumns; j++)
            if(j != column)
            	res.setValue(r,c++,(*this)(i,j));
         r++;
      }
   }
   return (res);
}

double
SparseMatrix::determinant() const
{
	if ( ! isSquare() )
	{
		string msg = "SparseMatrix::determinant: Matrix is not square!";
		throw CdmathException(msg);
	}
	else
	{
		SparseMatrix matrix;

		double res = 0.0;
		int dim = _numberOfRows;
		if (dim==1)
			return ((*this)(0,0));

	   for (int i=0; i<dim; i++)
	   {
		   matrix = this->partMatrix(i,0);
		   res += ( coefficient(i)*(*this)(i,0)*(matrix.determinant() ) );
	   }
	   return (res);
	}
}

ostream&
operator<<(ostream& out, const SparseMatrix& matrix)
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
	return (out);
}
