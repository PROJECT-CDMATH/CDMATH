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
#include "MEDCouplingIMesh.hxx"
#include "MEDCouplingFieldDouble.hxx"

#include "CdmathException.hxx"

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
    _xMin=0.;
    _xSup=0.;
    _yMin=0.;
    _ySup=0.;
    _zMin=0.;
    _zSup=0.;
    _groups.resize(0);
}

//----------------------------------------------------------------------
Mesh::~Mesh( void )
//----------------------------------------------------------------------
{
	delete [] _cells;
	delete [] _nodes;
	delete [] _faces;
}

Mesh::Mesh( const ParaMEDMEM::MEDCouplingIMesh* mesh )
{
	_dim=mesh->getSpaceDimension();
	vector<double> dxyz=mesh->getDXYZ();
    vector<int> nxyz=mesh->getCellGridStructure();
    double* Box0=new double[2*_dim];
    mesh->getBoundingBox(Box0);
    _nxyz = nxyz ;

    _xMin=Box0[0];
    _xSup=Box0[1];
    if (_dim==2)
    {
        _yMin=Box0[2];
        _ySup=Box0[3];
    }
    if (_dim==3)
    {
		_zMin=Box0[4];
		_zSup=Box0[5];
    }
    _dxyz=mesh->getDXYZ();

	double *originPtr = new double[_dim];
    double *dxyzPtr = new double[_dim];
    int *nodeStrctPtr = new int[_dim];

    for(int i=0;i<_dim;i++)
    {
		originPtr[i]=Box0[2*i];
		nodeStrctPtr[i]=nxyz[i]+1;
		dxyzPtr[i]=dxyz[i];
    }

    _mesh=MEDCouplingIMesh::New("MESH2D",
    						    _dim,
    							nodeStrctPtr,
    							nodeStrctPtr+_dim,
    							originPtr,
    							originPtr+_dim,
    							dxyzPtr,
    							dxyzPtr+_dim);
    delete [] originPtr;
    delete [] dxyzPtr;
    delete [] nodeStrctPtr;
    delete [] Box0 ;
    _groups.resize(0);
    setMesh();
}

//----------------------------------------------------------------------
Mesh::Mesh( const Mesh& m )
//----------------------------------------------------------------------
{
    _dim = m.getSpaceDimension() ;
    _nxyz = m.getCellGridStructure() ;
    _xMin=m.getXMin();
    _xSup=m.getXSup();
    _yMin=m.getYMin();
    _ySup=m.getYSup();
    _zMin=m.getZMin();
    _zSup=m.getZSup();

    _dxyz=m.getDXYZ();

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

    MEDCouplingAutoRefCountObjectPtr<MEDCouplingMesh> m1=m.getMEDCouplingMesh()->deepCpy();
    _mesh=m1;
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
    	indexesFacesPerio(iface)=getIndexFacePeriodic(iface);
    return indexesFacesPerio;
}

