/*
 * AdvectionSolver.hxx
 *
 *  Created on: 3 juil. 2014
 *      Author: mekkas
 */

#ifndef ADVECTIONSOLVER_HXX_
#define ADVECTIONSOLVER_HXX_

#include "GenericSolver.hxx"


class AdvectionSolver : public GenericSolver
{
public:
	AdvectionSolver(double finalTime, int maximumNumberbOfIter, bool isAlternatingDirection, int numberOfGhostCells, int frequencyOfPostTreatment,double cfl);

	virtual ~AdvectionSolver();

	void setCfl( double cfl ) ;

	double getCfl( void ) const ;

	double computeDt(double cfl, std::vector<double> dxyz, const ParaMEDMEM::MEDCouplingFieldDouble* velocity) const;

	ParaMEDMEM::MEDCouplingFieldDouble* computeVelocity(const ParaMEDMEM::MEDCouplingIMesh* mesh, double currentTime, double finalTime) const;

	ParaMEDMEM::MEDCouplingFieldDouble* computeFluxUpwind(int direction,
													  double dspace,
													  int numberOfCellsGhost,
													  const ParaMEDMEM::MEDCouplingIMesh* mesh,
													  const ParaMEDMEM::MEDCouplingFieldDouble* velocity,
													  const ParaMEDMEM::MEDCouplingFieldDouble* YY_WITH_GHOST) const;

	ParaMEDMEM::MEDCouplingFieldDouble* computeFluxDL(int direction,
													  double dspace,
													  double dt,
													  int numberOfCellsGhost,
													  const ParaMEDMEM::MEDCouplingIMesh* mesh,
													  const ParaMEDMEM::MEDCouplingFieldDouble* velocity,
													  const ParaMEDMEM::MEDCouplingFieldDouble* YY_WITH_GHOST) const;

	void advanceTimeDL(int numberOfCellsGhost,
					   double dspace,
					   double dt,
					   const ParaMEDMEM::MEDCouplingIMesh* imesh,
					   const ParaMEDMEM::MEDCouplingFieldDouble* fluxes,
					   ParaMEDMEM::MEDCouplingFieldDouble* yyWithGhost) const;

	void SolverDL(int idir,
				  int numberOfCellsGhost,
				  double dt,
				  const ParaMEDMEM::MEDCouplingIMesh *imesh,
				  const ParaMEDMEM::MEDCouplingFieldDouble* velocity,
				  ParaMEDMEM::MEDCouplingFieldDouble* yyWithGhost) const;


	/* classe obligatoire */
	ParaMEDMEM::MEDCouplingFieldDouble* initialConditions(const ParaMEDMEM::MEDCouplingIMesh* mesh) const ;

	ParaMEDMEM::MEDCouplingFieldDouble* refinementCriterion(const ParaMEDMEM::MEDCouplingFieldDouble* field) const ;

	double advancingTimeStep(int idir,
							 double currentTime,
						     ParaMEDMEM::MEDCouplingAMRAttribute* fields,
						     const ParaMEDMEM::MEDCouplingCartesianAMRPatchGen* grid) const;
private:
	double _cfl;
};

#endif /* ADVECTIONSOLVER_HXX_ */
