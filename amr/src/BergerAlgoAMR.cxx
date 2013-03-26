/*
 * BergerAlgoAMR.cxx
 *
 *  Created on: 3 fvr. 2013
 *      Author: mekkas
 */

#include "BergerAlgoAMR.hxx"

#include <iostream>

using namespace std;

BergerAlgoAMR::BergerAlgoAMR()
{
}

BergerAlgoAMR::BergerAlgoAMR(const int maxLevel,
		                     const IntTab coefRefinementX,
		                     const IntTab coefRefinementY,
                             const DoubleTab efficiency,
                             const DoubleTab efficiencySnd,
                             const IntTab minCellDirection,
                             const IntTab maxCells )
{
	define(maxLevel,coefRefinementX,coefRefinementY,efficiency,efficiencySnd,minCellDirection, maxCells);
}

BergerAlgoAMR::~BergerAlgoAMR()
{
}

void BergerAlgoAMR::computeGrids(const Mesh& mesh, const Field& fieldFlag)
{
    int maxLevels=getMaxLevel() ;
    addGrid(computeGrid(mesh,fieldFlag,0));
	if (isVerbose())
	{
		getGrid(0).infoGrid();
		getGrid(0).statistic();
	}

	for (int iLevel=1;iLevel<maxLevels;iLevel++)
	{
		Grid grid = getGrid(iLevel-1);
		int nbPatchs=grid.getNumberOfPatchs();
	    Grid gridLevel(iLevel,getCoefRefinementX()[iLevel],getCoefRefinementY()[iLevel]);
	    vector<Patch> vectPatch=grid.getPatchs();
		for (int ip=0;ip<nbPatchs;ip++)
		{
			Grid gridPatch=computeGrid(vectPatch[ip].getFieldFlag().getMesh(),vectPatch[ip].getFieldFlag(),iLevel);
			int nbPatchsIp=gridPatch.getNumberOfPatchs();
			for (int j=0;j<nbPatchsIp;j++)
			{
				gridLevel.addPatch(gridPatch.getPatch(j));
			}
		}
		gridLevel.correctPatchsIndex();
		if (isVerbose())
		{
			gridLevel.infoGrid();
			gridLevel.statistic();
		}
		addGrid(gridLevel);
	}

//	for (int iLevel=0;iLevel<maxLevels;iLevel++)
//		getGrid(iLevel).outPut("AmrPatchs","VTK");
}

