/*
 * HeatSolver.hxx
 *
 *  Created on: 3 juil. 2014
 *      Author: mekkas
 */

#ifndef HEATSOLVER_HXX_
#define HEATSOLVER_HXX_

#include "GenericSolver.hxx"


class HeatSolver : public GenericSolver
{
public:
	HeatSolver(double finalTime, int maximumNumberbOfIter, bool isAlternatingDirection, int numberOfGhostCells, int frequencyOfPostTreatment,double cfl);

	virtual ~HeatSolver();

	void setDiffusion( double nu ) ;

	double getDiffusion( void ) const ;

	double computeDt(double cfl, std::vector<double> dxyz) const;

	void SolverHeat2D(int numberOfCellsGhost,
					  const ParaMEDMEM::MEDCouplingIMesh *imesh,
					  double dt,
					  ParaMEDMEM::MEDCouplingFieldDouble* yyWithGhost) const ;

	/* classe obligatoire */
	ParaMEDMEM::MEDCouplingFieldDouble* initialConditions(const ParaMEDMEM::MEDCouplingIMesh* mesh) const ;

	ParaMEDMEM::MEDCouplingFieldDouble* refinementCriterion(const ParaMEDMEM::MEDCouplingFieldDouble* field) const ;

	double advancingTimeStep(int idir,
							 double currentTime,
						     ParaMEDMEM::MEDCouplingAMRAttribute* fields,
						     const ParaMEDMEM::MEDCouplingCartesianAMRPatchGen* grid) const;
private:
	double _nu;
};

#endif /* HeatSOLVER_HXX_ */