int
Mesh::getIndexFacePeriodic(int indexFace) const
{
	if (!_faces[indexFace].isBorder())
			return -1;
	double minmax[4];
    _mesh->buildUnstructured()->getCoords()->getMinMaxPerComponent(minmax);
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
    MEDCouplingUMesh* mu=_mesh->buildUnstructured();
    MEDCouplingUMesh *m2=mu->buildDescendingConnectivity(desc,descI,revDesc,revDescI);
    m2->setName(mu->getName());

    DataArrayInt *desc2=DataArrayInt::New();
    DataArrayInt *descI2=DataArrayInt::New();
    DataArrayInt *revDesc2=DataArrayInt::New();
    DataArrayInt *revDescI2=DataArrayInt::New();
    MEDCouplingUMesh *m3=mu->buildDescendingConnectivity2(desc2,descI2,revDesc2,revDescI2);
    m3->setName(mu->getName());

    MEDCouplingFieldDouble* fields=mu->getMeasureField(true);
    DataArrayDouble *surface = fields->getArray();
    const double *surf=surface->getConstPointer();

    const int *tmp=desc->getConstPointer();
    const int *tmpI=descI->getConstPointer();
    DataArrayDouble *baryCell = mu->getBarycenterAndOwner() ;
    const double *coorBary=baryCell->getConstPointer();

    const int *tmp2=desc2->getConstPointer();
    const int *tmpI2=descI2->getConstPointer();

    DataArrayDouble *baryCellF = m2->getBarycenterAndOwner() ;

    MEDCouplingFieldDouble* fieldn=m2->buildOrthogonalField();
    DataArrayDouble *normal = fieldn->getArray();
    const double *tmpNormal=normal->getConstPointer();

    _numberOfCells = mu->getNumberOfCells() ;
    _cells    = new Cell[_numberOfCells] ;

    int k=0;
    for( int id=0;id<_numberOfCells;id++ )
    {
        const int *work=tmp+tmpI[id];
        const int *work2=tmp2+tmpI2[id];
        int nbFaces=tmpI[id+1]-tmpI[id];
        int nbVertices=mu->getNumberOfNodesInCell(id) ;

        double coorBaryY=0.;
        double coorBaryZ=0.;
        double coef=0;
        double coorBaryX=coorBary[k];
        if (_dim==2)
        	coorBaryY = coorBary[k+1];
        if (_dim==3)
        	coorBaryZ = coorBary[k+2];

		Point p(coorBaryX,coorBaryY,coorBaryZ) ;
        Cell ci( nbVertices, nbFaces, surf[id], p ) ;
        for( int el=0;el<nbFaces;el++ )
        {
            double xn=0.;
            double yn=0.;
            double zn=0.;
            if (work2[el]<0)
            {
                xn=-tmpNormal[_dim*work[el]];
                if (_dim==2)
					yn=-tmpNormal[_dim*work[el]+1];
                if (_dim==3)
                	zn=-tmpNormal[_dim*work[el]+2];
            }else
            {
                xn=tmpNormal[_dim*work[el]];
                if (_dim==2)
                	yn=tmpNormal[_dim*work[el]+1];
                if (_dim==3)
                	zn=tmpNormal[_dim*work[el]+2];
            }
            ci.addNormalVector(el,xn,yn,zn) ;
            ci.addFaceId(el,work[el]) ;
        }
        std::vector<int> nodeIdsOfCell ;
        mu->getNodeIdsOfCell(id,nodeIdsOfCell) ;
        for( int el=0;el<nbVertices;el++ )
            ci.addNodeId(el,nodeIdsOfCell[el]) ;
        _cells[id] = ci ;
		k+=_dim;
    }

    DataArrayInt *revNode=DataArrayInt::New();
    DataArrayInt *revNodeI=DataArrayInt::New();
    mu->getReverseNodalConnectivity(revNode,revNodeI) ;
    const int *tmpN=revNode->getConstPointer();
    const int *tmpNI=revNodeI->getConstPointer();

    DataArrayInt *revCell=DataArrayInt::New();
    DataArrayInt *revCellI=DataArrayInt::New();
    m2->getReverseNodalConnectivity(revCell,revCellI) ;
    const int *tmpC=revCell->getConstPointer();
    const int *tmpCI=revCellI->getConstPointer();

    _numberOfNodes = mu->getNumberOfNodes() ;
    _nodes    = new Node[_numberOfNodes] ;

    DataArrayDouble *coo = mu->getCoords() ;
    const double *cood=coo->getConstPointer();
    k=0;
    for( int id=0;id<_numberOfNodes;id++,k+=_dim)
    {
        double zc=0.;
        if (_dim==3)
        	zc=cood[k+2];
		Point p(cood[k],cood[k+1],zc) ;
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

    coo->decrRef();
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
    DataArrayDouble *normalFaces1 = m2->buildOrthogonalField()->getArray() ;
    const double *normalFaces2=normalFaces1->getConstPointer();

    k=0;
    for(int id=0;id<_numberOfFaces;id++)
    {
    	double coorBarySegX=coorBarySeg[k];
    	double coorBarySegY=0.;
    	double coorBarySegZ=0.;
        if (_dim==2)
        {
        	coorBarySegY = coorBarySeg[k+1];
        }
        if (_dim==3)
        {
        	coorBarySegZ = coorBarySeg[k+2];
        }
        Point p(coorBarySegX,coorBarySegY,coorBarySegZ) ;
        const int *workc=tmpA+tmpAI[id];
        int nbCells=tmpAI[id+1]-tmpAI[id];

        const int *workv=tmpNE+tmpNEI[id]+1;
        Face fi( 2, nbCells, lon[id], p, normalFaces2[k], normalFaces2[k+1], 0.0) ;
        fi.addNodeId(0,workv[0]) ;
        fi.addNodeId(1,workv[1]) ;

        fi.addCellId(0,workc[0]) ;
        if (nbCells==2)
			fi.addCellId(1,workc[1]) ;

        _faces[id] = fi ;
        k+=_dim;
    }
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
    mu->decrRef();
}

//----------------------------------------------------------------------
Mesh::Mesh( double xinf, double xsup, int nx )
//----------------------------------------------------------------------
{
    // il manque test xsup siinf
    double dx = (xsup - xinf)/nx ;

    _dim = 1 ;
    _xMin=xinf;
    _xSup=xsup;
    _yMin=0.;
    _ySup=0.;
    _zMin=0.;
    _zSup=0.;

    _dxyz.resize(_dim);
	_dxyz[0]=dx;
	_nxyz.resize(_dim);
	_nxyz[0]=nx+1;

    double *originPtr = new double[_dim];
    double *dxyzPtr = new double[_dim];
    int *nodeStrctPtr = new int[_dim];

	originPtr[0]=xinf;
	nodeStrctPtr[0]=nx+1;
	dxyzPtr[0]=dx;


    _mesh=MEDCouplingIMesh::New("MESH1D",
    						    _dim,
    							nodeStrctPtr,
    							nodeStrctPtr+_dim,
    							originPtr,
    							originPtr+_dim,
    							dxyzPtr,
    							dxyzPtr+_dim);
    delete [] originPtr;
    delete [] dxyzPtr;
    delete [] nodeStrctPtr;

    DataArrayInt *desc=DataArrayInt::New();
    DataArrayInt *descI=DataArrayInt::New();
    DataArrayInt *revDesc=DataArrayInt::New();
    DataArrayInt *revDescI=DataArrayInt::New();
    MEDCouplingUMesh* mu=_mesh->buildUnstructured();
    MEDCouplingUMesh *m2=mu->buildDescendingConnectivity(desc,descI,revDesc,revDescI);
    m2->setName(mu->getName());

    DataArrayDouble *baryCell = mu->getBarycenterAndOwner() ;
    const double *coorBary=baryCell->getConstPointer();

    _numberOfCells = _mesh->getNumberOfCells() ;
    _cells    = new Cell[_numberOfCells] ;

    MEDCouplingFieldDouble* fieldl=mu->getMeasureField(true);
    DataArrayDouble *longueur = fieldl->getArray();
    const double *lon=longueur->getConstPointer();

    int comp=0;
    for( int id=0;id<_numberOfCells;id++ )
    {
        int nbVertices=mu->getNumberOfNodesInCell(id) ;
        Point p(coorBary[id],0.0,0.0) ;
        Cell ci( nbVertices, nbVertices, lon[id], p ) ;

        std::vector<int> nodeIdsOfCell ;
        mu->getNodeIdsOfCell(id,nodeIdsOfCell) ;
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
    mu->getReverseNodalConnectivity(revNode,revNodeI) ;
    const int *tmpN=revNode->getConstPointer();
    const int *tmpNI=revNodeI->getConstPointer();

    _numberOfNodes = mu->getNumberOfNodes() ;
    _nodes    = new Node[_numberOfNodes] ;
    _numberOfFaces = _numberOfNodes;
    _faces    = new Face[_numberOfFaces] ;

    for( int id=0;id<_numberOfNodes;id++ )
    {
        std::vector<double> coo ;
        mu->getCoordinatesOfNode(id,coo);
        Point p(coo[0],0.0,0.0) ;
        const int *workc=tmpN+tmpNI[id];
        int nbCells=tmpNI[id+1]-tmpNI[id];
        int nbFaces=1;
        Node vi( nbCells, nbFaces, p ) ;
        int nbVertices=1;
        /* provisoire !!!!!!!!!!!!*/
        Point pf(0.0,0.0,0.0) ;
        Face fi( nbVertices, nbCells, 0.0, pf, 0., 0., 0. ) ;

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
    m2->decrRef();
    mu->decrRef();
}

//----------------------------------------------------------------------
Mesh::Mesh( double xinf, double xsup, int nx, double yinf, double ysup, int ny)
//----------------------------------------------------------------------
{

// Anouar a faire
//  if ( xinf >= xsup and yinf >= ysup )
// exit probleme
    _xMin=xinf;
    _xSup=xsup;
    _yMin=yinf;
    _ySup=ysup;
    _zMin=0.;
    _zSup=0.;


    double dx = (xsup - xinf)/nx ;
    double dy = (ysup - yinf)/ny ;

    _dim = 2 ;
	_nxyz.resize(_dim);
	_nxyz[0]=nx+1;
	_nxyz[1]=ny+1;

    _dxyz.resize(_dim);
	_dxyz[0]=dx;
	_dxyz[1]=dy;

	double *originPtr = new double[_dim];
    double *dxyzPtr = new double[_dim];
    int *nodeStrctPtr = new int[_dim];

	originPtr[0]=xinf;
	originPtr[1]=yinf;
	nodeStrctPtr[0]=nx+1;
	nodeStrctPtr[1]=ny+1;
	dxyzPtr[0]=dx;
	dxyzPtr[1]=dy;


    _mesh=MEDCouplingIMesh::New("MESH2D",
    						    _dim,
    							nodeStrctPtr,
    							nodeStrctPtr+_dim,
    							originPtr,
    							originPtr+_dim,
    							dxyzPtr,
    							dxyzPtr+_dim);
    delete [] originPtr;
    delete [] dxyzPtr;
    delete [] nodeStrctPtr;
    _groups.resize(0);
    setMesh();
}

//----------------------------------------------------------------------
Mesh::Mesh( double xinf, double xsup, int nx, double yinf, double ysup, int ny, double zinf, double zsup, int nz)
//----------------------------------------------------------------------
{
    // Anouar a faire
    //  if ( xinf >= xsup and yinf >= ysup )
    // exit probleme

	_dim=3;
    _xMin=xinf;
    _xSup=xsup;
    _yMin=yinf;
    _ySup=ysup;
    _zMin=zinf;
    _zSup=zsup;

    double dx = (xsup - xinf)/nx ;
    double dy = (ysup - yinf)/ny ;
    double dz = (zsup - zinf)/nz ;

    _dxyz.resize(_dim);
	_dxyz[0]=dx;
	_dxyz[1]=dy;
	_dxyz[2]=dz;

	_nxyz.resize(_dim);
	_nxyz[0]=nx+1;
	_nxyz[1]=ny+1;
	_nxyz[2]=nz+1;

	double *originPtr = new double[_dim];
    double *dxyzPtr = new double[_dim];
    int *nodeStrctPtr = new int[_dim];

	originPtr[0]=xinf;
	originPtr[1]=yinf;
	originPtr[2]=zinf;
	nodeStrctPtr[0]=nx+1;
	nodeStrctPtr[1]=ny+1;
	nodeStrctPtr[2]=nz+1;
	dxyzPtr[0]=dx;
	dxyzPtr[1]=dy;
	dxyzPtr[2]=dz;


    _mesh=MEDCouplingIMesh::New("MESH3D",
    						    _dim,
    							nodeStrctPtr,
    							nodeStrctPtr+_dim,
    							originPtr,
    							originPtr+_dim,
    							dxyzPtr,
    							dxyzPtr+_dim);
    delete [] originPtr;
    delete [] dxyzPtr;
    delete [] nodeStrctPtr;
    _groups.resize(0);
    setMesh();
}

//----------------------------------------------------------------------
int
Mesh::getSpaceDimension( void )  const
//----------------------------------------------------------------------
{
    return _dim ;
}

std::vector<double>
Mesh::getDXYZ() const
{
	return _dxyz;
}

std::vector<int>
Mesh::getCellGridStructure() const
{
	return _nxyz;
}

//----------------------------------------------------------------------
int
Mesh::getNx( void )  const
//----------------------------------------------------------------------
{
    return _nxyz[0]-1 ;
}

//----------------------------------------------------------------------
int
Mesh::getNy( void )  const
//----------------------------------------------------------------------
{
 	if(_dim < 2)
	    throw CdmathException("int double& Field::operator[ielem] : Ny is not defined in dimension < 2!");
    return _nxyz[1]-1 ;
}

//----------------------------------------------------------------------
int
Mesh::getNz( void )  const
//----------------------------------------------------------------------
{
 	if(_dim < 3)
	    throw CdmathException("int double& Field::operator[ielem] : Nz is not defined in dimension < 3!");
    return _nxyz[2]-1 ;
}

//----------------------------------------------------------------------
double
Mesh::getXMin( void )  const
//----------------------------------------------------------------------
{
    return _xMin ;
}

//----------------------------------------------------------------------
double
Mesh::getXSup( void )  const
//----------------------------------------------------------------------
{
    return _xSup ;
}

//----------------------------------------------------------------------
double
Mesh::getYMin( void )  const
//----------------------------------------------------------------------
{
    return _yMin ;
}

//----------------------------------------------------------------------
double
Mesh::getYSup( void )  const
//----------------------------------------------------------------------
{
    return _ySup ;
}

//----------------------------------------------------------------------
double
Mesh::getZMin( void )  const
//----------------------------------------------------------------------
{
    return _zMin ;
}

//----------------------------------------------------------------------
double
Mesh::getZSup( void )  const
//----------------------------------------------------------------------
{
    return _zSup ;
}

//----------------------------------------------------------------------
MEDCouplingAutoRefCountObjectPtr<MEDCouplingMesh>
Mesh::getMEDCouplingMesh( void )  const
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
    _dim = mesh.getSpaceDimension() ;
    _numberOfNodes = mesh.getNumberOfNodes();
    _numberOfFaces = mesh.getNumberOfFaces();
    _numberOfCells = mesh.getNumberOfCells();
    _xMin=mesh.getXMin();
    _xSup=mesh.getXSup();
    _yMin=mesh.getYMin();
    _ySup=mesh.getYSup();
    _zMin=mesh.getZMin();
    _zSup=mesh.getZSup();
    _groups = mesh.getNamesOfGroups() ;
    _nxyz = mesh.getCellGridStructure() ;

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

    MEDCouplingAutoRefCountObjectPtr<MEDCouplingMesh> m1=mesh.getMEDCouplingMesh()->deepCpy();
    _mesh=m1;
    return *this;
}

//----------------------------------------------------------------------
void
Mesh::writeVTK ( const string fileName ) const
//----------------------------------------------------------------------
{
	string fname=fileName+".vtu";
	_mesh->writeVTK(fname.c_str()) ;
}

//----------------------------------------------------------------------
void
Mesh::writeMED ( const string fileName ) const
//----------------------------------------------------------------------
{
	string fname=fileName+".med";
	MEDCouplingUMesh* mu=_mesh->buildUnstructured();
	MEDLoader::WriteUMesh(fname.c_str(),mu,true);
	mu->decrRef();
}
