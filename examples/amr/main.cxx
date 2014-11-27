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
    int spaceDim = 3;

    double xinf=0.;
    double yinf=0.;
    double zinf=0.;
    double xsup=1.;
    double ysup=1.;
<<<<<<< HEAD
    int nx=50;
    int ny=50;
=======
    double zsup=1.;
    int nx=200;
    int ny=200;
    int nz=200;
>>>>>>> dd355ff4f4f9ec0f845fac2acdb475c3e0055109

    double dx = (xsup - xinf)/nx ;
    double dy = (ysup - yinf)/ny ;
    double dz = (zsup - zinf)/nz;

    double *originPtr = new double[spaceDim];
    double *dxyzPtr = new double[spaceDim];
    int *nodeStrctPtr = new int[spaceDim];

    originPtr[0]=xinf;
    originPtr[1]=yinf;
    if (spaceDim == 3)
        originPtr[2]=zinf;
    nodeStrctPtr[0]=nx+1;
    nodeStrctPtr[1]=ny+1;
    if (spaceDim == 3)
        nodeStrctPtr[2]=nz+1;
    dxyzPtr[0]=dx;
    dxyzPtr[1]=dy;
    if (spaceDim == 3)
        dxyzPtr[2]=dz;


    string meshName="MeshAMR2D";

    MEDCouplingIMesh* coarseMesh=MEDCouplingIMesh::New(meshName,spaceDim,nodeStrctPtr,nodeStrctPtr+spaceDim,originPtr,originPtr+spaceDim,dxyzPtr,dxyzPtr+spaceDim);
    delete [] originPtr;
    delete [] dxyzPtr;
    delete [] nodeStrctPtr;

    cout << coarseMesh->getNumberOfCells() << endl;


    vector< pair<string,int> > fieldsInfos(1);
    fieldsInfos[0].first="YY";
    fieldsInfos[0].second=1;


    /* Infos solver advection */
    //*
<<<<<<< HEAD
    bool isAlternatingDirection=true;
    int maximumNumberbOfIter=500;
    double finalTime=10.;
    int numberOfGhostCells=2;
    int frequencyOfPostTreatment=10;
    double cfl=0.7;
=======
    bool isAlternatingDirection = true;
    int maximumNumberbOfIter = 1000;
    double finalTime = 12.;
    int numberOfGhostCells = 2;
    int frequencyOfPostTreatment = 10;
    double cfl = 0.7;
>>>>>>> dd355ff4f4f9ec0f845fac2acdb475c3e0055109
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

<<<<<<< HEAD
    vector<int> coefRefinement1(2);
    coefRefinement1[0]=1024;
    coefRefinement1[1]=4;
=======
    vector<int> coefRefinement1(spaceDim);
    coefRefinement1[0]=8;
    coefRefinement1[1]=8;
    coefRefinement1[2]=8;
>>>>>>> dd355ff4f4f9ec0f845fac2acdb475c3e0055109

    vector< vector<int> > coefsRefinement(maxLevels);
    coefsRefinement[0]=coefRefinement1;

    vector< double > efficiencyGoal(maxLevels);
    efficiencyGoal[0]=0.8;
    vector< double > efficiencyThreshold(maxLevels);
    efficiencyThreshold[0]=0.7;
    vector< int > maximumNbOfCellsInPatch(maxLevels);
    maximumNbOfCellsInPatch[0]=6000; // NB: 6000 >> 1728=12*12*12
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
    specificName += toString(maximumPatchLength[0]);
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
