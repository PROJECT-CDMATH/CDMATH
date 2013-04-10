/*
 * field.cxx
 *
 *  Created on: 7 fevrier. 2012
 *      Author: CDMAT
 */

#include "Node.hxx"
#include "Cell.hxx"
#include "Field.hxx"
#include "MEDFileMesh.hxx"
#include "MEDLoader.hxx"
#include "MEDCouplingUMesh.hxx"
#include "MEDCouplingFieldDouble.hxx"

#include <fstream>
#include <sstream>
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
	_typeField=type;
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
	_typeField=type;
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
	_typeField=type;
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
//-------------------------------t---------------------------------------
{
	_mesh=f.getMesh() ;
    _field=static_cast<MEDCouplingFieldDouble *>(f.getField()->deepCpy());
	_typeField=f.getTypeOfField();
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
	return _typeField;
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
	_typeField=f.getTypeOfField() ;
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
	writeVTK (fileName,true);
}

//----------------------------------------------------------------------
void
Field::writeVTK ( const string fileName, bool fromScratch ) const
//----------------------------------------------------------------------
{
	string fname=fileName+".pvd";
    int iter,order;
    double time=_field->getTime(iter,order);

    if (fromScratch)
	{
        ofstream file(fname.c_str()) ;
		file << "<VTKFile type=\"Collection\" version=\"0.1\" byte_order=\"LittleEndian\"><Collection>\n" ;
    	ostringstream numfile;
        numfile << iter ;
        string filetmp=fileName+"_";
        filetmp=filetmp+numfile.str();
    	filetmp=filetmp+".vtu";
    	file << "<DataSet timestep=\""<< time << "\" group=\"\" part=\"0\" file=\"" << filetmp << "\"/>\n" ;
    	_field->writeVTK(filetmp.c_str()) ;
        file << "</Collection></VTKFile>\n" ;
        file.close() ;
	}
	else
	{
        ifstream file1(fname.c_str()) ;
	    string contenus;
	    getline(file1, contenus, '\0');
	    string to_remove="</Collection></VTKFile>";
	    size_t m = contenus.find(to_remove);
	    size_t n = contenus.find_first_of("\n", m + to_remove.length());
	    contenus.erase(m, n - m + 1);
        file1.close() ;
        ofstream file(fname.c_str()) ;
	    file << contenus ;
	    ostringstream numfile;
        numfile << iter ;
        string filetmp=fileName+"_";
        filetmp=filetmp+numfile.str();
    	filetmp=filetmp+".vtu";
    	file << "<DataSet timestep=\""<< time << "\" group=\"\" part=\"0\" file=\"" << filetmp << "\"/>\n" ;
    	_field->writeVTK(filetmp.c_str()) ;
        file << "</Collection></VTKFile>\n" ;
        file.close() ;
	}
}

//----------------------------------------------------------------------
void
Field::writeASCII ( const string fileName ) const
//----------------------------------------------------------------------
{
    int iter,order;
    double time=_field->getTime(iter,order);

	ostringstream numfile;
	numfile << iter ;
	string filetmp=fileName+"_";
	filetmp=filetmp+numfile.str();
	filetmp=filetmp+".csv";
	ofstream file(filetmp.c_str()) ;
	int dim=_mesh.getDim();
	int nbElements;
	if (getTypeOfField()==CELLS)
		nbElements=_mesh.getNumberOfCells();
	else
		nbElements=_mesh.getNumberOfNodes();

	if (dim==1)
	{
		file << "x," << _field->getName() << endl;
		for (int i=0;i<nbElements;i++)
			if (getTypeOfField()==CELLS)
				file << _mesh.getCell(i).x() << "," << getValues()[i] << endl;
			else
				file << _mesh.getNode(i).x() << "," << getValues()[i] << endl;
	}else if (dim==2)
	{
		int nbCompo=getNumberOfComponents();
		if (nbCompo==1)
			file << "x,y," << _field->getName() << endl;
		else if (nbCompo>1)
		{
			file << "x,y";
			for (int i=0;i<nbCompo;i++)
			{
		    	ostringstream numCompo;
		    	numCompo << i+1 ;
				file << "," << _field->getName() << " Compo " << numCompo;

			}
			file << endl;
		}
		for (int i=0;i<nbElements;i++)
		{
			if (getTypeOfField()==CELLS)
				file << _mesh.getCell(i).x() << "," << _mesh.getCell(i).y() ;
			else
				file << _mesh.getNode(i).x() << "," << _mesh.getNode(i).y() ;
			for (int j=0;j<nbCompo;j++)
				file << "," << getValues()[i+j*nbCompo] ;
			file << endl;
		}
	}
    file.close() ;
}

//----------------------------------------------------------------------
void
Field::writeMED ( const string fileName ) const
//----------------------------------------------------------------------
{
	string fname=fileName+".med";
	writeMED(fileName,true);
}

//----------------------------------------------------------------------
void
Field::writeMED ( const string fileName, bool fromScratch ) const
//----------------------------------------------------------------------
{
	string fname=fileName+".med";
	if (fromScratch)
		MEDLoader::WriteField(fname.c_str(),_field,fromScratch);
	else
		MEDLoader::WriteFieldUsingAlreadyWrittenMesh(fname.c_str(),_field);
}
