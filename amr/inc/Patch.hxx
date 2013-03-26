/*
 * Patch.hxx
 *
 *  Created on: 4 fvr. 2013
 *      Author: am217485
 */

#ifndef PATCH_HXX_
#define PATCH_HXX_

#include "Mesh.hxx"
#include "Field.hxx"

#include <vector>

class Patch
{
	public:

		Patch();

		Patch(const int *indexCorners) ;

		Patch( const Patch & patch ) ;

		Patch(const Mesh& mesh, const Field& fieldFlag) ;

		const Patch& operator= ( const Patch& patch) ;

		~Patch();

		void computeMesh(const Mesh& mesh, const int coefRefinementX, const int coefRefinementY ) ;

		void computeMesh(const Mesh& mesh) ;

		void computeFieldFlag(const Field& fieldFlag) ;

		void computeNumberOfCellsFlags( void ) ;

		void dissectPatch( const Mesh& mesh, const Field& fieldFlag, const unsigned int minCellDirection, const int *cut,
						   int *rectLeft, int *rectRight) const ;

		void dissectBigPatch( const Mesh& mesh, const Field& fieldFlag, const unsigned int minCellDirection,
							  const unsigned int bigDims, const int dissectDirection, int *cut ) const ;

		void findHole(const int dissectDirection, const unsigned int minCellDirection, int *cut) const ;

		void findInflection(const unsigned int minCellDirection, int *cut ) const ;

		void findPosCellsFlags(void) ;

		void computeSignature( void ) ;

		void infoPatch(void) const ;

		void setIndex(const int index);

		bool isVerbose( void ) const ;

		Mesh getMesh( void ) const ;

		Field getFieldFlag( void ) const ;

		Cell& getCellRightLowCorner( void ) const ;

		Cell& getCellLeftLowCorner ( void ) const ;

		Cell& getCellRightHighCorner ( void ) const ;

		Cell& getCellLeftHighCorner ( void ) const ;

		std::vector<int> getSignatureX(void) const ;

		std::vector<int> getSignatureY(void) const ;

		std::vector<int> getPosCellX( void ) const ;

		std::vector<int> getPosCellY( void ) const ;

		const int* getIndexCorners ( void ) const ;

		int getIndexCellLeftLowCorner ( void ) const ;

		int getIndexCellLeftHighCorner ( void ) const ;

		int getIndexCellRightLowCorner ( void ) const ;

		int getIndexCellRightHighCorner ( void ) const ;

		int getNx ( void ) const ;

		int getNy ( void ) const ;

		int getCoefRefinementX ( void ) const ;

		int getCoefRefinementY ( void ) const ;

		int getLevel ( void ) const ;

		int getIndex ( void ) const ;

		int getNumberOfCellsFlags( void ) const ;

		int getNumberOfCells( void ) const ;

		float getEfficiency( void ) const ;

	protected:
		int _indexCellLeftLowCorner;
		int _indexCellRightLowCorner;
		int _indexCellLeftHighCorner;
		int _indexCellRightHighCorner;
		int _indexCorners[4];
		int _nX;
		int _nY;
		int _coefRefinementX;
		int _coefRefinementY;
		int _level;
		int _index;
		bool _verbose;
		Mesh _mesh;
		int _numberOfCellsFlags;
		float _efficiency;
		Field _fieldFlag;
		std::vector<int> _posCellX;
		std::vector<int> _posCellY;
		std::vector<int> _signatureX;
		std::vector<int> _signatureY;
};

#endif /* PATCH_HXX_ */
