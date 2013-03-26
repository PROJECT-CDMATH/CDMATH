/*
 * field.hxx
 *
 *  Created on: 07 fevrier. 2012
 *      Authors: CDMAT groups
 */

#ifndef FIELD_HXX_
#define FIELD_HXX_

namespace ParaMEDMEM
{
  class MEDCouplingFieldDouble;
}

typedef enum
  {
    CELLS = 0,
    NODES = 1,
  } TypeField;

#include "Mesh.hxx"


/**
 * Field class is defined by
 * - ........
 */

class Field
{
    public: //----------------------------------------------------------------
	/**
	 * default constructor
	 */
	Field ( void ) ;

	/**
	* constructor with data:
	* @param fieldName : name of the field
	* @param mesh : mesh of the field
	* @param numberOfComponents : number of the component
	* @param time : time of the field
	*/
	Field( const std::string fieldName, TypeField type, const Mesh& mesh , int numberOfComponents, double time) ;

	/**
	* constructor with data:
	* @param fieldName : name of the field
	* @param mesh : mesh of the field
	* @param numberOfComponents : number of the component
	*/
	Field( const std::string fieldName, TypeField type, const Mesh& mesh , int numberOfComponents) ;

	/**
	* constructor with data:
	* @param fieldName : name of the field
	* @param mesh : mesh of the field
	*/
	Field( const std::string fieldName, TypeField type, const Mesh& mesh) ;

	/**
	* destructor
	*/
	~Field ( void ) ;

	/**
	* constructor by copy
	* @param f : The object to be copied
	*/
	Field ( const Field & field ) ;

	double& operator[] ( int i ) ;

	double operator[] ( int i ) const;

	double& operator() ( int i ) ;

	double operator() ( int i ) const;

	double& operator() ( int i, int j ) ;

	double operator() ( int i, int j ) const ;

	int getNumberOfComponents ( void ) const ;

	const double* getValues ( void ) const ;

	const std::string getName ( void ) const;

	Mesh getMesh ( void ) const ;

	int getNumberOfElements ( void ) const ;

	TypeField getTypeOfField ( void ) const ;

  	/**
  	 * return the mesh MEDCoupling
  	 * return _mesh
  	 */
	ParaMEDMEM::MEDCouplingFieldDouble* getField ( void )  const ;

	void setField ( ParaMEDMEM::MEDCouplingFieldDouble* field );

	void setTime ( double time, int iter );

	double getTime ( void ) const;

	void setName ( const std::string fieldName ) ;

	const Field& operator= ( const Field& f ) ;

	Field operator+ ( const Field& f ) const ;

	Field operator- ( const Field& f ) const ;

	const Field& operator+= ( const Field& f ) ;

	const Field& operator-= ( const Field& f ) ;

	const Field& operator*= ( double s ) ;

	const Field& operator/= ( double s ) ;

	const Field& operator-= ( double s ) ;

	const Field& operator+= ( double s ) ;

	void writeVTK ( const std::string fileName ) const ;

	void writeMED ( const std::string fileName ) const ;

	void writeMED ( const std::string fileName, bool fromScratch ) const ;

    protected: //----------------------------------------------------------------

	ParaMEDMEM::MEDCouplingFieldDouble* _field;
	Mesh _mesh ;
};

#endif /* Field_HXX_ */
