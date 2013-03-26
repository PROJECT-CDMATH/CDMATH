/*
 * mesh.cxx
 *
 *  Created on: 22 janv. 2012
 *      Authors: CDMAT
 */

#include "Mesh.hxx"
#include "Node.hxx"
#include "Cell.hxx"
#include "Face.hxx"
#include "IntTab.hxx"

#include "MEDFileMesh.hxx"
#include "MEDLoader.hxx"
#include "MEDCouplingUMesh.hxx"
#include "MEDCouplingFieldDouble.hxx"

#include <iostream>
#include <cassert>

using namespace ParaMEDMEM;
using namespace std;

//----------------------------------------------------------------------
Mesh::Mesh( void )
//----------------------------------------------------------------------
{
    _mesh=NULL;
    _cells=NULL;
    _nodes=NULL;
    _faces=NULL;
    _dim = 0 ;
    _numberOfNodes = 0;
    _numberOfFaces = 0;
    _numberOfCells = 0;
    _nX=0;
    _nY=0;
    _nZ=0;
}

//----------------------------------------------------------------------
Mesh::~Mesh( void )
//----------------------------------------------------------------------
{
	_mesh->decrRef();
	delete []  _cells;
	delete []  _nodes;
	delete []  _faces;
}

//----------------------------------------------------------------------
Mesh::Mesh( const Mesh& m )
//----------------------------------------------------------------------
{
    _dim = m.getDim() ;
    _nX = m.getNx() ;
    _nY = m.getNy() ;
    _nZ = m.getNz() ;
    _numberOfNodes = m.getNumberOfNodes();
    _numberOfFaces = m.getNumberOfFaces();
    _numberOfCells = m.getNumberOfCells();
    _groups = m.getNamesOfGroups() ;
    _nodes   = new Node[_numberOfNodes] ;
    _faces   = new Face[_numberOfFaces] ;
    _cells   = new Cell[_numberOfCells] ;

    for (int i=0;i<_numberOfNodes;i++)
    	_nodes[i]=m.getNode(i);

    for (int i=0;i<_numberOfFaces;i++)
    	_faces[i]=m.getFace(i);

    for (int i=0;i<_numberOfCells;i++)
    	_cells[i]=m.getCell(i);

    _mesh=static_cast<MEDCouplingUMesh *>(m.getMeshU()->deepCpy());
}

//----------------------------------------------------------------------
Mesh::Mesh( const string filename )
//----------------------------------------------------------------------
{
	readMeshMed(filename);
}

//----------------------------------------------------------------------
void
Mesh::readMeshMed( const string filename)
//----------------------------------------------------------------------
{
    MEDFileUMesh *m=MEDFileUMesh::New(filename.c_str());
    _mesh=m->getMeshAtLevel(0);
    _mesh->setName(m->getName());
    _dim=_mesh->getMeshDimension();
    setMesh();
    setGroups(m);
	m->decrRef();
}

void
Mesh::setGroupAtPlan(double value, int direction, double eps, string groupName)
{
    int nbFace=getNumberOfFaces();
    bool flag=false;
    for (int iface=0;iface<nbFace;iface++)
    {
        double cord=_faces[iface].getBarryCenter()[direction];
        if (direction==direction && abs(cord-value)<eps)
        {
    		_faces[iface].setGroupName(groupName);
    		flag=true;
        }
    }
    if (flag)
    	_groups.push_back(groupName);
}

IntTab
Mesh::getIndexFacePeriodic( void ) const
{
    int nbFace=getNumberOfFaces();
    IntTab indexesFacesPerio(nbFace);
    for (int iface=0;iface<nbFace;iface++)
    	indexesFacesPerio[iface]=getIndexFacePeriodic(iface);
    return indexesFacesPerio;
}

