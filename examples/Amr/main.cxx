//============================================================================
// Name        : Solver.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
using namespace std;

#include <MEDCouplingCartesianAMRMesh.hxx>
#include <MEDCouplingFieldDouble.hxx>
#include <MEDCouplingAMRAttribute.hxx>
#include <MEDCouplingIMesh.hxx>
#include <MEDCouplingAutoRefCountObjectPtr.hxx>

#include "AMR.hxx"
#include "AdvectionSolver.hxx"
#include "HeatSolver.hxx"
using namespace ParaMEDMEM;

int main() {


    /* Coarse Mesh */
	double xinf=0.;
	double yinf=0.;
	double xsup=2.;
	double ysup=2.;
	int nx=50;
	int ny=50;

    double dx = (xsup - xinf)/nx ;
    double dy = (ysup - yinf)/ny ;

	double *originPtr = new double[2];
    double *dxyzPtr = new double[2];
    int *nodeStrctPtr = new int[2];

    originPtr[0]=xinf;
    originPtr[1]=yinf;
    nodeStrctPtr[0]=nx+1;
    nodeStrctPtr[1]=ny+1;
    dxyzPtr[0]=dx;
    dxyzPtr[1]=dy;


    int spaceDim=2;
    string meshName="MeshAMR2D";

    MEDCouplingIMesh* coarseMesh=MEDCouplingIMesh::New(meshName,spaceDim,nodeStrctPtr,nodeStrctPtr+spaceDim,originPtr,originPtr+spaceDim,dxyzPtr,dxyzPtr+spaceDim);
    delete [] originPtr;
    delete [] dxyzPtr;
    delete [] nodeStrctPtr;


    vector< pair<string,int> > fieldsInfos(1);
    fieldsInfos[0].first="YY";
    fieldsInfos[0].second=1;


    /* Infos solver advection */
    /*
    bool isAlternatingDirection=true;
    int maximumNumberbOfIter=1;
    double finalTime=2.;
    int numberOfGhostCells=2;
    int frequencyOfPostTreatment=1;
    double cfl=0.7;
	AdvectionSolver solver(finalTime,
    					   maximumNumberbOfIter,
    					   isAlternatingDirection,
    					   numberOfGhostCells,
    					   frequencyOfPostTreatment,
    					   cfl);
   */
    /* End Infos Solver Advection */
    bool isAlternatingDirection=false;
    int maximumNumberbOfIter=100;
    double finalTime=2.;
    int numberOfGhostCells=2;
    int frequencyOfPostTreatment=1;
    double nu=0.005;
	HeatSolver solver(finalTime,
    					   maximumNumberbOfIter,
    					   isAlternatingDirection,
    					   numberOfGhostCells,
    					   frequencyOfPostTreatment,
    					   nu);


    /* Infos AMR */
	int maxLevels=1;

    int frequencyOfRefinement=1;

    vector<int> coefRefinement1(2);
    coefRefinement1[0]=2;
    coefRefinement1[1]=2;

    vector< vector<int> > coefsRefinement(maxLevels);
    coefsRefinement[0]=coefRefinement1;

    vector< double > efficiencyGoal(maxLevels);
    efficiencyGoal[0]=0.9;
    vector< double > efficiencyThreshold(maxLevels);
    efficiencyThreshold[0]=0.9;
    vector< int > maximumNbOfCellsInPatch(maxLevels);
    maximumNbOfCellsInPatch[0]=300;
    vector< int > minimumPatchLength(maxLevels);
    minimumPatchLength[0]=4;
    vector< int > maximumPatchLength(maxLevels);
    maximumPatchLength[0]=10;

    /* End Infos AMR */
	AMR amr(maxLevels,
			frequencyOfRefinement,
    		efficiencyGoal,
    		efficiencyThreshold,
    		maximumNbOfCellsInPatch,
    		minimumPatchLength,
    		maximumPatchLength,
    		coefsRefinement);

	amr.initialize(coarseMesh,
    			   fieldsInfos,
    			   solver);

    amr.compute(solver);
    coarseMesh->decrRef();
    cout << "fin........." << endl;
    return 0;
}
