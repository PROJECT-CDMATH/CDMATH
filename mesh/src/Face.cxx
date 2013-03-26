/*
 * face.cxx
 *
 *  Created on: 23 janv. 2012
 *      Authors: CDMAT
 */

#include "Face.hxx"
#include <cmath>

using namespace std;

//----------------------------------------------------------------------
Face::Face( void )
//----------------------------------------------------------------------
{
	_measure = 0.0 ;
	_region=-1;
	_groupName="";
	_numberOfCells = 0 ;
	_numberOfNodes = 0 ;
}

//----------------------------------------------------------------------
Face::Face( const Face& face )
//----------------------------------------------------------------------
{
	_measure = face.getMeasure() ;
	_region=face.getRegion();
	_groupName=face.getGroupName();
	_point = face.getBarryCenter();
	_numberOfCells = face.getNumberOfCells() ;
	_numberOfNodes = face.getNumberOfNodes() ;
	_nodesId=face.getNodesId();
	_cellsId=face.getCellsId();
}

//----------------------------------------------------------------------
Face::Face( const int numberOfNodes, const int numberOfCells, const double measure, const Point p )
//----------------------------------------------------------------------
{
	_point        = p ;
	_numberOfNodes = numberOfNodes ;
	_numberOfCells   = numberOfCells ;
	_nodesId = IntTab(_numberOfNodes,0);
	_cellsId = IntTab(_numberOfCells,0);
	_measure = measure ;
	_region=-1;
	_groupName="";
}

//----------------------------------------------------------------------
Face::~Face( void )
//----------------------------------------------------------------------
{
}

int
Face::getRegion(void) const
{
	return _region;
}

string
Face::getGroupName(void) const
{
	return _groupName;
}

void
Face::setGroupName(const string groupName)
{
	_groupName=groupName;
	_region=0;
}

bool
Face::isBorder(void)
{
	if (_region==0)
		return true;
	else
		return false;
}
//----------------------------------------------------------------------
Point
Face::getBarryCenter( void ) const
//----------------------------------------------------------------------
{
	return _point ;
}

//----------------------------------------------------------------------
double
Face::x( void ) const
//----------------------------------------------------------------------
{
  return _point.x() ;
}

//----------------------------------------------------------------------
double
Face::y( void ) const
//----------------------------------------------------------------------
{
	return _point.y() ;
}

//----------------------------------------------------------------------
double
Face::z( void ) const
//----------------------------------------------------------------------
{
	return _point.z() ;
}

//----------------------------------------------------------------------
IntTab
Face::getCellsId( void ) const
//----------------------------------------------------------------------
{
	return _cellsId ;
}

//----------------------------------------------------------------------
IntTab
Face::getNodesId( void ) const
//----------------------------------------------------------------------
{
	return _nodesId ;
}

//----------------------------------------------------------------------
double
Face::getMeasure( void ) const
//----------------------------------------------------------------------
{
	return _measure ;
}

//----------------------------------------------------------------------
int
Face::getNumberOfCells( void ) const
//----------------------------------------------------------------------
{
	return _numberOfCells ;
}

//----------------------------------------------------------------------
int
Face::getNumberOfNodes( void ) const
//----------------------------------------------------------------------
{
	return _numberOfNodes ;
}

//----------------------------------------------------------------------
void
Face::addCellId(const int numCell, const int cellId )
//----------------------------------------------------------------------
{
	_cellsId[numCell] = cellId ;
}
//----------------------------------------------------------------------
void
Face::addNodeId(const int numNode, const int nodeId )
//----------------------------------------------------------------------
{
	_nodesId[numNode] = nodeId ;
}

//----------------------------------------------------------------------
const Face&
Face::operator= ( const Face& face )
//----------------------------------------------------------------------
{
	_measure = face.getMeasure() ;
	_point = face.getBarryCenter() ;
	_numberOfCells = face.getNumberOfCells() ;
	_numberOfNodes = face.getNumberOfNodes() ;
	_nodesId=face.getNodesId();
	_cellsId=face.getCellsId();
	_groupName=face.getGroupName();
	_region=face.getRegion();
	return *this;
}
