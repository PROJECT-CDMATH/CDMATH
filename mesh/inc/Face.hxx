/*
 * face.hxx
 *
 *  Created on: 23 janv. 2012
 *      Authors: CDMAT
 */

#ifndef FACE_HXX_
#define FACE_HXX_

/**
 * face class is defined by
 * - number of nodes allocated for this face
 * - number of cells allocated for this face
 * - measure of this face
 * - barycenter of this face
 */

#include "IntTab.hxx"
#include "Point.hxx"

#include <string>

class Face
{
    public: //----------------------------------------------------------------
	/**
	 * default constructor
	 */
	Face ( void ) ;

	/**
	 * constructor by copy
	 * @param face : The object to be copied
	 */
	Face ( const Face& face ) ;

	/**
	 * constructor with data
	 * @param numberOfNodes : The number of nodes allocated for this face
	 * @param numberOfCells : The number of cells allocated for this face
	 * @param measure : The measure of this cell
	 * @param p : The barycenter of this cell
	 */
	Face( const int numberOfNodes, const int numberOfCells, const double measure, const Point p ) ;

	/**
	 * destructor
	 */
	~Face ( void ) ;

	/**
	 * The cells ID that this face belongs to
	 * @return _cellsId
	 */
	IntTab getCellsId ( void ) const ;

	/**
	 * The nodes ID that this face belongs to
	 * @return _nodesId
	 */
	IntTab getNodesId ( void ) const ;

	/**
	 * return the measure of this face (length in 2D or
	 * or surface in 3D
	 * @return _measure
	 */
	double getMeasure( void ) const ;

	/**
	 * return number of cells in this face
	 * @return _numberOfCells
	 */
	int getNumberOfCells ( void ) const ;

  	/**
  	 * return number of nodes in this face
  	 * @return _numberOfNodes
  	 */
	int getNumberOfNodes ( void ) const ;

  	/**
  	 * return barrycenter of this face
  	 * @return _point
  	 */
	Point getBarryCenter( void ) const ;

  	/**
  	 * return cordinate x of the barycenter in this face
  	 */
	double x ( void ) const ;

  	/**
  	 * return cordinate y of the barycenter in this face
  	 */
	double y ( void ) const ;

	/**
	 * return cordinate z of the barycenter in this face
	 */
	double z ( void ) const ;

	/**
	 * return groupe name of this face
	 */
	std::string getGroupName(void) const;

	/**
	 * @param groupName : groupe name to set of this face
	 */
	void setGroupName(const std::string groupName);

	/**
	 * return 0 if the face is on the border of domain
	 * else -1
	 */
	int getRegion(void) const ;

	/**
	 * return True if the face is on the border of domain
	 * else False
	 */
	bool isBorder(void) ;

	/**
	 * @param numCell : index local of cell to add in this face
	 * @param cellId : index global of cell to add in this face
	 */
	void addCellId (const int numCell, const int cellId ) ;

	/**
	 * @param numNode : index local of node to add in this face
	 * @param nodeId : index global of node to add in this face
	 */
	void addNodeId (const int numNode, const int nodeId ) ;

	/**
	 * surcharge opertor =
	 * @param face : The face object to be copied
	 */
	const Face& operator= ( const Face& face ) ;

    private: //----------------------------------------------------------------

	/*
	 * The cell id that this face belongs to.
	 */
	IntTab _cellsId ;

	/*
	 * The vertex id that this face belongs to.
	 */
	IntTab _nodesId ;

	/*
	 * The number of cells allocated for this face.
	 */
	int _numberOfCells ;

	/*
	 * The number of nodes allocated for this face.
	 */
	int _numberOfNodes ;

	/*
	 * The length of this face.
	 */
	double _measure ;

	/*
	 * The coordinate of barycenter the cell.
	 */
	Point _point ;

	/*
	 * The region of this face. -1 internal or number of edge that this face belongs to
	 */
	int _region ;

	/*
	 * The group name of the cell.
	 */
	std::string _groupName ;
};

#endif /* FACE_HXX_ */
