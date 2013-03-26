/*
 * BergerAlgoAMR.hxx
 *
 *  Created on: 3 fvr. 2013
 *      Author: mekkas
 */

#ifndef BERGERALGOAMR_HXX_
#define BERGERALGOAMR_HXX_

#include "AlgoAMR.hxx"

class BergerAlgoAMR : public AlgoAMR
{
public:
	/// Default constructor -- leaves object in an unusable state
	BergerAlgoAMR();

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
	BergerAlgoAMR(const int maxLevel,
	        	  const IntTab coefRefinementX,
	        	  const IntTab coefRefinementY,
            	  const DoubleTab efficiency,
            	  const DoubleTab efficiencySnd,
            	  const IntTab minCellDirection,
            	  const IntTab maxCells);

	/// Destructor
	virtual ~BergerAlgoAMR();


	void computeGrids(const Mesh& mesh, const Field& fieldFlag) ;

	Grid computeGrid(const Mesh& mesh, const Field& fieldFlag, const int level) ;

};


#endif /* BERGERALGOAMR_HXX_ */