int
Mesh::getIndexFacePeriodic(int indexFace) const
{
	if (!_faces[indexFace].isBorder())
			return -1;
	double minmax[4];
    _mesh->getCoords()->getMinMaxPerComponent(minmax);
    double xmin=minmax[0];
    double xmax=minmax[1];
    double ymin=minmax[2];
    double ymax=minmax[3];
    int nbFace=getNumberOfFaces();
    double x=_faces[indexFace].x();
    double y=_faces[indexFace].y();
    double eps=1.E-10;
    // pos=0 : bottom
    // pos=1 : right
    // pos=2 : top
    // pos=3 : left
    int pos=-1;
    if (abs(y-ymin)<1.E-10)
    	pos=0;
    if (abs(x-xmax)<1.E-10)
    	pos=1;
    if (abs(y-ymax)<1.E-10)
    	pos=2;
    if (abs(x-xmin)<1.E-10)
    	pos=3;
    if (pos==-1)
    	cerr << "periodic face not found " << endl;

    int ifaceOk=-1;
    for (int iface=0;iface<nbFace;iface++)
    {
        double xi=_faces[iface].x();
        double yi=_faces[iface].y();
        if (abs(y-yi)<eps && pos==1 && abs(xi-xmin)<eps)
        {
        	ifaceOk=iface;
        	break;
        }
		if (abs(y-yi)<eps && pos==3 && abs(xi-xmax)<eps)
        {
        	ifaceOk=iface;
        	break;
        }
        if (abs(x-xi)<eps && pos==0 && abs(yi-ymax)<eps)
        {
        	ifaceOk=iface;
        	break;
        }
        if (abs(x-xi)<eps && pos==2 && abs(yi-ymin)<eps)
        {
        	ifaceOk=iface;
        	break;
        }
    }
    return ifaceOk;
}

