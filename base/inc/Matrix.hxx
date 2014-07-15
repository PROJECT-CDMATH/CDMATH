/*
 * Matrix.hxx
 *
 *  Created on: 13 April. 2013
 *      Authors: CDMAT
 */

#ifndef MATRIX_HXX_
#define MATRIX_HXX_


/**
 * Cell class is defined by
 * - number of rows
 * - number of columns
 */

namespace ParaMEDMEM
{
  class DenseMatrix;
}


#include <MEDCouplingAutoRefCountObjectPtr.hxx>

#include <iostream>

class Vector ;

class Matrix
{
    public: //----------------------------------------------------------------
	/**
	 * default constructor
	 */
	Matrix ( void ) ;

	/**
	 * constructor with data
	 * @param dim : The number of rows and columns
	 */
	Matrix ( int dim ) ;

	/**
	 * constructor with data
	 * @param numberOfRows : The number of rows
	 * @param numberOfColumns : The number of columns
	 */
	Matrix ( int numberOfRows, int numberOfColumns ) ;

	/**
	 * constructor with data
	 * @param numberOfRows : The number of rows
	 * @param numberOfColumns : The number of columns
	 */
	Matrix ( int numberOfRows, int numberOfColumns, int numberOfNonZeros ) ;

	/**
	 * constructor by copy
	 * @param cell : The matrix object to be copied
	 */
	Matrix ( const Matrix& matrix ) ;

	/**
	 * destructor
	 */
	~Matrix ( void ) ;

	/**
	 * return number of rows in this matrix
	 * @return _numberOfRows
	 */
	int getNumberOfRows ( void ) const ;

	/**
	 * return number of columns in this matrix
	 * @return _numberOfColumns
	 */
	int getNumberOfColumns ( void ) const ;

	bool isSparseMatrix( void ) const ;

	int getNumberOfNonZeros() const ;

	double& operator () ( int i, int j ) ;

	double operator () ( int i, int j ) const ;

	Matrix& operator+= (const Matrix& matrix) ;

	Matrix& operator-= (const Matrix& matrix) ;

	Matrix& operator*= (double value) ;

	Matrix& operator*= (const Matrix& matrix) ;

	Matrix& operator/= (double value) ;

	Vector operator* (const Vector& vector) const ;

	bool isSymmetric() const ;

	bool isSquare() const ;

	Matrix transpose() const ;

	Matrix partMatrix(int row, int column) const ;

	int coefficient(int index) const ;

	double determinant() const ;

	void view() const ;

	const Matrix& operator= ( const Matrix& matrix ) ;

	friend Matrix operator+ (const Matrix& matrix1, const Matrix& matrix2);

	friend Matrix operator- (const Matrix& matrix1, const Matrix& matrix2);

	friend Matrix operator* (double value , const Matrix& matrix ) ;

	friend Matrix operator* (const Matrix& matrix, double value ) ;

	friend Matrix operator/ (const Matrix& matrix, double value) ;

	friend Matrix operator*(const Matrix& M, const Matrix& N) ;

	friend std::ostream& operator<<(std::ostream& out, const Matrix& matrix ) ;

    private: //----------------------------------------------------------------

	void setDensiteMatrix (const ParaMEDMEM::DenseMatrix* mat) ;

	ParaMEDMEM::MEDCouplingAutoRefCountObjectPtr<ParaMEDMEM::DenseMatrix> getMEDCouplingDenseMatrix ( void )  const ;

	/*
	 * The number of rows in this cell.
	 */
	int _numberOfRows ;

	/*
	 * The number of columns in this cell.
	 */
	int _numberOfColumns ;

	int _numberOfNonZeros ;

	bool _isSparseMatrix ;

	ParaMEDMEM::MEDCouplingAutoRefCountObjectPtr<ParaMEDMEM::DenseMatrix> _mat;

};

#endif /* MATRIX_HXX_ */
