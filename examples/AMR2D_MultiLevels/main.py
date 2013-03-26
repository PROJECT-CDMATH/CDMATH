# coding: latin-1 

from cdmath import *
from math import sqrt

def main():
    print "GENERATION STRUCTURE AMR AVEC 3 NIVEAUX"

    xinf=0.0 ;
    yinf=0.0 ;
    xsup=2.0 ;
    ysup=2.0 ;
    nl = 50 ;
    nc = 50 ;
    M=Mesh(xinf,xsup,nc,yinf,ysup,nl);
    M.writeVTK("Mesh_Coarse.vtu");


    maxLevel = 3;
    coefRefinementX=IntTab(maxLevel);
    coefRefinementX[0]=2;
    coefRefinementX[1]=4;
    coefRefinementX[2]=6;
    coefRefinementY=IntTab(maxLevel);
    coefRefinementY[0]=2;
    coefRefinementY[1]=4;
    coefRefinementY[2]=6;
    efficiency=DoubleTab(maxLevel);
    efficiency[0]=0.1;
    efficiency[1]=0.5;
    efficiency[2]=0.9;
    efficiencySnd=DoubleTab(maxLevel);
    efficiencySnd[0]=0.1;
    efficiencySnd[1]=0.5;
    efficiencySnd[2]=0.9;
    minCellDirection=IntTab(maxLevel);
    minCellDirection[0]=3;
    minCellDirection[1]=3;
    minCellDirection[2]=3;
    maxCells=IntTab(maxLevel);
    maxCells[0]=1000;
    maxCells[1]=500;
    maxCells[2]=100;

    BA=BergerAlgoAMR(maxLevel,
                     coefRefinementX,
                     coefRefinementY,
                     efficiency,
                     efficiencySnd,
                     minCellDirection,
                     maxCells);

    fieldFlag=Field("Flag",CELLS,M,1) ;

    xr = 1. ;
    yr = 1.;
    r1 = 0.4;
    r2 = 0.7;
    for i in xrange(M.getNumberOfCells()):
        x=M.getCell(i).x();
        y=M.getCell(i).y();
        if ( sqrt( pow(x-xr,2) + pow(y-yr,2) ) < r2 and sqrt( pow(x-xr,2) + pow(y-yr,2) ) > r1 ):
            fieldFlag[i] = 1.0;
            pass
        pass
    
    fieldFlag.writeVTK("FLAG.vtu") ;

    BA.computeGrids(M,fieldFlag);
    BA.writeVTK("TestAMR");
    return

if __name__ == '__main__':
    main()