void
Mesh::setGroups( const MEDFileUMesh* medmesh)
{
	vector<string> groups=medmesh->getGroupsNames() ;
    for (unsigned int i=0;i<groups.size();i++ )
    {
		string groupName=groups[i];
		_groups.push_back(groupName);
		MEDCouplingUMesh *m=medmesh->getGroup(-1,groupName.c_str());
	    DataArrayDouble *baryCell = m->getBarycenterAndOwner() ;
	    const double *coorBary=baryCell->getConstPointer();
	    int nb=m->getNumberOfCells();
	    int k=0;
	    for (int ic=0;ic<nb;ic++)
	    {
	        double xb=coorBary[k];
	        double yb=coorBary[k+1];
	        int flag=0;
	        for (int iface=0;iface<_numberOfFaces;iface++ )
	        {
	        	double xx=_faces[iface].x();
	        	double yy=_faces[iface].y();
	        	if(abs(xx-xb)<1.E-10 && abs(yy-yb)<1.E-10)
	        	{
	        		_faces[iface].setGroupName(groupName);
	        		flag=1;
	        		break;
	        	}
	        }
	        if (flag==0)
	        	assert("face non trouve");
    		k+=2;
	    }
	    baryCell->decrRef();
	    m->decrRef();
    }
}
//----------------------------------------------------------------------
void
Mesh::setMesh( void )
//----------------------------------------------------------------------
{
    DataArrayInt *desc=DataArrayInt::New();
    DataArrayInt *descI=DataArrayInt::New();
    DataArrayInt *revDesc=DataArrayInt::New();
    DataArrayInt *revDescI=DataArrayInt::New();
    MEDCouplingUMesh *m2=_mesh->buildDescendingConnectivity(desc,descI,revDesc,revDescI);
    m2->setName(_mesh->getName());

    DataArrayInt *desc2=DataArrayInt::New();
    DataArrayInt *descI2=DataArrayInt::New();
    DataArrayInt *revDesc2=DataArrayInt::New();
    DataArrayInt *revDescI2=DataArrayInt::New();
    MEDCouplingUMesh *m3=_mesh->buildDescendingConnectivity2(desc2,descI2,revDesc2,revDescI2);
    m3->setName(_mesh->getName());

    MEDCouplingFieldDouble* fields=_mesh->getMeasureField(true);
    DataArrayDouble *surface = fields->getArray();
    const double *surf=surface->getConstPointer();

    const int *tmp=desc->getConstPointer();
    const int *tmpI=descI->getConstPointer();
    DataArrayDouble *baryCell = _mesh->getBarycenterAndOwner() ;
    const double *coorBary=baryCell->getConstPointer();

    const int *tmp2=desc2->getConstPointer();
    const int *tmpI2=descI2->getConstPointer();

    DataArrayDouble *baryCellF = m2->getBarycenterAndOwner() ;

    MEDCouplingFieldDouble* fieldn=m2->buildOrthogonalField();
    DataArrayDouble *normal = fieldn->getArray();
    const double *tmpNormal=normal->getConstPointer();

    _numberOfCells = _mesh->getNumberOfCells() ;
    _cells    = new Cell[_numberOfCells] ;

    int k=0;
    for( int id=0;id<_numberOfCells;id++ )
    {
        const int *work=tmp+tmpI[id];
        const int *work2=tmp2+tmpI2[id];
        int nbFaces=tmpI[id+1]-tmpI[id];
        int nbVertices=_mesh->getNumberOfNodesInCell(id) ;

        Point p(coorBary[k],coorBary[k+1],0.0) ;
        Cell ci( nbVertices, nbFaces, surf[id], p ) ;
        for( int el=0;el<nbFaces;el++ )
        {
            double xn;
            double yn;
            if (work2[el]<0)
            {
                xn=-tmpNormal[2*work[el]];
                yn=-tmpNormal[2*work[el]+1];
            }else
            {
                xn=tmpNormal[2*work[el]];
                yn=tmpNormal[2*work[el]+1];
            }
            ci.addNormalVector(el,xn,yn,0.0) ;
            ci.addFaceId(el,work[el]) ;
        }
        std::vector<int> nodeIdsOfCell ;
        _mesh->getNodeIdsOfCell(id,nodeIdsOfCell) ;
        for( int el=0;el<nbVertices;el++ )
            ci.addNodeId(el,nodeIdsOfCell[el]) ;
        _cells[id] = ci ;
        k+=2;
    }

    DataArrayInt *revNode=DataArrayInt::New();
    DataArrayInt *revNodeI=DataArrayInt::New();
    _mesh->getReverseNodalConnectivity(revNode,revNodeI) ;
    const int *tmpN=revNode->getConstPointer();
    const int *tmpNI=revNodeI->getConstPointer();

    DataArrayInt *revCell=DataArrayInt::New();
    DataArrayInt *revCellI=DataArrayInt::New();
    m2->getReverseNodalConnectivity(revCell,revCellI) ;
    const int *tmpC=revCell->getConstPointer();
    const int *tmpCI=revCellI->getConstPointer();

    _numberOfNodes = _mesh->getNumberOfNodes() ;
    _nodes    = new Node[_numberOfNodes] ;

    for( int id=0;id<_numberOfNodes;id++ )
    {
        std::vector<double> coo ;
        _mesh->getCoordinatesOfNode(id,coo);
        Point p(coo[0],coo[1],0.0) ;
        const int *workc=tmpN+tmpNI[id];
        int nbCells=tmpNI[id+1]-tmpNI[id];
        const int *workf=tmpC+tmpCI[id];
        int nbFaces=tmpCI[id+1]-tmpCI[id];
        Node vi( nbCells, nbFaces, p ) ;

        for( int el=0;el<nbCells;el++ )
            vi.addCellId(el,workc[el]) ;
        for( int el=0;el<nbFaces;el++ )
            vi.addFaceId(el,workf[el]) ;
        _nodes[id] = vi ;
    }

    const DataArrayInt *nodal = m2->getNodalConnectivity() ;
    const DataArrayInt *nodalI = m2->getNodalConnectivityIndex() ;
    const int *tmpNE=nodal->getConstPointer();
    const int *tmpNEI=nodalI->getConstPointer();


    const int *tmpA=revDesc->getConstPointer();
    const int *tmpAI=revDescI->getConstPointer();

    _numberOfFaces = m2->getNumberOfCells();
    _faces = new Face[_numberOfFaces] ;

    MEDCouplingFieldDouble* fieldl=m2->getMeasureField(true);
    DataArrayDouble *longueur = fieldl->getArray();
    const double *lon=longueur->getConstPointer();

    DataArrayDouble *barySeg = m2->getBarycenterAndOwner() ;
    const double *coorBarySeg=barySeg->getConstPointer();

    k=0;
    for(int id=0;id<_numberOfFaces;id++)
    {
        Point p(coorBarySeg[k],coorBarySeg[k+1],0.0) ;
        const int *workc=tmpA+tmpAI[id];
        int nbCells=tmpAI[id+1]-tmpAI[id];

        const int *workv=tmpNE+tmpNEI[id]+1;
        Face fi( 2, nbCells, lon[id], p ) ;
        fi.addNodeId(0,workv[0]) ;
        fi.addNodeId(1,workv[1]) ;

        fi.addCellId(0,workc[0]) ;
        if (nbCells==2)
                fi.addCellId(1,workc[1]) ;

        _faces[id] = fi ;
        k+=2;
    }

    /*
    double minmax[4];
    _mesh->getCoords()->getMinMaxPerComponent(minmax);
    double xmin=minmax[0];
    double xmax=minmax[1];
    double ymin=minmax[2];
    double ymax=minmax[3];
    double eps=1.E-10;
    setGroupAtPlan(xmax,0,eps,"RightEdge");
    setGroupAtPlan(xmin,0,eps,"LeftEdge");
    setGroupAtPlan(ymin,1,eps,"BottomEdge");
    setGroupAtPlan(ymax,1,eps,"TopEdge");
*/
    fieldl->decrRef();
    fields->decrRef();
    fieldn->decrRef();
    revCell->decrRef();
    revCellI->decrRef();
    revNode->decrRef();
    revNodeI->decrRef();
    baryCell->decrRef();
    baryCellF->decrRef();
    barySeg->decrRef();
    desc->decrRef();
    descI->decrRef();
    revDesc->decrRef();
    revDescI->decrRef();
    desc2->decrRef();
    descI2->decrRef();
    revDesc2->decrRef();
    revDescI2->decrRef();
    m2->decrRef();
    m3->decrRef();
}

