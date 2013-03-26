/*
 * node.hxx
 *
 *  Created on: 23 janv. 2012
 *      Authors: CDMAT
 */

#ifndef NODE_HXX_
#define NODE_HXX_

/**
 * Node class is defined by
 * - The coordinate of the node
 * - number of faces allocated for this node
 * - number of cells allocated for this node
 */

#include "Point.hxx"
#include "IntTab.hxx"

class Node
{
    public: //----------------------------------------------------------------
	/**
	* default constructor
	*/
	Node ( void ) ;

	/**
	 * constructor with data
	 * @param numberOfCells : The number of cells allocated for this node
	 * @param numberOfFaces : The number of faces allocated for this node
	 * @param p : The barycenter of this node
	 */
	Node ( const int numberOfCells, const int numberOfFaces, const Point p ) ;

	/**
	 * constructor by copy
	 * @param node : The object to be copied
	 */
	Node ( const Node & node ) ;

	/**
	 * destructor
	 */
	~Node ( void ) ;

	/**
	 * The cells ID that this Node belongs to
	 * @return _cellsId
	 */
	IntTab getCellsId ( void ) const ;

	/**
	 * The faces ID that this Node belongs to
	 * @return _facesId
	 */
	IntTab getFacesId ( void ) const ;

	/**
	 * return number of cells in this cell
	 * @return _numberOfCells
	 */
	int getNumberOfCells ( void ) const ;

	/**
	 * return number of faces in this cell
	 * @return _numberOfFaces
	 */
	int getNumberOfFaces ( void ) const ;

      /**
       * return The coordinate of the Node
       * @return _point
       */
  	/**
  	 * return The coordinate of the Node
  	 * @return _point
  	 */
	Point getPoint ( void ) const ;

  	/**
  	 * return cordinate x of the barycenter in this face
  	 */
	double x ( void ) const ;

  	/**
  	 * return cordinate x of the barycenter in this face
  	 */
	double y ( void ) const ;

	/**
	 * return cordinate x of the barycenter in this face
	 */
	double z ( void ) const ;

	/**
	 * @param numFace : index local of face to add in this node
	 * @param faceId : index global of face to add in this node
	 */
	void addFaceId (const int numFace, const int faceId ) ;

	/**
	 * @param numCell : index local of cell to add in this node
	 * @param cellId : index global of cell to add in this node
	 */
	void addCellId (const int numCell, const int CellId ) ;

	/**
	 * surcharge opertor =
	 * @param node : The node object to be copied
	 */
	const Node& operator= ( const Node& node ) ;

	/**
	 * return Compute distance to given node
	 * @param n : Node
	 * @return The distance
	 */
	double distance( const Node& n ) const ;

    private: //----------------------------------------------------------------

	/*
	 * The coordinate of the Node.
	 */
	Point _point ;

	/*
	 * The cells ID that this Node belongs to.
	 */
	IntTab _cellsId ;

	/*
	 * The faces ID that this Node belongs to.
	 */
	IntTab _facesId ;

	/*
	 * The number of cells allocated for this Node.
	 */
	int _numberOfCells ;

	/*
	 * The number of faces allocated for this Node.
	 */
	int _numberOfFaces ;

};

#endif /* NODe_HXX_ */
