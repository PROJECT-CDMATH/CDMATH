/*
 * IterativeProblem.cpp
 *
 *  Created on: 3 juil. 2014
 *      Author: mekkas
 */

#include "CdmathException.hxx"
#include "IterativeProblem.hxx"


IterativeProblem::IterativeProblem(double finalTime, int maximumNumberbOfIter, bool isAlternatingDirection, int numberOfGhostCells, int frequencyOfPostTreatment)
{
    _isAlternatingDirection=isAlternatingDirection;
    _finalTime=finalTime;
    _maximumNumberbOfIter=maximumNumberbOfIter;
    _numberOfGhostCells=numberOfGhostCells;
    _frequencyOfPostTreatment=frequencyOfPostTreatment;
}

IterativeProblem::IterativeProblem()
{
    _isAlternatingDirection=false;
    _finalTime=0.;
    _maximumNumberbOfIter=0;
    _numberOfGhostCells=1;
    _frequencyOfPostTreatment=0;
}

IterativeProblem::~IterativeProblem()
{
}


IterativeProblem::IterativeProblem( const IterativeProblem& IterativeProblem )
{
    _isAlternatingDirection=IterativeProblem.isAlternatingDirection();
    _finalTime=IterativeProblem.getFinalTime();
    _maximumNumberbOfIter=IterativeProblem.getMaximumNumberbOfIter();
    _numberOfGhostCells=IterativeProblem.getNumberOfGhostCells();
    _frequencyOfPostTreatment=IterativeProblem.getFrequencyOfPostTreatment();
}

//----------------------------------------------------------------------
const IterativeProblem&
IterativeProblem::operator= ( const IterativeProblem& IterativeProblem )
//----------------------------------------------------------------------
{
    _isAlternatingDirection=IterativeProblem.isAlternatingDirection();
    _finalTime=IterativeProblem.getFinalTime();
    _maximumNumberbOfIter=IterativeProblem.getMaximumNumberbOfIter();
    _numberOfGhostCells=IterativeProblem.getNumberOfGhostCells();
    _frequencyOfPostTreatment=IterativeProblem.getFrequencyOfPostTreatment();
    return *this;
}

int
IterativeProblem::getFrequencyOfPostTreatment( void ) const
{
    return _frequencyOfPostTreatment;
}

void
IterativeProblem::setFrequencyOfPostTreatment( int frequencyOfPostTreatment )
{
    _frequencyOfPostTreatment=frequencyOfPostTreatment;
}

bool
IterativeProblem::isAlternatingDirection( void ) const
{
    return _isAlternatingDirection;
}

void
IterativeProblem::setAlternatingDirection( void )
{
    _isAlternatingDirection=true;
}

void
IterativeProblem::setMaximumNumberbOfIter( int maximumNumberbOfIter )
{
    _maximumNumberbOfIter=maximumNumberbOfIter;
}

void
IterativeProblem::setFinalTime( double finalTime )
{
    _finalTime=finalTime;
}

int
IterativeProblem::getMaximumNumberbOfIter( void ) const
{
    return _maximumNumberbOfIter;
}

double
IterativeProblem::getFinalTime( void ) const
{
    return _finalTime;
}

void
IterativeProblem::setNumberOfGhostCells( int numberOfGhostCells )
{
    _numberOfGhostCells=numberOfGhostCells;
}

int
IterativeProblem::getNumberOfGhostCells( void ) const
{
    return _numberOfGhostCells;
}
