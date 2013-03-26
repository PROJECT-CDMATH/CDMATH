//============================================================================
// Author      : Anouar MEKKAS
// Version     :
// Description : Generation d'une structure AMR 2D a 3 niveaux
//============================================================================

#include "BergerAlgoAMR.hxx"
#include "Grid.hxx"
#include "Mesh.hxx"
#include "Cell.hxx"

#include <cmath>

using namespace std;

int main(int argc, char** argv)
{
    int nc, nl;

    double xinf=0.0 ;
    double yinf=0.0 ;
    double xsup=2.0 ;
    double ysup=2.0 ;
    nl = 50 ;
    nc = 50 ;
    Mesh M(xinf,xsup,nc,yinf,ysup,nl);
    M.writeVTK("Mesh_Coarse.vtu");


    int maxLevel = 3;
    IntTab coefRefinementX(maxLevel);
    coefRefinementX[0]=2;
    coefRefinementX[1]=4;
    coefRefinementX[2]=6;
    IntTab coefRefinementY(maxLevel);
    coefRefinementY[0]=2;
    coefRefinementY[1]=4;
    coefRefinementY[2]=6;
    DoubleTab efficiency(maxLevel);
    efficiency[0]=0.1;
    efficiency[1]=0.5;
    efficiency[2]=0.9;
    DoubleTab efficiencySnd(maxLevel);
    efficiencySnd[0]=0.1;
    efficiencySnd[1]=0.5;
    efficiencySnd[2]=0.9;
    IntTab minCellDirection(maxLevel);
    minCellDirection[0]=3;
    minCellDirection[1]=3;
    minCellDirection[2]=3;
    IntTab maxCells(maxLevel);
    maxCells[0]=1000;
    maxCells[1]=500;
    maxCells[2]=100;

	BergerAlgoAMR BA(maxLevel,
					 coefRefinementX,
					 coefRefinementY,
					 efficiency,
					 efficiencySnd,
					 minCellDirection,
					 maxCells);

    Field fieldFlag("Flag",CELLS,M,1) ;

    double xr = 1. ;
    double yr = 1.;
    double r1 = 0.4;
    double r2 = 0.7;
    for (int i=0 ; i<M.getNumberOfCells() ; i++)
    {
    	double x=M.getCell(i).x();
    	double y=M.getCell(i).y();
    	if ( sqrt( pow(x-xr,2) + pow(y-yr,2) ) < r2 && sqrt( pow(x-xr,2) + pow(y-yr,2) ) > r1 )
    		fieldFlag(i) = 1.0;
    }
    fieldFlag.writeVTK("FLAG.vtu") ;

    BA.computeGrids(M,fieldFlag);
    BA.writeVTK("TestAMR");
    cout << "fin test......." << endl;
    return 0;
}
