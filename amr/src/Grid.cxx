/*
 * Grid.cxx
 *
 *  Created on: 4 fvr. 2013
 *      Author: am217485
 */

#include "Grid.hxx"

#include <iostream>
#include <cassert>
#include <sstream>
#include <fstream>
#include <algorithm>

using namespace std;

Grid::Grid()
{
	_level=-1;
	_coefRefinementX=-1;
	_coefRefinementY=-1;
}

Grid::Grid(const int level,const int coefRefinementX,const int coefRefinementY)
{
	_level=level;
	_coefRefinementX=coefRefinementX;
	_coefRefinementY=coefRefinementY;
}


void
Grid::addPatch (Patch patch )
{
	_patchs.push_back(patch);
}

void
Grid::erasePatch ( int index )
{
	_patchs.erase(_patchs.begin()+index);
}

void
Grid::resize ( int nbPatchs )
{
	_patchs.resize(nbPatchs);
}

vector<Patch>
Grid::getPatchs ( void ) const
{
	return _patchs;
}

Patch
Grid::getPatch ( unsigned int index ) const
{
	if (index > _patchs.size() )
		cerr << "getPatch: index > patch size" << endl;
	return _patchs[index];
}

Grid::~Grid()
{
//	_patchs.clear();
}

unsigned int
Grid::getNumberOfPatchs ( void ) const
{
	return _patchs.size();
}

int
Grid::getCoefRefinementX ( void ) const
{
	return _coefRefinementX;
}

int
Grid::getCoefRefinementY ( void ) const
{
	return _coefRefinementY;
}

int
Grid::getLevel ( void ) const
{
	return _level;
}


//----------------------------------------------------------------------
Grid::Grid( const Grid & grid )
//----------------------------------------------------------------------
{
	_level=grid.getLevel();
	_coefRefinementX=grid.getCoefRefinementX();
	_coefRefinementY=grid.getCoefRefinementY();
	for (unsigned int i=0;i<grid.getNumberOfPatchs();i++)
		_patchs.push_back(grid.getPatch(i));
}


const Grid&
Grid::operator= ( const Grid& grid)
{
	_level=grid.getLevel();
	_coefRefinementX=grid.getCoefRefinementX();
	_coefRefinementY=grid.getCoefRefinementY();
	for (unsigned int i=0;i<grid.getNumberOfPatchs();i++)
		_patchs.push_back(grid.getPatch(i));
	return *this;
}

void
Grid::infoGrid(void) const
{
	cout << "=====================================================" << endl;
	cout << "           Information Grid level " << getLevel() << endl;
	cout << "=====================================================" << endl;
	for (unsigned int i=0;i<getNumberOfPatchs();i++)
		getPatch(i).infoPatch();
}

void
Grid::statistic(void) const
{
    vector<int> rect_area(getNumberOfPatchs()) ;
    int total_rect_area =0;
    float min_nx = 1.E30 ;
    float min_ny = 1.E30 ;
    int flagged_area=0;
    for (unsigned int i=0;i<getNumberOfPatchs();i++)
    {
        int nx = getPatch(i).getNx() ;
        int ny = getPatch(i).getNy() ;
        if ( float(nx) < min_nx )
            min_nx = float(nx) ;
        if ( float(ny) < min_ny )
            min_ny = float(ny) ;
        rect_area[i] = nx*ny ;
        total_rect_area+=rect_area[i] ;
        flagged_area+=getPatch(i).getNumberOfCellsFlags();
    }
    int min_edge = min(min_nx,min_ny) ;
    int max_rect_area=*max_element(rect_area.begin(),rect_area.end()) ;
    float mean_rect_area = float(total_rect_area) / float(rect_area.size()) ;
    float ratio = 100.*flagged_area / total_rect_area ;
	cout << "=====================================================" << endl;
	cout << "                        Static                       " << endl;
	cout << "=====================================================" << endl;

    cout << "- Total Number of flagged pts : " << flagged_area << endl;
    cout << "- Total Area in patchs : " << total_rect_area << endl;
    cout << "- flagged points/tot.patch.vol. : " << ratio << "%" << endl;
    cout << "- Number of patchs : " << getNumberOfPatchs() << endl;
    cout << "- Minimum box edge : " << min_edge << endl;
    cout << "- Maximum box volume : " << max_rect_area << endl;
    cout << "- Average box volume : " << mean_rect_area << endl;
}

void
Grid::writeVTK(const string fileName) const
{
	string filPvd;
    filPvd=fileName;
    filPvd=filPvd+"_Level_";
    ostringstream numGrid;
    numGrid << getLevel()+1;
    filPvd=filPvd+numGrid.str();
    filPvd=filPvd+".pvd";
    ofstream file(filPvd.c_str(),ios::out | ios::trunc) ;
    if (file)
    {
        file << "<VTKFile type=\"Collection\" version=\"0.1\" byte_order=\"LittleEndian\"><Collection>\n" ;
    }else
        cerr << "Error : Can not open file " << filPvd << endl;
    unsigned int nbPatch=getNumberOfPatchs();
    for (unsigned int i=0; i<nbPatch; i++)
    {
        string filetmp;
        string outPutNameFile="CDMATH_Leve_";
        filetmp=outPutNameFile+numGrid.str()+"_Patch_";
        ostringstream numfile;
        numfile << i+1 ;
        filetmp=filetmp+numfile.str();
        filetmp=filetmp+".vtu";
        file << "<DataSet timestep=\"0.0\" group=\"advection\" part=\"0\" file=\"" << filetmp << "\"/>\n" ;
        getPatch(i).getMesh().writeVTK(filetmp) ;
    }
    file << "</Collection></VTKFile>\n" ;
    file.close() ;
}

void
Grid::correctPatchsIndex(void)
{
    unsigned int nbPatch=getNumberOfPatchs();
    for (unsigned int i=0; i<nbPatch; i++)
    {
    	getPatch(i).setIndex(i);
    }
}
