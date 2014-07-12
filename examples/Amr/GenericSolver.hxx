/*
 * GenericSolver.hxx
 *
 *  Created on: 3 juil. 2014
 *      Author: mekkas
 */

#ifndef GENERICSOLVER_HXX_
#define GENERICSOLVER_HXX_

namespace ParaMEDMEM
{
	class MEDCouplingFieldDouble;
	class MEDCouplingIMesh;
	class MEDCouplingAMRAttribute;
	class MEDCouplingCartesianAMRPatchGen;
}

#include <MEDCouplingAutoRefCountObjectPtr.hxx>

class GenericSolver {

public:

	GenericSolver();

	GenericSolver(double finalTime, int maximumNumberbOfIter, bool isAlternatingDirection, int numberOfGhostCells, int frequencyOfPostTreatment) ;

	GenericSolver( const GenericSolver& genericSolver ) ;

	const GenericSolver& operator= ( const GenericSolver& genericSolver ) ;

	/* class héritage */
	bool isAlternatingDirection( void ) const ;

	void setAlternatingDirection( void ) ;

	void setMaximumNumberbOfIter( int maximumNumberbOfIter ) ;

	void setNumberOfGhostCells( int numberOfGhostCells ) ;

	void setFinalTime( double finalTime ) ;

	int getMaximumNumberbOfIter( void ) const ;

	double getFinalTime( void ) const ;

	int getNumberOfGhostCells( void ) const ;

	int getFrequencyOfPostTreatment( void ) const ;

	void setFrequencyOfPostTreatment( int frequencyOfPostTreatment ) ;

	virtual ParaMEDMEM::MEDCouplingFieldDouble* initialConditions(const ParaMEDMEM::MEDCouplingIMesh* mesh) const = 0;

//	virtual ParaMEDMEM::MEDCouplingFieldDouble* refinementCriterion(const ParaMEDMEM::MEDCouplingAutoRefCountObjectPtr<ParaMEDMEM::MEDCouplingAMRAttribute> fields) const = 0 ;

	virtual ParaMEDMEM::MEDCouplingFieldDouble* refinementCriterion(const ParaMEDMEM::MEDCouplingFieldDouble* field) const = 0 ;

	virtual double advancingTimeStep(int idirection,
							 	 	 double currentTime,
							 	 	 ParaMEDMEM::MEDCouplingAMRAttribute* fields,
							 	 	 const ParaMEDMEM::MEDCouplingCartesianAMRPatchGen* grid) const = 0;

	virtual ~GenericSolver();

protected:
	bool _isAlternatingDirection;
	double _finalTime;
	int _maximumNumberbOfIter;
	int _numberOfGhostCells;
	int _frequencyOfPostTreatment;
};

#endif /* GENERICSOLVER_HXX_ */
