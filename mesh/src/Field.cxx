/*
 * field.cxx
 *
 *  Created on: 7 fevrier. 2012
 *      Author: CDMAT
 */

#include "Field.hxx"
#include "MEDFileMesh.hxx"
#include "MEDLoader.hxx"
#include "MEDCouplingUMesh.hxx"
#include "MEDCouplingFieldDouble.hxx"

using namespace ParaMEDMEM;
using namespace std;


//----------------------------------------------------------------------
Field::Field( void )
//----------------------------------------------------------------------
{
	_field=NULL;
}

//----------------------------------------------------------------------
Field::~Field( void )
//----------------------------------------------------------------------
{
	_field->decrRef();
}

Field::Field( const string fieldName, TypeField type, const Mesh& mesh , int numberOfComponents, double time )
{
	_mesh=mesh ;
	DataArrayDouble *array=DataArrayDouble::New();
	if (type==CELLS)
	{
		_field=MEDCouplingFieldDouble::New(ON_CELLS);
		array->alloc(mesh.getNumberOfCells(),numberOfComponents);
	}
	if (type==NODES)
	{
		_field=MEDCouplingFieldDouble::New(ON_NODES);
		array->alloc(mesh.getNumberOfNodes(),numberOfComponents);
	}
	_field->setName(fieldName.c_str()) ;
	_field->setMesh(mesh.getMeshU());
	_field->setArray(array);
	_field->setTime(time,0,0);
	array->decrRef();
}

Field::Field( const string fieldName, TypeField type, const Mesh& mesh , int numberOfComponents)
{
	_mesh=mesh ;
	DataArrayDouble *array=DataArrayDouble::New();
	if (type==CELLS)
	{
		_field=MEDCouplingFieldDouble::New(ON_CELLS);
		array->alloc(mesh.getNumberOfCells(),numberOfComponents);
	}
	if (type==NODES)
	{
		_field=MEDCouplingFieldDouble::New(ON_NODES);
		array->alloc(mesh.getNumberOfNodes(),numberOfComponents);
	}
	_field->setName(fieldName.c_str()) ;
	_field->setMesh(mesh.getMeshU());
	_field->setArray(array);
	_field->setTime(0.0,0,0);
	array->decrRef();
}

Field::Field( const string fieldName, TypeField type, const Mesh& mesh)
{
	_mesh=mesh ;
	DataArrayDouble *array=DataArrayDouble::New();
	if (type==CELLS)
	{
		_field=MEDCouplingFieldDouble::New(ON_CELLS);
		array->alloc(mesh.getNumberOfCells(),1);
	}
	if (type==NODES)
	{
		_field=MEDCouplingFieldDouble::New(ON_NODES);
		array->alloc(mesh.getNumberOfNodes(),1);
	}
	_field->setName(fieldName.c_str()) ;
	_field->setMesh(mesh.getMeshU());
	_field->setArray(array);
	_field->setTime(0.0,0,0);
	array->decrRef();
}

//----------------------------------------------------------------------
Field::Field( const Field & f )
//----------------------------------------------------------------------
{
	_mesh=f.getMesh() ;
    _field=static_cast<MEDCouplingFieldDouble *>(f.getField()->deepCpy());
}

//----------------------------------------------------------------------
MEDCouplingFieldDouble*
Field::getField ( void )  const
//----------------------------------------------------------------------
{
    return _field ;
}

//----------------------------------------------------------------------
void
Field::setField ( MEDCouplingFieldDouble* field )
//----------------------------------------------------------------------
{
	if (_field)
		_field->decrRef();
    _field=static_cast<MEDCouplingFieldDouble *>(field->deepCpy());
}

//----------------------------------------------------------------------
double&
Field::operator() ( int i )
//----------------------------------------------------------------------
{
	return _field->getArray()->getPointer()[i*_field->getNumberOfComponents()];
}

//----------------------------------------------------------------------
double&
Field::operator[] ( int i )
//----------------------------------------------------------------------
{
	return _field->getArray()->getPointer()[i*_field->getNumberOfComponents()];
}

//----------------------------------------------------------------------
double
Field::operator() ( int i ) const
//----------------------------------------------------------------------
{
	return _field->getArray()->getPointer()[i*_field->getNumberOfComponents()];
}

//----------------------------------------------------------------------
double
Field::operator[] ( int i ) const
//----------------------------------------------------------------------
{
	return _field->getArray()->getPointer()[i*_field->getNumberOfComponents()];
}

//----------------------------------------------------------------------
double&
Field::operator() ( int i, int j )
//----------------------------------------------------------------------
{
	return _field->getArray()->getPointer()[i+j*_field->getNumberOfComponents()];
}

//----------------------------------------------------------------------
double
Field::operator() ( int i, int j ) const
//----------------------------------------------------------------------
{
	return _field->getArray()->getPointer()[i+j*_field->getNumberOfComponents()];
}

//----------------------------------------------------------------------
void
Field::setTime ( double time, int iter )
//----------------------------------------------------------------------
{
	_field->setTime(time,iter,0.0);
}
//----------------------------------------------------------------------
double
Field::getTime ( void ) const
//----------------------------------------------------------------------
{
	int a,b;
	return _field->getTime(a,b);
}

//----------------------------------------------------------------------
int
Field::getNumberOfElements ( void ) const 
//----------------------------------------------------------------------
{
	return _field->getNumberOfTuples() ;
}