//----------------------------------------------------------------------
Mesh::Mesh( double xinf, double xsup, int nx )
//----------------------------------------------------------------------
{
    // il manque test xsup siinf
    double lx = xsup - xinf ;
    double dx = lx/nx ;

    _dim = 1 ;
    _nX=nx ;
    _nY=0 ;
    _nZ=0 ;

    double *arrX = new double[nx+1] ;
    for ( int i=0; i < nx+1 ; i++ )
            arrX[i] = xinf + i*dx ;

    MEDCouplingCMesh *m1c=MEDCouplingCMesh::New();
    DataArrayDouble *coordsX=DataArrayDouble::New();
    coordsX->alloc(nx+1,1);
    std::copy(arrX,arrX+nx+1,coordsX->getPointer());
    m1c->setCoordsAt(0,coordsX);
    coordsX->decrRef();
    delete [] arrX ;
    _mesh=m1c->buildUnstructured();
    _mesh->setName("MeshCart1D");

    DataArrayInt *desc=DataArrayInt::New();
    DataArrayInt *descI=DataArrayInt::New();
    DataArrayInt *revDesc=DataArrayInt::New();
    DataArrayInt *revDescI=DataArrayInt::New();
    MEDCouplingUMesh *m2=_mesh->buildDescendingConnectivity(desc,descI,revDesc,revDescI);
    m2->setName(_mesh->getName());

    DataArrayDouble *baryCell = _mesh->getBarycenterAndOwner() ;
    const double *coorBary=baryCell->getConstPointer();

    _numberOfCells = _mesh->getNumberOfCells() ;
    _cells    = new Cell[_numberOfCells] ;

    MEDCouplingFieldDouble* fieldl=_mesh->getMeasureField(true);
    DataArrayDouble *longueur = fieldl->getArray();
    const double *lon=longueur->getConstPointer();

    int comp=0;
    for( int id=0;id<_numberOfCells;id++ )
    {
        int nbVertices=_mesh->getNumberOfNodesInCell(id) ;
        Point p(coorBary[id],0.0,0.0) ;
        Cell ci( nbVertices, nbVertices, lon[id], p ) ;

        std::vector<int> nodeIdsOfCell ;
        _mesh->getNodeIdsOfCell(id,nodeIdsOfCell) ;
        for( int el=0;el<nbVertices;el++ )
        {
            ci.addNodeId(el,nodeIdsOfCell[el]) ;
            ci.addFaceId(el,nodeIdsOfCell[el]) ;
        }
        _cells[id] = ci ;
        comp=comp+2;
    }


    DataArrayInt *revNode=DataArrayInt::New();
    DataArrayInt *revNodeI=DataArrayInt::New();
    _mesh->getReverseNodalConnectivity(revNode,revNodeI) ;
    const int *tmpN=revNode->getConstPointer();
    const int *tmpNI=revNodeI->getConstPointer();

    _numberOfNodes = _mesh->getNumberOfNodes() ;
    _nodes    = new Node[_numberOfNodes] ;
    _numberOfFaces = _numberOfNodes;
    _faces    = new Face[_numberOfFaces] ;

    for( int id=0;id<_numberOfNodes;id++ )
    {
        std::vector<double> coo ;
        _mesh->getCoordinatesOfNode(id,coo);
        Point p(coo[0],0.0,0.0) ;
        const int *workc=tmpN+tmpNI[id];
        int nbCells=tmpNI[id+1]-tmpNI[id];
        int nbFaces=1;
        Node vi( nbCells, nbFaces, p ) ;
        int nbVertices=1;
        /* provisoire !!!!!!!!!!!!*/
        Point pf(0.0,0.0,0.0) ;
        Face fi( nbVertices, nbCells, 0.0, pf ) ;

        for( int el=0;el<nbCells;el++ )
            vi.addCellId(el,workc[el]) ;
        for( int el=0;el<nbFaces;el++ )
            vi.addFaceId(el,id) ;
        _nodes[id] = vi ;

        for( int el=0;el<nbVertices;el++ )
            fi.addNodeId(el,id) ;

        for( int el=0;el<nbCells;el++ )
            fi.addCellId(el,workc[el]) ;
        _faces[id] = fi ;
    }
    fieldl->decrRef();
    baryCell->decrRef();
    desc->decrRef();
    descI->decrRef();
    revDesc->decrRef();
    revDescI->decrRef();
    revNode->decrRef();
    revNodeI->decrRef();
    m1c->decrRef();
    m2->decrRef();
}