Grid
BergerAlgoAMR::computeGrid(const Mesh& mesh, const Field& fieldFlag, const int level)
{
    unsigned int minCellDirection=getMinCellDirection()[level];
    double effeciency=getEfficiency()[level];
    double effeciencySnd=getEfficiencySnd()[level];
    int maxCells=getMaxCells()[level] ;

    if (isVerbose())
	{
		cout << "/////////////////////////////////////////////" << endl;
		cout << "// Parameters Level n° " << level << "     //" << endl;
		cout << "/////////////////////////////////////////////" << endl;
		cout << "Efficiency threshold: " << 100*effeciency << "%"<< endl;
		cout << "Efficiency Snd threshold: " << 100*effeciencySnd << "%"<< endl;
		cout << "Min. box side length: " << minCellDirection << endl;
		cout << "Max. box cells : " << maxCells << endl;
	}

    /* Initialize and find the first box */
    int coefRefinementX=getCoefRefinementX()[level];
    int coefRefinementY=getCoefRefinementY()[level];

    Grid grid(level,coefRefinementX,coefRefinementY);

    Grid gridOut(level,coefRefinementX,coefRefinementY);

    Patch PInitial(mesh,fieldFlag);
    PInitial.computeMesh(mesh);
    PInitial.computeFieldFlag(fieldFlag);
	grid.addPatch(PInitial);
    unsigned int bigDims ;
    int dissectDirection ;
    int nX ;
    int nY ;
    unsigned int idPatch = 0 ;
    int cut[3] ;
    int nbActions=4 ;

    while ( idPatch < grid.getNumberOfPatchs() )
    {
    	Patch patch=grid.getPatch(idPatch);
    	patch.computeMesh(mesh);
    	patch.computeFieldFlag(fieldFlag);
        if (isVerbose())
        	patch.infoPatch();
        nX= patch.getNx();
		nY= patch.getNy();
        if ( nY > nX )
        {
        	bigDims = nY ;
            dissectDirection = 1 ;
        }
        else
        {
        	bigDims = nX ;
            dissectDirection = 0 ;
        }

        int cutFound= 0 ;
        int cutPlace=-1 ;
        int cutDim=-1 ;
        int flagAction1=0 ;
        int flagAction2=0 ;
        int flagAction3=0 ;
        int flagAction4=0 ;
    	for(int idAction=1; idAction<=nbActions; idAction++)
        {
            switch (idAction)
            {
                case 1 : /* Action 1 */
                {
                	if (isVerbose())
                		cout << "- Action 1: check efficiency; if efficienct but box too big, dissect" << endl;
                    if ( patch.getEfficiency() >= effeciency )
                    {
                        flagAction1 = 0 ;
                        if (isVerbose())
                        	cout << "- Box has the required efficiency" << endl;
                        if ( patch.getNumberOfCells() > maxCells)
                        {
                            patch.dissectBigPatch (mesh,
                            						fieldFlag,
                            						minCellDirection,
                            						bigDims,
                            						dissectDirection,
                            						cut) ;
                      	    cutFound = cut[0] ;
                            cutPlace = cut[1] ;
                            cutDim   = cut[2] ;
                        }
                        if (cutFound==0) flagAction1 = 1 ;
                    }
                    break ;
                }
                case 2 : /* Action 2 */
                {
                	if (isVerbose())
                		cout << "- Action 2: looking for holes" << endl;
                    flagAction2=0;
                    patch.findHole(dissectDirection,
                    				minCellDirection,
                    				cut) ;
					cutFound = cut[0] ;
                    cutPlace = cut[1] ;
                    cutDim   = cut[2] ;
                    if (cutFound==1) flagAction2 = 1 ;
                    break ;
                }
                case 3 : /* Action 3 */
                {
                	if (isVerbose())
                		cout << "- Action 3: looking for inflection points" << endl ;
                    flagAction3 = 0 ;
                    patch.findInflection(minCellDirection,
                    					  cut);
                    cutFound = cut[0] ;
                    cutPlace = cut[1] ;
                    cutDim   = cut[2] ;
                    if (cutFound==1) flagAction3 = 1 ;
                    break ;
                }
                case 4 : /* Action 4 */
                {
                	if (isVerbose())
                		cout << "- Action 4: no holes or inflections, dissect if not efficienct and not too small; otherwise, check if too big" << endl ;
                    flagAction4 = 0 ;
                    cut[2]=dissectDirection;
                    if ( patch.getEfficiency() <= effeciencySnd)
                    {
                    	if (bigDims>=2*minCellDirection)
                    	{
                    		if (isVerbose())
                    		{
                        		cout << "Bisecting box because efficiency = " << patch.getEfficiency() << " < " << effeciencySnd
                        			 << " and side size = " << bigDims << " > minimum cells = " << minCellDirection << endl;
                    		}
                    		cut[0]=1;
                    		cut[1]=int(bigDims/2)+patch.getIndexCorners()[dissectDirection]-1;
                    		cutFound = cut[0] ;
                            cutPlace = cut[1] ;
                            cutDim   = cut[2] ;
                    	}
                    } else
                    {
                        if ( patch.getNumberOfCells() > maxCells)
                        {
                        	patch.dissectBigPatch (mesh,
                        							fieldFlag,
                        							minCellDirection,
                        							bigDims,
                        							dissectDirection,
                        							cut) ;
                            cutFound = cut[0] ;
                            cutPlace = cut[1] ;
                            cutDim   = cut[2] ;
                        }
                    }
                    if (cutFound==1) flagAction4 = 1 ;
                    break ;
                }
                default:/* Action default */
                {
                    cerr << "Unknown action! exiting" << endl ;
                }
            }
            if ( flagAction1 )  break ;
            if ( flagAction2 )  break ;
            if ( flagAction3 )  break ;
            if ( flagAction4 )  break ;
            if ( cutFound ==1 ) break;
        }
    	/* Patch found */
        if ( cutFound )
        {
           if(isVerbose())
        	   cout << "This box is dissected at cut.dim = " << cutDim << " cut.place = " << cutPlace << endl;
           int rectLeft[4] ;
           int rectRight[4] ;
           patch.dissectPatch(mesh,
        		   	   	   	   fieldFlag,
        		   	   	   	   minCellDirection,
        		   	   	   	   cut,
        		   	   	   	   rectLeft,
        		   	   	   	   rectRight);
           Patch patchLeft(rectLeft);
           patchLeft.computeMesh(mesh);
           patchLeft.computeFieldFlag(fieldFlag);
           grid.addPatch(patchLeft);
           Patch patchRight(rectRight);
           patchRight.computeMesh(mesh);
           patchRight.computeFieldFlag(fieldFlag);
           grid.addPatch(patchRight);
           grid.erasePatch(idPatch);
        }
        else
        {
        	patch.setIndex(idPatch);
        	patch.computeMesh(mesh,coefRefinementX,coefRefinementY);
        	gridOut.addPatch(patch);
			cout << "- This patch " << idPatch+1 << " is accepted" << endl ;
            idPatch++;
        }
    }
    return gridOut ;
}

