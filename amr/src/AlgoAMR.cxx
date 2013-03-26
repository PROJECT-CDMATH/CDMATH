/*
 * AlgoAMR.cxx
 *
 *  Created on: 4 fvr. 2013
 *      Author: mekkas
 */

#ifndef ALGOAMR_CXX_
#define ALGOAMR_CXX_

#include "AlgoAMR.hxx"

using namespace std;

AlgoAMR::AlgoAMR()
{
	_maxLevel=0;
	_verbose=true;
}

AlgoAMR::~AlgoAMR()
{
}

AlgoAMR::AlgoAMR(const int maxLevel,
				 const IntTab coefRefinementX,
				 const IntTab coefRefinementY,
				 const DoubleTab efficiency,
				 const DoubleTab efficiencySnd,
	             const IntTab minCellDirection,
	             const IntTab maxCells )
{
	define(maxLevel,coefRefinementX,coefRefinementY,efficiency,efficiencySnd,minCellDirection, maxCells);
}

void
AlgoAMR::define(const int maxLevel,
		        const IntTab coefRefinementX,
		        const IntTab coefRefinementY,
                const DoubleTab efficiency,
                const DoubleTab efficiencySnd,
                const IntTab minCellDirection,
                const IntTab maxCells )
{
    _efficiency = efficiency ;
    _efficiencySnd = efficiencySnd ;
    _verbose=true;
    _minCellDirection = minCellDirection ;
    _maxCells = maxCells ;
    _maxLevel = maxLevel ;
    _coefRefinementX = coefRefinementX ;
    _coefRefinementY = coefRefinementY ;
}

const DoubleTab&
AlgoAMR::getEfficiency(void) const
{
	return _efficiency;
}

double
AlgoAMR::getEfficiency(const int level) const
{
	return _efficiency[level];
}

const DoubleTab&
AlgoAMR::getEfficiencySnd(void) const
{
	return _efficiencySnd;
}
double
AlgoAMR::getEfficiencySnd(const int level) const
{
	return _efficiencySnd[level];
}

bool
AlgoAMR::isVerbose() const
{
	return _verbose;
}

int
AlgoAMR::getMinCellDirection(const int level) const
{
	return _minCellDirection[level];
}

const IntTab&
AlgoAMR::getMinCellDirection(void) const
{
	return _minCellDirection;
}

int
AlgoAMR::getMaxCells(const int level) const
{
	return _maxCells[level];
}

const IntTab&
AlgoAMR::getMaxCells(void) const
{
	return _maxCells;
}


const IntTab&
AlgoAMR::getCoefRefinementX() const
{
	return _coefRefinementX;
}

const IntTab&
AlgoAMR::getCoefRefinementY() const
{
	return _coefRefinementY;
}

vector<Grid>
AlgoAMR::getGrids() const
{
	return _grids;
}

Grid
AlgoAMR::getGrid(const int index) const
{
	return _grids[index];
}

void
AlgoAMR::addGrid(Grid grid)
{
	return _grids.push_back(grid);
}

int
AlgoAMR::getMaxLevel ( void ) const
{
	return _maxLevel;
}

unsigned int
AlgoAMR::getNumberOfGrids ( void ) const
{
	return _grids.size();
}

//----------------------------------------------------------------------
AlgoAMR::AlgoAMR( const AlgoAMR & algoAMR )
//----------------------------------------------------------------------
{
    _efficiency = algoAMR.getEfficiency();
    _efficiencySnd = algoAMR.getEfficiencySnd() ;
    _verbose=algoAMR.isVerbose();
    _minCellDirection =algoAMR.getMinCellDirection();
    _maxCells =algoAMR.getMaxCells() ;
	_maxLevel=algoAMR.getMaxLevel();
    _coefRefinementX = algoAMR.getCoefRefinementX() ;
    _coefRefinementY = algoAMR.getCoefRefinementY() ;

	for (unsigned int i=0;i<algoAMR.getNumberOfGrids();i++)
		_grids.push_back(algoAMR.getGrid(i));
}


const AlgoAMR&
AlgoAMR::operator= ( const AlgoAMR& algoAMR)
{
    _efficiency = algoAMR.getEfficiency();
    _efficiencySnd = algoAMR.getEfficiencySnd() ;
    _verbose=algoAMR.isVerbose();
    _minCellDirection =algoAMR.getMinCellDirection();
    _maxCells =algoAMR.getMaxCells() ;
	_maxLevel=algoAMR.getMaxLevel();
    _coefRefinementX = algoAMR.getCoefRefinementX() ;
    _coefRefinementY = algoAMR.getCoefRefinementY() ;
	for (unsigned int i=0;i<algoAMR.getNumberOfGrids();i++)
		_grids.push_back(algoAMR.getGrid(i));
	return *this;
}

void
AlgoAMR::writeVTK(const string fileName ) const
{
	for (unsigned int i=0;i<getNumberOfGrids();i++)
		getGrid(i).writeVTK(fileName);
}
#endif /* ALGOAMR_CXX_ */
