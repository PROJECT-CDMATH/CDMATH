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

#include "DoubleTab.hxx"
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

	DoubleTab& operator [] ( int i ) ;

	const DoubleTab& operator [] ( int i ) const ;

	DoubleTab& operator () ( int i ) ;

	const DoubleTab& operator () ( int i ) const ;

	double& operator () ( int i, int j ) ;

	const double& operator () ( int i, int j ) const ;

	Matrix& operator= ( const Matrix& matrix ) ;

	Matrix& operator+= (const Matrix& matrix) ;

	Matrix& operator-= (const Matrix& matrix) ;

	Matrix& operator*= (double value) ;

	Matrix& operator/= (double value) ;

	Vector operator* (const Vector& vector) const ;

	bool isSymmetric() const ;

	bool isSquare() const ;

	Matrix transpose() const ;

	Matrix partMatrix(int row, int column) const ;

	int coefficient(int index) const ;

	double determinant() const ;

	Matrix getMatrixL(void) const ;

	Matrix getMatrixU(void) const ;

	void view() const ;

	friend Matrix operator+ (const Matrix& matrix1, const Matrix& matrix2);

	friend Matrix operator- (const Matrix& matrix1, const Matrix& matrix2);

	friend Matrix operator* (double value , const Matrix& matrix ) ;

	friend Matrix operator* (const Matrix& matrix, double value ) ;

	friend Matrix operator/ (const Matrix& matrix, double value) ;

	friend Matrix operator*(const Matrix& M, const Matrix& N) ;

	friend std::ostream& operator<<(std::ostream& out, const Matrix& matrix ) ;

    private: //----------------------------------------------------------------

	/*
	 * The values in this cell.
	 */
	DoubleTab *_values ;

	/*
	 * The number of rows in this cell.
	 */
	int _numberOfRows ;

	/*
	 * The number of columns in this cell.
	 */
	int _numberOfColumns ;

};

/*
	matrice & operator = (matrice &);	// et l'opperateur d'affectation

	void saisie();						// permet de saisir les éléments d'une matrice
	void afficher() ;					// Permet d'afficher les éléments d'une matrice

	bool estCarre() const;
	bool estSymetrique() const;			// Verifie si la matrice est symétirque (si elle est carré)

	vecteur & operator [] (int);		// Permet l'accès aux vecteurs lignes

	void echagerDeuxLignes(int , int);
	void echangerLinges();				//Echange tous les lignes d'une matrice

	matrice transposee();				// Renvoit la matrice transposée de la matrice
	matrice sousMatrice(int , int ) ;	// Renvoit une sous-matrice en éléminant une ligne et une colonne


	double trace() const;
	double determinant() ;

	matrice coMatrice();
	matrice inverse();

	friend matrice operator+(matrice , matrice ) ;
    friend matrice operator-(matrice , matrice ) ;
    friend matrice operator*(matrice , matrice ) ;
	friend matrice operator*(double  , matrice  ) ;
	friend matrice operator*(matrice , double  ) ;
	friend matrice operator/(matrice , double  ) ;
	friend vecteur operator*(matrice , vecteur);

	matrice & operator+=(matrice );
    matrice & operator-=(matrice );
	matrice & operator*=(matrice );
    matrice & operator*=(double );


	friend ostream & operator<<(ostream&, matrice );
	friend istream & operator>>(istream&, matrice &);

*/

#endif /* MATRIX_HXX_ */
