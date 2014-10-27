/*
 * SparseMatrix.hxx
 *
 *  Created on: 25 oct. 2014
 *      Author: amekkas
 */

#ifndef SOURCE_DIRECTORY__BASE_INC_SparseMatrix_HXX_
#define SOURCE_DIRECTORY__BASE_INC_SparseMatrix_HXX_

#include <iostream>

#include "GenericMatrix.hxx"
#include "Vector.hxx"
#include "IntTab.hxx"

class SparseMatrix: public GenericMatrix {

public:
	SparseMatrix();
	virtual ~SparseMatrix();

	/**
     * constructor with data
     * @param numberOfRows : The number of rows
     * @param numberOfColumns : The number of columns
     */
	SparseMatrix( int numberOfRows, int numberOfColumns ) ;

	/**
     * constructor with data
     * @param numberOfRows : The number of rows
     * @param numberOfColumns : The number of columns
     * @param nnz : The number of nonzeros
     */
	SparseMatrix( int numberOfRows, int numberOfColumns, int nnz ) ;

    /**
     * constructor by copy
     * @param SparseMatrix : The SparseMatrix object to be copied
     */
    SparseMatrix ( const SparseMatrix& SparseMatrix ) ;

    SparseMatrix transpose() const ;

	int getBlocNNZ() const ;
	bool getFlagNNZ() const ;

	double operator()( int i, int j ) const ;
	void setValue( int i, int j, double value ) ;

    SparseMatrix& operator+= (const SparseMatrix& SparseMatrix) ;

    SparseMatrix& operator-= (const SparseMatrix& SparseMatrix) ;

    SparseMatrix& operator*= (double value) ;

    SparseMatrix& operator/= (double value) ;

    SparseMatrix& operator*= (const SparseMatrix& matrix) ;

	Vector operator* (const Vector& vector) const ;

    SparseMatrix partMatrix(int row, int column) const ;

    const IntTab& getIndexRows( void ) const ;
	IntTab getIndexRows( void ) ;

	const IntTab& getIndexColumns( void ) const ;
	IntTab getIndexColumns( void ) ;

    int getNumberOfNonZeros ( void ) const ;

    double determinant() const ;

    void setBlocNNZ(int blocNNZ) ;

    const SparseMatrix& operator= ( const SparseMatrix& SparseMatrix ) ;

    friend SparseMatrix operator+ (const SparseMatrix& SparseMatrix1, const SparseMatrix& SparseMatrix2);

    friend SparseMatrix operator- (const SparseMatrix& SparseMatrix1, const SparseMatrix& SparseMatrix2);

    friend SparseMatrix operator* (double value , const SparseMatrix& SparseMatrix ) ;

    friend SparseMatrix operator* (const SparseMatrix& SparseMatrix, double value ) ;

    friend SparseMatrix operator/ (const SparseMatrix& SparseMatrix, double value) ;

    friend SparseMatrix operator*(const SparseMatrix& M, const SparseMatrix& N) ;

    friend std::ostream& operator<<(std::ostream& out, const SparseMatrix& matrix ) ;

private:
    void replaceValue(int i,int j,double value) ;
    void insertValue(int i,int j,double value) ;

protected:
	bool _flagNNZ;
	int _blocNNZ;
    int _numberOfNonZeros ;
	IntTab _indexRows;
	IntTab _indexColumns;
};


#endif /* SOURCE_DIRECTORY__BASE_INC_SparseMatrix_HXX_ */