//----------------------------------------------------------------------
Mesh::Mesh( double xinf, double xsup, int nx, double yinf, double ysup, int ny)
//----------------------------------------------------------------------
{

// Anouar a faire
//  if ( xinf >= xsup and yinf >= ysup )
// exit probleme
    double lx = xsup - xinf ;
    double ly = ysup - yinf ;
    _nX=nx ;
    _nY=ny ;
    _nZ=0 ;

    double dx = lx/nx ;
    double dy = ly/ny ;

    double *arrX = new double[nx+1] ;
    for ( int i=0; i < nx+1 ; i++ )
            arrX[i] = xinf + i*dx ;
    double *arrY = new double[ny+1] ;
    for ( int i=0; i < ny+1 ; i++ )
            arrY[i] = yinf + i*dy ;

    _dim = 2 ;

    MEDCouplingCMesh *m1=MEDCouplingCMesh::New();
    DataArrayDouble *coordsX=DataArrayDouble::New();
    coordsX->alloc(nx+1,1);
    std::copy(arrX,arrX+nx+1,coordsX->getPointer());
    m1->setCoordsAt(0,coordsX);
    coordsX->decrRef();
    delete [] arrX ;
    DataArrayDouble *coordsY=DataArrayDouble::New();
    coordsY->alloc(ny+1,1);
    std::copy(arrY,arrY+ny+1,coordsY->getPointer());
    m1->setCoordsAt(1,coordsY);
    coordsY->decrRef();
    delete [] arrY ;
    _mesh=m1->buildUnstructured();
    _mesh->setName("MeshCart2D");
    m1->decrRef();
    setMesh();
}

//----------------------------------------------------------------------
Mesh::Mesh( double xinf, double xsup, int nx, double yinf, double ysup, int ny, double zinf, double zsup, int nz)
//----------------------------------------------------------------------
{
    // Anouar a faire
    //  if ( xinf >= xsup and yinf >= ysup )
    // exit probleme
    double lx = xsup - xinf ;
    double ly = ysup - yinf ;
    double lz = zsup - zinf ;

    _nX=nx ;
    _nY=ny ;
    _nZ=nz ;

    double dx = lx/nx ;
    double dy = ly/ny ;
    double dz = lz/nz ;

    double *arrX = new double[nx+1] ;
    for ( int i=0; i < nx+1 ; i++ )
            arrX[i] = xinf + i*dx ;
    double *arrY = new double[ny+1] ;
    for ( int i=0; i < ny+1 ; i++ )
            arrY[i] = yinf + i*dy ;
    double *arrZ = new double[nz+1] ;
    for ( int i=0; i < nz+1 ; i++ )
            arrZ[i] = zinf + i*dz ;

    _dim = 3 ;

    MEDCouplingCMesh *m1c=MEDCouplingCMesh::New();
    DataArrayDouble *coordsX=DataArrayDouble::New();
    coordsX->alloc(nx+1,1);
    std::copy(arrX,arrX+nx+1,coordsX->getPointer());
    m1c->setCoordsAt(0,coordsX);
    coordsX->decrRef();
    delete [] arrX ;
    DataArrayDouble *coordsY=DataArrayDouble::New();
    coordsY->alloc(ny+1,1);
    std::copy(arrY,arrY+ny+1,coordsY->getPointer());
    m1c->setCoordsAt(1,coordsY);
    coordsY->decrRef();
    delete [] arrY ;
    DataArrayDouble *coordsZ=DataArrayDouble::New();
    coordsZ->alloc(nz+1,1);
    std::copy(arrZ,arrZ+nz+1,coordsZ->getPointer());
    m1c->setCoordsAt(0,coordsZ);
    coordsZ->decrRef();
    delete [] arrZ ;
    _mesh=m1c->buildUnstructured();
    _mesh->setName("MeshCart3D");
    m1c->decrRef();
    setMesh();
}

