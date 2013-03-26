/*
 * Grid.hxx
 *
 *  Created on: 4 fvr. 2013
 *      Author: am217485
 */

#ifndef GRID_HXX_
#define GRID_HXX_

#include "Patch.hxx"

#include <vector>

class Grid
{
	public:

	Grid();

	Grid(const int level,const int coefRefinementX,const int coefRefinementY) ;

	~Grid();

	Grid( const Grid & grid ) ;

    int getCoefRefinementX ( void ) const ;

    int getCoefRefinementY ( void ) const ;

    const Grid& operator= ( const Grid& grid) ;

	std::vector<Patch> getPatchs ( void ) const ;

    Patch getPatch ( unsigned int index ) const ;

    void addPatch ( Patch patch ) ;

    void infoGrid(void) const ;

    void erasePatch ( int index ) ;

    unsigned int getNumberOfPatchs ( void ) const ;

    void correctPatchsIndex(void) ;

    int getLevel ( void ) const ;

    void resize ( int nbPatchs ) ;

    void statistic( void ) const ;

    void writeVTK(const std::string fileName) const ;

	protected:
	int _level;
	int _coefRefinementX;
	int _coefRefinementY;
	std::vector<Patch> _patchs;
};

#endif /* GRID_HXX_ */