//----------------------------------------------------------------------
int
Field::getNumberOfComponents ( void ) const 
//----------------------------------------------------------------------
{
	return _field->getNumberOfComponents() ;
}

//----------------------------------------------------------------------
const double*
Field::getValues ( void ) const 
//----------------------------------------------------------------------
{
	return _field->getArray()->getConstPointer() ;
}

//----------------------------------------------------------------------
const string
Field::getName ( void ) const
//----------------------------------------------------------------------
{
	return _field->getName() ;
}

//----------------------------------------------------------------------
Mesh
Field::getMesh ( void ) const
//----------------------------------------------------------------------
{
	return _mesh ;
}

//----------------------------------------------------------------------
TypeField
Field::getTypeOfField ( void ) const
//----------------------------------------------------------------------
{
	if (_field->getTypeOfField()==ON_CELLS)
		return CELLS;
	else
		return NODES;
}

//----------------------------------------------------------------------
void
Field::setName ( const string fieldName )
//----------------------------------------------------------------------
{
	_field->setName(fieldName.c_str()) ;
}


//----------------------------------------------------------------------
Field
Field::operator+ ( const Field& f ) const
//----------------------------------------------------------------------
{
	Field fres("Sum Field",f.getTypeOfField(),f.getMesh(),f.getNumberOfComponents(),f.getTime());
	int nbComp=f.getNumberOfComponents();
	int nbElem=f.getNumberOfElements();
	for (int i=0 ; i<nbComp ; i++)
			for (int j=0 ; j<nbElem; j++)
				fres(i,j)=_field->getArray()->getPointer()[i+j*_field->getNumberOfComponents()]+f(i,j);
	return fres;
}

//----------------------------------------------------------------------
Field
Field::operator- ( const Field& f ) const
//----------------------------------------------------------------------
{
	Field fres("Diff Field",f.getTypeOfField(),f.getMesh(),f.getNumberOfComponents(),f.getTime());
	int nbComp=f.getNumberOfComponents();
	int nbElem=f.getNumberOfElements();
	for (int i=0 ; i<nbComp ; i++)
			for (int j=0 ; j<nbElem; j++)
				fres(i,j)=_field->getArray()->getPointer()[i+j*_field->getNumberOfComponents()]-f(i,j);
	return fres;
}

//----------------------------------------------------------------------
const Field&
Field::operator= ( const Field& f )
//----------------------------------------------------------------------
{
	_mesh=f.getMesh() ;
	if (_field)
		_field->decrRef();
	_field=static_cast<MEDCouplingFieldDouble *>(f.getField()->deepCpy());
	return *this;
}

//----------------------------------------------------------------------
const Field&
Field::operator+= ( const Field& f )
//----------------------------------------------------------------------
{
	_field->setMesh(f.getField()->getMesh());
	  (*_field)+=(*f.getField());
	return *this;
}

//----------------------------------------------------------------------
const Field&
Field::operator-= ( const Field& f )
//----------------------------------------------------------------------
{
	_field->setMesh(f.getField()->getMesh());
	  (*_field)-=(*f.getField());
	return *this;
}

//----------------------------------------------------------------------
const Field&
Field::operator*= ( double s )
//----------------------------------------------------------------------
{
	int nbComp=getNumberOfComponents();
	int nbElem=getNumberOfElements();
	for (int i=0 ; i<nbComp ; i++)
		for (int j=0 ; j<nbElem; j++)
			_field->getArray()->getPointer()[i+j*_field->getNumberOfComponents()]*=s;
	return *this;
}

//----------------------------------------------------------------------
const Field&
Field::operator/= ( double s )
//----------------------------------------------------------------------
{
	int nbComp=getNumberOfComponents();
	int nbElem=getNumberOfElements();
	for (int i=0 ; i<nbComp ; i++)
		for (int j=0 ; j<nbElem; j++)
			_field->getArray()->getPointer()[i+j*_field->getNumberOfComponents()]/=s;
	return *this;
}

//----------------------------------------------------------------------
const Field&
Field::operator-= ( double s )
//----------------------------------------------------------------------
{
	int nbComp=getNumberOfComponents();
	int nbElem=getNumberOfElements();
	for (int i=0 ; i<nbComp ; i++)
			for (int j=0 ; j<nbElem; j++)
				_field->getArray()->getPointer()[i+j*_field->getNumberOfComponents()]-=s;
	return *this;
}

//----------------------------------------------------------------------
const Field&
Field::operator+= ( double s )
//----------------------------------------------------------------------
{
	int nbComp=getNumberOfComponents();
	int nbElem=getNumberOfElements();
	for (int i=0 ; i<nbComp ; i++)
			for (int j=0 ; j<nbElem; j++)
				_field->getArray()->getPointer()[i+j*_field->getNumberOfComponents()]+=s;
	return *this;
}

//----------------------------------------------------------------------
void
Field::writeVTK ( const string fileName ) const
//----------------------------------------------------------------------
{
	_field->writeVTK(fileName.c_str()) ;
}

//----------------------------------------------------------------------
void
Field::writeMED ( const string fileName ) const
//----------------------------------------------------------------------
{
	writeMED(fileName,true);
}

//----------------------------------------------------------------------
void
Field::writeMED ( const string fileName, bool fromScratch ) const
//----------------------------------------------------------------------
{
	if (fromScratch)
		MEDLoader::WriteField(fileName.c_str(),_field,fromScratch);
	else
		MEDLoader::WriteFieldUsingAlreadyWrittenMesh(fileName.c_str(),_field);
}
