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

#include "CdmathException.hxx"

#include <fstream>
#include <sstream>
using namespace ParaMEDMEM;
using namespace std;


//----------------------------------------------------------------------
Field::Field( void )
//----------------------------------------------------------------------
{
    _field=NULL;
    _typeField=CELLS;
}

//----------------------------------------------------------------------
Field::~Field( void )
//----------------------------------------------------------------------
{
}

Field::Field(const std::string fieldName, TypeField type, const Mesh& mesh, int numberOfComponents, double time)
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
    MEDCouplingUMesh* mu=mesh.getMEDCouplingMesh()->buildUnstructured();
    _field->setMesh(mu);
    _field->setArray(array);
    _field->setTime(time,0,0);
    array->decrRef();
    mu->decrRef();
}

Field::Field(const std::string fieldName, TypeField type, const Mesh& mesh, int numberOfComponents)
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
    MEDCouplingUMesh* mu=mesh.getMEDCouplingMesh()->buildUnstructured();
    _field->setMesh(mu);
    _field->setArray(array);
    _field->setTime(0.0,0,0);
    array->decrRef();
    mu->decrRef();
}

Field::Field(const std::string fieldName, TypeField type, const Mesh& mesh)
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
    MEDCouplingUMesh* mu=mesh.getMEDCouplingMesh()->buildUnstructured();
    _field->setMesh(mu);
    _field->setArray(array);
    _field->setTime(0.0,0,0);
    array->decrRef();
    mu->decrRef();
}

DoubleTab
Field::getNormEuclidean() const
{
    DoubleTab norm(getNumberOfElements(),_field->magnitude()->getArray()->getConstPointer());
    return norm;
}

//----------------------------------------------------------------------
Field::Field( const Field & f )
//----------------------------------------------------------------------
{
    _mesh=f.getMesh() ;
    MEDCouplingAutoRefCountObjectPtr<MEDCouplingFieldDouble> f1=f.getField()->deepCpy();
    _field=f1;
    _typeField=f.getTypeOfField();
}

//----------------------------------------------------------------------
MEDCouplingAutoRefCountObjectPtr<MEDCouplingFieldDouble>
Field::getField ( void )  const
//----------------------------------------------------------------------
{
    return _field ;
}

//----------------------------------------------------------------------
void
Field::setFieldByMEDCouplingFieldDouble ( const MEDCouplingFieldDouble* field )
//----------------------------------------------------------------------
{
    MEDCouplingAutoRefCountObjectPtr<MEDCouplingFieldDouble> ff=field->deepCpy();
    _field=ff;
}

//----------------------------------------------------------------------
void
Field::setFieldByDataArrayDouble ( const DataArrayDouble* array )
//----------------------------------------------------------------------
{
    _field->setArray(const_cast<DataArrayDouble*>(array));
}

//----------------------------------------------------------------------
double&
Field::operator() ( int ielem )
//----------------------------------------------------------------------
{
    if(ielem>_field->getNumberOfTuples() || ielem<0)
        throw CdmathException("double& Field::operator(ielem) : ielem>number of values !");
    return _field->getArray()->getPointer()[ielem*_field->getNumberOfComponents()];
}

//----------------------------------------------------------------------
double&
Field::operator[] ( int ielem )
//----------------------------------------------------------------------
{
    if(ielem>_field->getNumberOfTuples() || ielem<0)
        throw CdmathException("double& Field::operator[ielem] : ielem>number of values !");
    return _field->getArray()->getPointer()[ielem*_field->getNumberOfComponents()];
}

//----------------------------------------------------------------------
double
Field::operator() ( int ielem ) const
//----------------------------------------------------------------------
{
    if(ielem>_field->getNumberOfTuples() || ielem<0)
        throw CdmathException("double Field::operator(ielem) : ielem>number of values !");
    return _field->getArray()->getConstPointer()[ielem*_field->getNumberOfComponents()];
}

