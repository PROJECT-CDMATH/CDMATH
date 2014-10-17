/*
 * GenericSolver.cpp
 *
 *  Created on: 3 juil. 2014
 *      Author: mekkas
 */

#include "GenericSolver.hxx"
#include "CdmathException.hxx"


GenericSolver::GenericSolver(double finalTime, int maximumNumberbOfIter, bool isAlternatingDirection, int numberOfGhostCells, int frequencyOfPostTreatment)
{
	_isAlternatingDirection=isAlternatingDirection;
	_finalTime=finalTime;
	_maximumNumberbOfIter=maximumNumberbOfIter;
	_numberOfGhostCells=numberOfGhostCells;
	_frequencyOfPostTreatment=frequencyOfPostTreatment;
}

GenericSolver::GenericSolver()
{
	_isAlternatingDirection=false;
	_finalTime=0.;
	_maximumNumberbOfIter=0;
	_numberOfGhostCells=1;
	_frequencyOfPostTreatment=0;
}

GenericSolver::~GenericSolver()
{
}


GenericSolver::GenericSolver( const GenericSolver& genericSolver )
{
	_isAlternatingDirection=genericSolver.isAlternatingDirection();
	_finalTime=genericSolver.getFinalTime();
	_maximumNumberbOfIter=genericSolver.getMaximumNumberbOfIter();
	_numberOfGhostCells=genericSolver.getNumberOfGhostCells();
	_frequencyOfPostTreatment=genericSolver.getFrequencyOfPostTreatment();
}

//----------------------------------------------------------------------
const GenericSolver&
GenericSolver::operator= ( const GenericSolver& genericSolver )
//----------------------------------------------------------------------
{
	_isAlternatingDirection=genericSolver.isAlternatingDirection();
	_finalTime=genericSolver.getFinalTime();
	_maximumNumberbOfIter=genericSolver.getMaximumNumberbOfIter();
	_numberOfGhostCells=genericSolver.getNumberOfGhostCells();
	_frequencyOfPostTreatment=genericSolver.getFrequencyOfPostTreatment();
	return *this;
}

int
GenericSolver::getFrequencyOfPostTreatment( void ) const
{
	return _frequencyOfPostTreatment;
}

void
GenericSolver::setFrequencyOfPostTreatment( int frequencyOfPostTreatment )
{
	_frequencyOfPostTreatment=frequencyOfPostTreatment;
}

bool
GenericSolver::isAlternatingDirection( void ) const
{
	return _isAlternatingDirection;
}

void
GenericSolver::setAlternatingDirection( void )
{
	_isAlternatingDirection=true;
}

void
GenericSolver::setMaximumNumberbOfIter( int maximumNumberbOfIter )
{
	_maximumNumberbOfIter=maximumNumberbOfIter;
}

void
GenericSolver::setFinalTime( double finalTime )
{
	_finalTime=finalTime;
}

int
GenericSolver::getMaximumNumberbOfIter( void ) const
{
	return _maximumNumberbOfIter;
}

double
GenericSolver::getFinalTime( void ) const
{
	return _finalTime;
}

void
GenericSolver::setNumberOfGhostCells( int numberOfGhostCells )
{
	_numberOfGhostCells=numberOfGhostCells;
}

int
GenericSolver::getNumberOfGhostCells( void ) const
{
	return _numberOfGhostCells;
}
