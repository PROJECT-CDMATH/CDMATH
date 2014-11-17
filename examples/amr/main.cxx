//============================================================================
// Name        : Solver.cpp
// Author      : 
// Version     :
// Copyright   : CDMATH group
// Description : Computation with Adaptive Mesh Refinement
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
#include <sstream> // to convert integers to strings

#include <MEDCouplingCartesianAMRMesh.hxx>
#include <MEDCouplingFieldDouble.hxx>
#include <MEDCouplingAMRAttribute.hxx>
#include <MEDCouplingIMesh.hxx>
#include <MEDCouplingAutoRefCountObjectPtr.hxx>

#include "AMR.hxx"
#include "AdvectionSolver.hxx"
#include "HeatSolver.hxx"

using namespace ParaMEDMEM;
using namespace std;


std::string toString(const int value)
{
    stringstream ss;
    ss << value;
    return ss.str();
}

void initializeEnvironment(const std::string specificName)
{
    std::string commandString;
    std::string outputDirectory;
    outputDirectory = "out_" + specificName;
	commandString = "mkdir " + outputDirectory;
	const char * commandChar = commandString.c_str();
	system(commandChar);
    int statusIndicator;
	try
    {
		statusIndicator = chdir(outputDirectory.c_str());
		if (statusIndicator != 0) throw 20;
	}
	catch (int const& e)
	{
		cerr << "Error: could not change directory." << endl;
	}
	cout << "Cleaning up first" << endl;
	try
	{
		statusIndicator = system("rm *.pvd *.vtu *.med -r"); // careful with that command
		if (statusIndicator != 0) throw 20;
	}
	catch (int const& e)
	{
		cerr << "Error: something went wrong with deleting files." << endl;
	}
}

int main() {

    /* Coarse Mesh */
    double xinf=0.;
    double yinf=0.;
    double xsup=1.;
    double ysup=1.;
    int nx=200;
    int ny=200;

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
    //*
    bool isAlternatingDirection = true;
    int maximumNumberbOfIter = 1000;
    double finalTime = 12.;
    int numberOfGhostCells = 2;
    int frequencyOfPostTreatment = 10;
    double cfl = 0.7;
    AdvectionSolver solver(finalTime,
                           maximumNumberbOfIter,
                           isAlternatingDirection,
                           numberOfGhostCells,
                           frequencyOfPostTreatment,
                           cfl);
    //*/
    /* End Infos Solver Advection */
    /*
    bool isAlternatingDirection=false;
    int maximumNumberbOfIter=100;
    double finalTime=5.;
    int numberOfGhostCells=2;
    int frequencyOfPostTreatment=1;
    double nu=0.005;
    HeatSolver solver(finalTime,
                           maximumNumberbOfIter,
                           isAlternatingDirection,
                           numberOfGhostCells,
                           frequencyOfPostTreatment,
                           nu);
    */

    /* Infos AMR */
    int maxLevels=1;

    int frequencyOfRefinement=1;

    vector<int> coefRefinement1(2);
    coefRefinement1[0]=4;
    coefRefinement1[1]=4;

    vector< vector<int> > coefsRefinement(maxLevels);
    coefsRefinement[0]=coefRefinement1;

    vector< double > efficiencyGoal(maxLevels);
    efficiencyGoal[0]=0.8;
    vector< double > efficiencyThreshold(maxLevels);
    efficiencyThreshold[0]=0.7;
    vector< int > maximumNbOfCellsInPatch(maxLevels);
    maximumNbOfCellsInPatch[0]=2500;
    vector< int > minimumPatchLength(maxLevels);
    minimumPatchLength[0]=7;
    vector< int > maximumPatchLength(maxLevels);
    maximumPatchLength[0]=12;
    /* End Infos AMR */
    
    std::string specificName;
    specificName = toString(maxLevels);
    specificName += toString(coefRefinement1[0]);
    specificName += toString(efficiencyGoal[0]*10);
    specificName += toString(maximumNbOfCellsInPatch[0]);
    specificName += toString(minimumPatchLength[0]);
    initializeEnvironment(specificName);
    
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
    cout << "FIN........." << endl;
    return 0;
}
