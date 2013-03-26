/*
 * AlgoAMR.hxx
 *
 *  Created on: 4 fevr. 2013
 *      Author: mekkas
 */

#ifndef ALGOAMR_HXX_
#define ALGOAMR_HXX_

class Mesh;
class Field;

#include "IntTab.hxx"
#include "DoubleTab.hxx"
#include "Grid.hxx"
#include "Field.hxx"

#include<vector>
#include<string>

class AlgoAMR
{
public:
	AlgoAMR();

    /**
     * constructor with data
     * @param maxLevel :  Le nombre maximum de niveau.
     * @param coefRefinement :  Les coeficients de raffinement pour chaque niveau.
     * @param efficiency : Le seuil d'efficacite minimum autorise pour un patch.
     * Sueil d'efficacite (positif et >=1) = rapport du nombre de mailles
     * flages / aire totale d'un patch.
     * @param minCellDirection :  Le nombre minimal de maille pour un patch (dans toutes les directions).
     * @param maxCells :  Le nombre maximum de maille pour un patch.
     */
	AlgoAMR(const int maxLevel,
	        const IntTab coefRefinementX,
	        const IntTab coefRefinementY,
            const DoubleTab efficiency,
            const DoubleTab efficiencySnd,
            const IntTab minCellDirection,
            const IntTab maxCells);

    /**
     * constructor with data
     * @param maxLevel :  Le nombre maximum de niveau.
     * @param coefRefinement :  Les coeficients de raffinement pour chaque niveau.
     * @param efficiency : Le seuil d'efficacite minimum autorise pour un patch.
     * Sueil d'efficacite (positif et >=1) = rapport du nombre de mailles
     * flages / aire totale d'un patch.
     * @param minCellDirection :  Le nombre minimal de maille pour un patch (dans toutes les directions).
     * @param maxCells :  Le nombre maximum de maille pour un patch.
     */
	void define(const int maxLevel,
	        	const IntTab coefRefinementX,
	        	const IntTab coefRefinementY,
	        	const DoubleTab efficiency,
	        	const DoubleTab efficiencySnd,
	        	const IntTab minCellDirection,
	        	const IntTab maxCells) ;

	/// Destructor
	virtual ~AlgoAMR();

	const DoubleTab& getEfficiency(void) const ;
	double getEfficiency(const int level) const ;

	const DoubleTab& getEfficiencySnd(void) const ;
	double getEfficiencySnd(const int level) const ;

	const IntTab& getMinCellDirection(void) const ;
	int getMinCellDirection(const int level) const ;

	int getMaxCells(const int level) const ;
	const IntTab& getMaxCells(void) const ;

	int getMaxLevel ( void ) const ;
	bool isVerbose() const ;

	AlgoAMR( const AlgoAMR & algoAMR ) ;

	const AlgoAMR& operator= ( const AlgoAMR& algoAMR) ;

	std::vector<Grid> getGrids() const ;

	Grid getGrid(const int index) const ;

	void addGrid(Grid grid) ;

	unsigned int getNumberOfGrids ( void ) const ;

	const IntTab& getCoefRefinementX() const ;

	const IntTab& getCoefRefinementY() const ;

	virtual void computeGrids(const Mesh& mesh, const Field& fieldFlag) = 0;

	void writeVTK(const std::string fileName ) const ;

protected:
	  /*
	   * Le seuil d'efficacite minimum autorise pour un patch. Sueil d'efficacite (positif et >=1) = rapport du nombre de mailles flages / aire totale d'un patch.
	   */
	  DoubleTab _efficiency;

	  /*
	   * Le seuil d'efficacite minimum autorise pour un patch qui n'ont ni de point d'inflexion ni de trous.
	   */
	  DoubleTab _efficiencySnd;

	  /*
	   * Le nombre de maille minimum dans chaque direction dans un patch
	   */

	  IntTab _minCellDirection;
	  /*
	   * Le nombre maximum de cellules pour un patch
	   */
	  IntTab _maxCells;

	  /*
	   * Le nombre maximum des niveaux
	   */
	   int _maxLevel;

	  /*
	   * Les coefficients de raffinement dans la direction X pour chaque niveau
	   */
	   IntTab _coefRefinementX;

	  /*
	   * Les coefficients de raffinement dans la direction X pour chaque niveau
	   */
	   IntTab _coefRefinementY;

	  /*
	   * verbose log
	   */
	  bool _verbose;

	  /*
	   * liste des grilles
	   */
	 std::vector<Grid> _grids;

};


#endif /* ALGOAMR_HXX_ */
