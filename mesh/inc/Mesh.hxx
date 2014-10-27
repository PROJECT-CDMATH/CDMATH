/*
 * mesh.hxx
 *
 *  Created on: 22 janv. 2012
 *      Authors: CDMAT
 */

#ifndef MESH_HXX_
#define MESH_HXX_

/**
 * Mesh class is defined by
 * - case 1: file name of mesh med file
 * - case 2: 1D : xmin and xmax and number of cells
 * - case 3: 2D : xmin, xmax, ymin and ymax and numbers of cells in x direction and y direction
 * - case 4 (not yet): 3D : xmin, xmax, ymin and ymax and numbers of cells in x direction, y direction and z direction
 */

namespace ParaMEDMEM
{
  class MEDFileUMesh;
  class MEDCouplingMesh;
  class MEDCouplingIMesh;
}

#include <MEDCouplingAutoRefCountObjectPtr.hxx>

class Node;
class Cell;
class Face;
class IntTab;

#include <vector>
#include <string>

class Mesh
{

    public: //----------------------------------------------------------------
    /**
     * default constructor
     */
    Mesh ( void ) ;

    /**
     * constructor with data
     * @param filename name of mesh file
     */
    Mesh ( const std::string filename ) ;

    /**
     * constructor with data
     * @param xinf : minimum x
     * @param xsup : maximum x
     * @param nx : Number of cell in x direction
     */
    Mesh( double xinf, double xsup, int nx ) ;

    /**
     * constructor with data
     * @param xinf : minimum x
     * @param xsup : maximum x
     * @param yinf : minimum y
     * @param ysup : maximum y
     * @param nx : Number of cell in x direction
     * @param ny : Number of cell in y direction
     */
    Mesh( double xinf, double xsup, int nx, double yinf, double ysup, int ny) ;

    /**
     * constructor with data
     * @param xinf : minimum x
     * @param xsup : maximum x
     * @param yinf : minimum y
     * @param ysup : maximum y
     * @param zinf : minimum z
     * @param zsup : maximum z
     * @param nx : Number of cell in x direction
     * @param ny : Number of cell in y direction
     * @param nz : Number of cell in z direction
     */
    Mesh( double xinf, double xsup, int nx, double yinf, double ysup, int ny, double zinf, double zsup, int nz) ;

    Mesh( const ParaMEDMEM::MEDCouplingIMesh* mesh ) ;

    /**
     * constructor with data
     * @param filename : file name of mesh med file
     */
    void readMeshMed( const std::string filename ) ;

    /**
     * constructor by copy
     * @param mesh : The Mesh object to be copied
     */
    Mesh ( const Mesh & mesh ) ;

    /**
    * destructor
    */
    ~Mesh( void ) ;

    /**
     * return Space dimension
     * @return _dim
     */
    int getSpaceDimension( void ) const ;

    std::vector<double> getDXYZ() const ;

    std::vector<int> getCellGridStructure() const;
    /**
     * return The nodes in this mesh
     * @return _nodes
    */
    Node* getNodes ( void ) const ;

    /**
     * return The cells in this mesh
     * @return _vertices
     */
    Cell* getCells ( void ) const ;

    /**
     * return The faces in this mesh
     * @return _vertices
     */
    Face* getFaces ( void ) const ;

    /**
     * return number of nodes in this mesh
     * @return _numberOfNodes
     */
    int getNumberOfNodes ( void )  const ;

    /**
     * return number of faces in this mesh
     * @return _numberOfFaces
     */
    int getNumberOfFaces ( void )  const ;

    /**
     * return number of cells in this mesh
     * @return _numberOfCells
     */
    int getNumberOfCells ( void )  const ;

    /**
     * return The cell i in this mesh
     * @return _cells[i]
     */
    Cell& getCell ( int i )  const ;

    /**
     * return The face i in this mesh
     * @return _faces[i]
     */
    Face& getFace ( int i )  const ;

    /**
     * return The node i in this mesh
     * @return _nodes[i]
     */
    Node& getNode ( int i )  const ;

    /**
     * return number of cell in x direction
     * return _nX
     */
    int getNx( void )  const ;

    /**
     * return number of cell in y direction
     * return _nY
     */
    int getNy( void )  const ;

    /**
     * return number of cell in z direction
     * return _nZ
     */
    int getNz( void )  const ;

    double getXMin( void )  const ;

    double getXSup( void )  const ;

    double getYMin( void )  const ;

    double getYSup( void )  const ;

    double getZMin( void )  const ;

    double getZSup( void )  const ;

    /**
     * surcharge operator =
     * @param mesh : The Mesh object to be copied
     */
    const Mesh& operator= ( const Mesh& mesh ) ;

    /**
     * return the mesh MEDCoupling
     * return _mesh
     */
    ParaMEDMEM::MEDCouplingAutoRefCountObjectPtr<ParaMEDMEM::MEDCouplingMesh> getMEDCouplingMesh ( void )  const ;

    /**
     * return the mesh MEDCoupling
     * return _mesh
     */
    std::vector<std::string> getNamesOfGroups( void )  const ;

    /**
     * write mesh in the VTK format
     */
    void writeVTK ( const std::string fileName ) const ;

    /**
     * write mesh in the MED format
     */
    void writeMED ( const std::string fileName ) const ;

    void setGroupAtPlan(double value, int direction, double eps, std::string groupName) ;

    void setGroupAtFaceByCoords(double x, double y, double z, double eps, std::string groupName) ;

    int getIndexFacePeriodic(int indexFace) const ;

    IntTab getIndexFacePeriodic( void ) const ;

    private: //----------------------------------------------------------------

    void setMesh( void ) ;

    void setGroups( const ParaMEDMEM::MEDFileUMesh* medmesh) ;

    /**
     * Space dimension
     */
    int _dim ;

    double _xMin;

    double _xSup;

    double _yMin;

    double _ySup;

    double _zMin;

    double _zSup;

    std::vector<int> _nxyz;

    std::vector<double> _dxyz;
    /*
     * The nodes in this mesh.
     */
    Node *_nodes;

    /*
     * The number of nodes in this mesh.
     */
    int _numberOfNodes;

    /*
    * The faces in this mesh.
    */
    Face *_faces;

    /*
     * The numbers of faces in this mesh.
     */
    int _numberOfFaces;

    /*
     * The cells in this mesh.
     */
    Cell *_cells;

    /*
     * The number of cells in this mesh.
     */
    int _numberOfCells;

    /*
     * The names of groups.
     */
    std::vector<std::string> _groups;

    /*
     * The mesh MEDCoupling
     */
    ParaMEDMEM::MEDCouplingAutoRefCountObjectPtr<ParaMEDMEM::MEDCouplingMesh> _mesh;
};

#endif /* MESH_HXX_ */