//----------------------------------------------------------------------
int
Mesh::getDim( void )  const 
//----------------------------------------------------------------------
{
    return _dim ;
}

//----------------------------------------------------------------------
int
Mesh::getNx( void )  const
//----------------------------------------------------------------------
{
    return _nX ;
}

//----------------------------------------------------------------------
int
Mesh::getNy( void )  const
//----------------------------------------------------------------------
{
    return _nY ;
}

//----------------------------------------------------------------------
int
Mesh::getNz( void )  const
//----------------------------------------------------------------------
{
    return _nZ ;
}

//----------------------------------------------------------------------
MEDCouplingUMesh*
Mesh::getMeshU ( void )  const 
//----------------------------------------------------------------------
{
    return _mesh ;
}

//----------------------------------------------------------------------
int
Mesh::getNumberOfNodes ( void ) const 
//----------------------------------------------------------------------
{
    return _numberOfNodes ;
}

//----------------------------------------------------------------------
int
Mesh::getNumberOfCells ( void ) const 
//----------------------------------------------------------------------
{
    return _numberOfCells ;
}

//----------------------------------------------------------------------
int
Mesh::getNumberOfFaces ( void ) const 
//----------------------------------------------------------------------
{
    return _numberOfFaces ;
}

//----------------------------------------------------------------------
Face*
Mesh::getFaces ( void )  const 
//----------------------------------------------------------------------
{
    return _faces ;
}

//----------------------------------------------------------------------
Cell*
Mesh::getCells ( void ) const 
//----------------------------------------------------------------------
{
    return _cells ;
}

//----------------------------------------------------------------------
Cell&
Mesh::getCell ( int i ) const 
//----------------------------------------------------------------------
{
    return _cells[i] ;
}

//----------------------------------------------------------------------
Face&
Mesh::getFace ( int i ) const 
//----------------------------------------------------------------------
{
    return _faces[i] ;
}

//----------------------------------------------------------------------
Node&
Mesh::getNode ( int i ) const 
//----------------------------------------------------------------------
{
    return _nodes[i] ;
}

//----------------------------------------------------------------------
Node*
Mesh::getNodes ( void )  const 
//----------------------------------------------------------------------
{
    return _nodes ;
}

vector<string>
Mesh::getNamesOfGroups( void )  const
{
	return _groups;
}

//----------------------------------------------------------------------
const Mesh&
Mesh::operator= ( const Mesh& mesh )
//----------------------------------------------------------------------
{
    _dim = mesh.getDim() ;
    _numberOfNodes = mesh.getNumberOfNodes();
    _numberOfFaces = mesh.getNumberOfFaces();
    _numberOfCells = mesh.getNumberOfCells();
    _groups = mesh.getNamesOfGroups() ;
    _nX = mesh.getNx() ;
    _nY = mesh.getNy() ;
    _nZ = mesh.getNz() ;

    if (_nodes)
    {
        delete [] _nodes ;
        _nodes=NULL;
    }
    if (_faces)
    {
        delete [] _faces ;
        _faces=NULL;
    }
    if (_cells)
    {
        delete [] _cells ;
        _cells=NULL;
    }

    _nodes   = new Node[_numberOfNodes] ;
    _faces   = new Face[_numberOfFaces] ;
    _cells   = new Cell[_numberOfCells] ;

    for (int i=0;i<_numberOfNodes;i++)
    	_nodes[i]=mesh.getNode(i);

    for (int i=0;i<_numberOfFaces;i++)
    	_faces[i]=mesh.getFace(i);

    for (int i=0;i<_numberOfCells;i++)
    	_cells[i]=mesh.getCell(i);

    if (_mesh)
    	_mesh->decrRef();
    _mesh=static_cast<MEDCouplingUMesh *>(mesh.getMeshU()->deepCpy());
    return *this;
}

//----------------------------------------------------------------------
void
Mesh::writeVTK ( const string fileName ) const
//----------------------------------------------------------------------
{
	_mesh->writeVTK(fileName.c_str()) ;
}

//----------------------------------------------------------------------
void
Mesh::writeMED ( const string fileName ) const
//----------------------------------------------------------------------
{
	MEDLoader::WriteUMesh(fileName.c_str(),_mesh,true);
}