//----------------------------------------------------------------------
double
Field::operator[] ( int ielem ) const
//----------------------------------------------------------------------
{
    if(ielem>_field->getNumberOfTuples() || ielem<0)
        throw CdmathException("double Field::operator[ielem] : ielem>number of values !");
    return _field->getArray()->getConstPointer()[ielem*_field->getNumberOfComponents()];
}

//----------------------------------------------------------------------
double&
Field::operator() ( int ielem, int jcomp )
//----------------------------------------------------------------------
{
    if(ielem>_field->getNumberOfTuples() || jcomp>_field->getNumberOfComponents() || ielem<0 || jcomp<0)
        throw CdmathException("double& Field::operator( int ielem, int jcomp ) : ielem>number of values or jcomp>number of components !");
    return _field->getArray()->getPointer()[jcomp+ielem*_field->getNumberOfComponents()];
}

//----------------------------------------------------------------------
double
Field::operator() (  int ielem, int jcomp ) const
//----------------------------------------------------------------------
{
    if(ielem>_field->getNumberOfTuples() || jcomp>_field->getNumberOfComponents() || ielem<0 || jcomp<0)
        throw CdmathException("double Field::operator(  int ielem, int jcomp ) : ielem>number of values or jcomp>number of components !");
    return _field->getArray()->getConstPointer()[jcomp+ielem*_field->getNumberOfComponents()];
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

int
Field::getSpaceDimension( void ) const
{
    return _mesh.getSpaceDimension() ;
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
    for (int ielem=0 ; ielem<nbElem; ielem++)
        for (int jcomp=0 ; jcomp<nbComp ; jcomp++)
            fres(ielem, jcomp)=_field->getArray()->getConstPointer()[jcomp+ielem*_field->getNumberOfComponents()]+f(ielem, jcomp);
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
    for (int ielem=0 ; ielem<nbElem; ielem++)
        for (int jcomp=0 ; jcomp<nbComp ; jcomp++)
            fres(ielem, jcomp)=_field->getArray()->getConstPointer()[jcomp+ielem*_field->getNumberOfComponents()]-f(ielem, jcomp);
    return fres;
}

//----------------------------------------------------------------------
const Field&
Field::operator= ( const Field& f )
//----------------------------------------------------------------------
{
    _mesh=f.getMesh() ;
    _typeField=f.getTypeOfField() ;
    MEDCouplingAutoRefCountObjectPtr<MEDCouplingFieldDouble> f1=f.getField()->deepCpy();
    _field=f1;
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
Field::writeVTK (const std::string fileName) const
//----------------------------------------------------------------------
{
    writeVTK (fileName,true);
}

//----------------------------------------------------------------------
void
Field::writeVTK (const std::string fileName, bool fromScratch) const
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
        string ret=_field->writeVTK(filetmp.c_str()) ;
        file << "<DataSet timestep=\""<< time << "\" group=\"\" part=\"0\" file=\"" << ret << "\"/>\n" ;
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
        string ret=_field->writeVTK(filetmp.c_str()) ;
        file << "<DataSet timestep=\""<< time << "\" group=\"\" part=\"0\" file=\"" << ret << "\"/>\n" ;
        file << "</Collection></VTKFile>\n" ;
        file.close() ;
    }
}

//----------------------------------------------------------------------
void
Field::writeCSV ( const std::string fileName ) const
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
    int dim=_mesh.getSpaceDimension();
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
Field::writeMED ( const std::string fileName ) const
//----------------------------------------------------------------------
{
    string fname=fileName+".med";
    writeMED(fileName,true);
}

//----------------------------------------------------------------------
void
Field::writeMED ( const std::string fileName, bool fromScratch ) const
//----------------------------------------------------------------------
{
    string fname=fileName+".med";
    if (fromScratch)
        MEDLoader::WriteField(fname.c_str(),_field,fromScratch);
    else
        MEDLoader::WriteFieldUsingAlreadyWrittenMesh(fname.c_str(),_field);
}
