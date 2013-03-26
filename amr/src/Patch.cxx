/*
 * Patch.cxx
 *
 *  Created on: 4 fevr. 2013
 *      Author: am217485
 */

#include "Patch.hxx"

#include "MEDFileMesh.hxx"
#include "MEDCouplingFieldDouble.hxx"

#include "Cell.hxx"
#include "Node.hxx"

#include <cmath>
#include <algorithm>

using namespace ParaMEDMEM;
using namespace std;

Patch::Patch()
{
	_indexCellLeftLowCorner=-1;
	_indexCellRightLowCorner=-1;
	_indexCellLeftHighCorner=-1;
	_indexCellRightHighCorner=-1;
	_nX=-1;
	_nY=-1;
	_coefRefinementX=-1;
	_coefRefinementY=-1;
	_level=-1;
	_index=-1;
	_verbose=true;
	_indexCorners[0]=-1;
	_indexCorners[1]=-1;
	_indexCorners[2]=-1;
	_indexCorners[3]=-1;
	_numberOfCellsFlags=0;
	_efficiency=-1.;
}

Patch::Patch(const int *indexCorners)
{
	_indexCellLeftLowCorner=-1;
	_indexCellRightLowCorner=-1;
	_indexCellLeftHighCorner=-1;
	_indexCellRightHighCorner=-1;
	_coefRefinementX=-1;
	_coefRefinementY=-1;
	_level=-1;
	_index=-1;
	_verbose=true;
	_nX=indexCorners[2]-indexCorners[0]+1;
	_nY=indexCorners[3]-indexCorners[1]+1;
	_indexCorners[0]=indexCorners[0];
	_indexCorners[1]=indexCorners[1];
	_indexCorners[2]=indexCorners[2];
	_indexCorners[3]=indexCorners[3];
	_numberOfCellsFlags=0;
	_efficiency=-1.;
}

//----------------------------------------------------------------------
Patch::Patch( const Patch & patch )
//----------------------------------------------------------------------
{
	_indexCellLeftLowCorner=patch.getIndexCellLeftLowCorner();
	_indexCellRightLowCorner=patch.getIndexCellRightLowCorner();
	_indexCellLeftHighCorner=patch.getIndexCellLeftHighCorner();
	_indexCellRightHighCorner=patch.getIndexCellRightHighCorner();
	_coefRefinementX=patch.getCoefRefinementX();
	_coefRefinementY=patch.getCoefRefinementY();
	_level=patch.getLevel();
	_index=patch.getIndex();
	_nX=patch.getNx();
	_nY=patch.getNy();
	_verbose=patch.isVerbose();
	copy(patch.getIndexCorners(),patch.getIndexCorners()+4,_indexCorners) ;
	_mesh=patch.getMesh() ;
	_numberOfCellsFlags=patch.getNumberOfCellsFlags();
	_efficiency=patch.getEfficiency();
	_fieldFlag=patch.getFieldFlag() ;
}

Patch::Patch(const Mesh& mesh, const Field& fieldFlag)
{
	vector<int> posCellX;
	vector<int> posCellY;

	int nbOfCells=mesh.getNumberOfCells();
	for (int ic=0;ic<nbOfCells;ic++)
	{
		int i=ic%mesh.getNx();
		int j=ic/mesh.getNx();
		if (abs(fieldFlag(ic)-1.0)<1.E-15)
		{
			posCellX.push_back(i) ;
			posCellY.push_back(j) ;
		}
	}
	int indexCorners[4];
	indexCorners[0]=*min_element(posCellX.begin(),posCellX.end());
	indexCorners[1]=*min_element(posCellY.begin(),posCellY.end());
	indexCorners[2]=*max_element(posCellX.begin(),posCellX.end());
	indexCorners[3]=*max_element(posCellY.begin(),posCellY.end());
	_indexCellLeftLowCorner=-1;
	_indexCellRightLowCorner=-1;
	_indexCellLeftHighCorner=-1;
	_indexCellRightHighCorner=-1;
	_coefRefinementX=-1;
	_coefRefinementY=-1;
	_level=-1;
	_index=-1;
	_verbose=true;
	_nX=indexCorners[2]-indexCorners[0]+1;
	_nY=indexCorners[3]-indexCorners[1]+1;
	_indexCorners[0]=indexCorners[0];
	_indexCorners[1]=indexCorners[1];
	_indexCorners[2]=indexCorners[2];
	_indexCorners[3]=indexCorners[3];
	_numberOfCellsFlags=0;
	_efficiency=-1.;
}

const Patch&
Patch::operator= ( const Patch& patch)
{
	_indexCellLeftLowCorner=patch.getIndexCellLeftLowCorner();
	_indexCellRightLowCorner=patch.getIndexCellRightLowCorner();
	_indexCellLeftHighCorner=patch.getIndexCellLeftHighCorner();
	_indexCellRightHighCorner=patch.getIndexCellRightHighCorner();
	_coefRefinementX=patch.getCoefRefinementX();
	_coefRefinementY=patch.getCoefRefinementY();
	_level=patch.getLevel();
	_index=patch.getIndex();
	_nX=patch.getNx();
	_nY=patch.getNy();
	copy(patch.getIndexCorners(),patch.getIndexCorners()+4,_indexCorners) ;
	_mesh=patch.getMesh() ;
	_efficiency=patch.getEfficiency();
	_fieldFlag=patch.getFieldFlag() ;
	_numberOfCellsFlags=patch.getNumberOfCellsFlags();
	return *this;
}

Patch::~Patch()
{
}

void
Patch::computeMesh(const Mesh& mesh)
{
	int nXmesh=mesh.getNx();
	_indexCellLeftLowCorner=_indexCorners[1]*nXmesh+_indexCorners[0];
    _indexCellRightLowCorner=_indexCorners[1]*nXmesh+_indexCorners[2];
    _indexCellLeftHighCorner=_indexCorners[3]*nXmesh+_indexCorners[0];
    _indexCellRightHighCorner=_indexCorners[3]*nXmesh+_indexCorners[2];
    _coefRefinementX=1;
    _coefRefinementY=1;

    Cell cinf=mesh.getCell(_indexCellLeftLowCorner);
    double xinf=1.E30;
    double yinf=1.E30;
    double xsup=-1.E30;
    double ysup=-1.E30;
    for (int i=0;i<cinf.getNumberOfNodes();i++)
    {
    	int idNode=cinf.getNodesId()[i];
    	double x=mesh.getNode(idNode).getPoint().x();
    	double y=mesh.getNode(idNode).getPoint().y();
    	if (x-xinf<1.E-15)
    		xinf=x;
    	if (y-yinf<1.E-15)
    		yinf=y;
    }
    Cell csup=mesh.getCell(_indexCellRightHighCorner);
    for (int i=0;i<csup.getNumberOfNodes();i++)
    {
    	int idNode=csup.getNodesId()[i];
    	double x=mesh.getNode(idNode).getPoint().x();
    	double y=mesh.getNode(idNode).getPoint().y();
    	if (x-xsup>1.E-15)
    		xsup=x;
    	if (y-ysup>1.E-15)
    		ysup=y;
    }
	_mesh=Mesh(xinf,xsup,_nX,yinf,ysup,_nY);
}

void
Patch::computeMesh(const Mesh& mesh, const int coefRefinementX, const int coefRefinementY )
{
	int nXmesh=mesh.getNx();
	_indexCellLeftLowCorner=_indexCorners[1]*nXmesh+_indexCorners[0];
    _indexCellRightLowCorner=_indexCorners[1]*nXmesh+_indexCorners[2];
    _indexCellLeftHighCorner=_indexCorners[3]*nXmesh+_indexCorners[0];
    _indexCellRightHighCorner=_indexCorners[3]*nXmesh+_indexCorners[2];
    _coefRefinementX=coefRefinementX;
    _coefRefinementY=coefRefinementY;

    Cell cinf=mesh.getCell(_indexCellLeftLowCorner);
    double xinf=1.E30;
    double yinf=1.E30;
    double xsup=-1.E30;
    double ysup=-1.E30;
    for (int i=0;i<cinf.getNumberOfNodes();i++)
    {
    	int idNode=cinf.getNodesId()[i];
    	double x=mesh.getNode(idNode).getPoint().x();
    	double y=mesh.getNode(idNode).getPoint().y();
    	if (x-xinf<1.E-15)
    		xinf=x;
    	if (y-yinf<1.E-15)
    		yinf=y;
    }
    Cell csup=mesh.getCell(_indexCellRightHighCorner);
    for (int i=0;i<csup.getNumberOfNodes();i++)
    {
    	int idNode=csup.getNodesId()[i];
    	double x=mesh.getNode(idNode).getPoint().x();
    	double y=mesh.getNode(idNode).getPoint().y();
    	if (x-xsup>1.E-15)
    		xsup=x;
    	if (y-ysup>1.E-15)
    		ysup=y;
    }
	_mesh=Mesh(xinf,xsup,_coefRefinementX*_nX,yinf,ysup,_coefRefinementY*_nY);
    _nX = _coefRefinementX*_nX ;
    _nY = _coefRefinementY*_nY ;
}

void
Patch::computeFieldFlag(const Field& fieldFlag)
{
    _fieldFlag = Field("Field_k",fieldFlag.getTypeOfField(),getMesh(),1) ;
	int nbOfCells=getNumberOfCells();
    int nX=getNx();
    int nxG=fieldFlag.getMesh().getNx();

    DataArrayInt *id=DataArrayInt::New();
    id->alloc(nbOfCells,1);
    IntTab idi(nbOfCells);
	for (int ic=0;ic<nbOfCells;ic++)
    {
    	int i=ic%nX;
    	int j=ic/nX;
    	int icg=(getIndexCellLeftLowCorner()+i)+(j*nxG);
    	idi[ic]=icg;
    }

	copy(idi.getValues(),idi.getValues()+nbOfCells,id->getPointer());
	_fieldFlag.setField(fieldFlag.getField()->buildSubPart(id));
	id->decrRef();

    computeNumberOfCellsFlags() ;
    _efficiency = float(getNumberOfCellsFlags()) / float(nbOfCells) ;
    findPosCellsFlags() ;
    computeSignature();
}

void
Patch::computeNumberOfCellsFlags(void)
{
	const double* values=_fieldFlag.getField()->getArray()->getConstPointer();
	_numberOfCellsFlags=0;
	double sum=0.0;
	int nbOfCells=getNumberOfCells();
	for(int i=0; i<nbOfCells; i++)
		sum+=values[i];
	_numberOfCellsFlags=int(sum);
}

void
Patch::dissectPatch(const Mesh& mesh, const Field& fieldFlag, const unsigned int minCellDirection, const int *cut,
                    int *rectLeft, int *rectRight) const
{
    int cutDim = cut[2] ;
    int cutPlace = cut[1] ;
    for ( int h=0;h<2;h++)
    {
        int rect[4] ;
        int rectRef[4] ;
        rect[0]=_indexCorners[0];
        rect[1]=_indexCorners[1];
        rect[2]=_indexCorners[2];
        rect[3]=_indexCorners[3];
        if (h == 0 )
        	rect[cutDim+2] = cutPlace ;
        else if ( h == 1)
        	rect[cutDim] = cutPlace+1 ;
        rectRef[0] = rect[0] ;
        rectRef[1] = rect[1] ;
        rectRef[2] = rect[0] ;
        rectRef[3] = rect[1] ;
        Patch patch(rect);
        patch.computeMesh(mesh);
        patch.computeFieldFlag(fieldFlag);
        vector<int> posX = patch.getPosCellX();
        vector<int> posY = patch.getPosCellY();
        rect[0] = rectRef[0]+*min_element(posX.begin(),posX.end());
        rect[1] = rectRef[1]+*min_element(posY.begin(),posY.end()) ;
        rect[2] = rectRef[2]+*max_element(posX.begin(),posX.end()) ;
        rect[3] = rectRef[3]+*max_element(posY.begin(),posY.end()) ;

        if (h == 0 )
        {
        	rectLeft[0]=rect[0];
        	rectLeft[1]=rect[1];
        	rectLeft[2]=rect[2];
        	rectLeft[3]=rect[3];
        }
        else if ( h == 1)
        {
        	rectRight[0]=rect[0];
        	rectRight[1]=rect[1];
        	rectRight[2]=rect[2];
        	rectRight[3]=rect[3];
        }
    }

    if (isVerbose())
    {
        int nYLeft = rectLeft[2] - rectLeft[0] + 1 ;
        int nXLeft = rectLeft[3] - rectLeft[1] + 1 ;
        int nYRight = rectRight[2] - rectRight[0] + 1 ;
        int nXRight = rectRight[3] - rectRight[1] + 1 ;

        cout << "- New boxes after tightening bounding boxes : " << endl;
        cout << "- Left patch coordinates ["
             << rectLeft[0] << "," << rectLeft[1] << ","
             << rectLeft[2] << "," << rectLeft[3] << "]"
             << " size = " << nYLeft << " x " << nXLeft << endl;
        cout << "- Right patch coordinates ["
             << rectRight[0] << "," << rectRight[1] << ","
             << rectRight[2] << "," << rectRight[3] << "]"
             << " size = " << nYRight << " x " << nXRight << endl;
    }
    return ;
    // verification de taille minimum des patchs
    int dim=2 ;
    for ( int d=0;d<dim;d++)
    {
        for ( int h=0;h<2;h++)
        {
            int rect[4] ;
            if (h == 0 )
            	copy(rectLeft,rectLeft+4,rect) ;
            else if ( h == 1)
            	copy(rectRight,rectRight+4,rect) ;

            int slack = minCellDirection - (rect[d+dim]-rect[d]+1);
            if ( slack > 0 )
            {
                int ext_left = floor(slack/2);
                int ext_right = slack - ext_left;
                rect[d] = rect[d] - ext_left ;
                rect[d+dim] = rect[d+dim] + ext_right ;

                if ( rect[d] < _indexCorners[d] )
                {
                	rect[d+2] = rect[d+2] + (_indexCorners[d] - rect[d]);
                	rect[d+1] = rect[d+1] + (_indexCorners[d] - rect[d]);
                	rect[d] = rect[d] + (_indexCorners[d] - rect[d]);
                }
                if ( rect[d+dim]> _indexCorners[d+dim] )
                {
                	rect[d] = rect[d] + (_indexCorners[d+dim] - rect[d+dim]);
                	rect[d+1] = rect[d+1] + (_indexCorners[d+dim] - rect[d+dim]);
                	rect[d+2] = rect[d+2] + (_indexCorners[d+dim] - rect[d+dim]);
                }
                if (h == 0 )
                {
                	rectLeft[0]=rect[0];
                	rectLeft[1]=rect[1];
                	rectLeft[2]=rect[2];
                	rectLeft[3]=rect[3];
                }
                else if ( h == 1)
                {
                	rectRight[0]=rect[0];
                	rectRight[1]=rect[1];
                	rectRight[2]=rect[2];
                	rectRight[3]=rect[3];
                }
            }
        }
    }

    if (isVerbose())
    {
        int nYLeft = rectLeft[2] - rectLeft[0] + 1 ;
        int nXLeft = rectLeft[3] - rectLeft[1] + 1 ;
        int nYRight = rectRight[2] - rectRight[0] + 1 ;
        int nXRight = rectRight[3] - rectRight[1] + 1 ;

        cout << "- New boxes after tightening bounding boxes : " << endl;
        cout << "- Left patch coordinates ["
             << rectLeft[0] << "," << rectLeft[1] << ","
             << rectLeft[2] << "," << rectLeft[3] << "]"
             << " size = " << nYLeft << " x " << nXLeft << endl;
        cout << "- Right patch coordinates ["
             << rectRight[0] << "," << rectRight[1] << ","
             << rectRight[2] << "," << rectRight[3] << "]"
             << " size = " << nYRight << " x " << nXRight << endl;
    }
}

void
Patch::dissectBigPatch (const Mesh& mesh, const Field& fieldFlag, const unsigned int minCellDirection,
		                const unsigned int big_dims, const int dissect_direction, int *cut ) const
{
    int cut_found = 0 ;
    int cut_place = -1 ;
    int cut_dim = dissect_direction ;
    float * ratio = NULL ;
    float * ratio_inner = NULL ;
    if(isVerbose())
    	cout << "- Box too big" << endl;

    ratio = new float [big_dims-1];
    for(unsigned int id=0; id<big_dims-1; id++)
    {
        float efficiency[2] ;
        for(int h=0; h<2; h++)
        {
            int rect_h[4] ;
        	copy(getIndexCorners(),getIndexCorners()+4,rect_h) ;
            if (h == 0 )
                rect_h[dissect_direction+2] = _indexCorners[dissect_direction]+id ;
            else if ( h == 1)
                rect_h[dissect_direction] =  _indexCorners[dissect_direction]+id+1;

            Patch patch_h(rect_h);
            patch_h.computeMesh(mesh);
            patch_h.computeFieldFlag(fieldFlag);

            int nb_cells_h ;
            if ( dissect_direction == 0 )
                nb_cells_h = patch_h.getNx() ;
            else
                nb_cells_h = patch_h.getNy() ;

            int nb_cells_flag_h = patch_h.getNumberOfCellsFlags();
            efficiency[h] = float (nb_cells_flag_h) / float(nb_cells_h) ;
        }
        ratio[id] = max(efficiency[0],efficiency[1])/
                    min(efficiency[0],efficiency[1]) ;
    }

    int dim_ratio_inner = big_dims-1-2*(minCellDirection-1) ;
    ratio_inner = new float [dim_ratio_inner];
    float min_ratio = 1.E10 ;
    int index_min = -1 ;
    for(int i=0; i<dim_ratio_inner; i++)
    {
        if ( ratio[minCellDirection-1+i] < min_ratio )
        {
            min_ratio = ratio[minCellDirection-1+i] ;
            index_min = i+minCellDirection ;
        }
    }
    cut_found = 1 ;
    cut_place = index_min + _indexCorners[dissect_direction] - 1 ;
    cut[0] = cut_found ;
    cut[1] = cut_place ;
    cut[2] = cut_dim ;
    delete [] ratio ;
    delete [] ratio_inner ;
}

void
Patch::findHole(const int dissectDirection, const unsigned int minCellDirection, int *cut) const
{
    vector<int> signature;
    int sortedDims[2] ;
    if ( dissectDirection == 0 )
    {
    	sortedDims[0] = 0 ;
    	sortedDims[1] = 1 ;
    }
    else if ( dissectDirection == 1 )
    {
    	sortedDims[0] = 1 ;
    	sortedDims[1] = 0 ;
    }

    cut[0] = 0 ;
    cut[1] = -1 ;
    cut[2] = -1 ;
    int dim = 2 ;

    for (int id=0;id<dim;id++)
    {
        if ( sortedDims[id] == 0 )
            signature = _signatureY ;
        else if ( sortedDims[id] == 1 )
            signature = _signatureX ;
        vector<int> hole ;
        vector<float> distance ;
        unsigned int len = signature.size();
        for (unsigned int i=0;i<len;i++)
            if (signature[i]==0)
                if ( len>= 2*minCellDirection && i >= minCellDirection-1 && i <= len-minCellDirection-1 )
                    hole.push_back(i) ;
        if (hole.size()>0)
        {
            float center = (len/2.0)+0.5;
            for (unsigned int i=0;i<hole.size();i++)
                distance.push_back(fabs(hole[i]+1+0.5-center));

            float distanceMin=*min_element(distance.begin(),distance.end()) ;
            int posDistanceMin=find(distance.begin(),distance.end(),distanceMin)-distance.begin()-1;

            cut[0] = 1 ;
            cut[1] = hole[posDistanceMin] + _indexCorners[dissectDirection] + 1 ;
            cut[2] = dissectDirection ;
            if(isVerbose())
            	cout << "Found hole" << endl;
            hole.clear() ;
            distance.clear() ;
            return ;
        }
    }
}

void
Patch::findInflection( const unsigned int minCellDirection, int *cut ) const
{
    int sign ;
    int dim = 2 ;

    vector<int> zeroCrossDims(2) ;
    vector<int> zeroCrossVals(2) ;
    zeroCrossDims[0]=-1;
    zeroCrossDims[1]=-1;
    zeroCrossVals[0]=-1;
    zeroCrossVals[1]=-1;

    cut[0] = 0 ;
    cut[1] = -1 ;
    cut[2] = -1 ;

    for (int id=0;id<dim;id++)
    {
        vector<int> signature ;
        if ( id == 0 )
            signature = _signatureX ;
        else if ( id == 1 )
            signature = _signatureY ;

        vector<int> derivate_second_order ;
        vector<int> gradient_absolute ;
        vector<int> signe_change ;
        vector<int> zero_cross ;
        vector<int> edge ;
        vector<int> max_cross_list ;
        vector<float> distance ;

        for (unsigned int i=1;i<signature.size()-1;i++)
            derivate_second_order.push_back(signature[i-1]-2*signature[i]+signature[i+1]) ;

        // Gradient absolute value
        for ( unsigned int i=1;i<derivate_second_order.size();i++)
            gradient_absolute.push_back(fabs(derivate_second_order[i]-derivate_second_order[i-1])) ;
        for (unsigned int i=0;i<derivate_second_order.size()-1;i++)
        {
            if (derivate_second_order[i]*derivate_second_order[i+1] < 0 )
                sign = -1 ;
            if (derivate_second_order[i]*derivate_second_order[i+1] > 0 )
                sign = 1 ;
            if (derivate_second_order[i]*derivate_second_order[i+1] == 0 )
                sign = 0 ;
            if ( sign==0 || sign==-1 )
                if ( i >= minCellDirection-2 && i <= signature.size()-minCellDirection-2 )
                {
                    zero_cross.push_back(i) ;
                    edge.push_back(gradient_absolute[i]) ;
                }
            signe_change.push_back(sign) ;
        }
        if ( zero_cross.size() > 0 )
        {
            int max_cross=*max_element(edge.begin(),edge.end()) ;
            for (unsigned int i=0;i<edge.size();i++)
                if (edge[i]==max_cross)
                    max_cross_list.push_back(zero_cross[i]+1) ;

            float center = (signature.size()/2.0)+0.5;
            for (unsigned int i=0;i<max_cross_list.size();i++)
                distance.push_back(fabs(max_cross_list[i]+1+0.5-center));

            float distance_min=*min_element(distance.begin(),distance.end()) ;
            int pos_distance_min=find(distance.begin(),distance.end(),distance_min)-distance.begin();
            int best_place = max_cross_list[pos_distance_min] + _indexCorners[id] ;
            if ( max_cross >=0 )
            {
            	zeroCrossDims[id] = best_place ;
            	zeroCrossVals[id] = max_cross ;
            }
        }
        derivate_second_order.clear() ;
        gradient_absolute.clear() ;
        signe_change.clear() ;
        zero_cross.clear() ;
        edge.clear() ;
        max_cross_list.clear() ;
        distance.clear() ;
        signature.clear() ;
    }

    if ( zeroCrossDims[0]!=-1 || zeroCrossDims[1]!=-1  )
    {
        int max_cross_dims = *max_element(zeroCrossVals.begin(),zeroCrossVals.end()) ;

        if (zeroCrossVals[0]==max_cross_dims &&  zeroCrossVals[1]==max_cross_dims )
        {
            int nl_left = _indexCorners[2] - _indexCorners[0] + 1 ;
            int nc_left = _indexCorners[3] - _indexCorners[1] + 1 ;
            if ( nl_left >=  nc_left )
                max_cross_dims = 0 ;
            else
                max_cross_dims = 1 ;
        } else
            max_cross_dims=find(zeroCrossVals.begin(),zeroCrossVals.end(),max_cross_dims)-zeroCrossVals.begin();
        cut[0] = 1 ;
        cut[1] = zeroCrossDims[max_cross_dims] ;
        cut[2] = max_cross_dims ;
        if(isVerbose())
        	cout << "Inflection point Found" << endl;
    }
}

void
Patch::findPosCellsFlags(void)
{
    int nbOfCells=getNumberOfCells();
    for (int ic=0;ic<nbOfCells;ic++)
    {
    	int i=ic%getNx();
    	int j=ic/getNx();
    	if (abs(_fieldFlag(ic)-1.0)<1.E-15)
        {
    		_posCellX.push_back(i) ;
            _posCellY.push_back(j) ;
        }
    }
}

void
Patch::computeSignature ()
{
	int nX=getNx();
	int nY=getNy();

    for(int j=0; j<nY; j++)
	{
		double Sumi= 0 ;
		for(int i=0; i<nX; i++)
		{
			int ic=j*nX+i;
			Sumi+=_fieldFlag(ic) ;
		}
		_signatureY.push_back(Sumi) ;
	}

	for(int i=0; i<nX; i++)
	{
		double Sumj= 0 ;
		for(int j=0; j<nY; j++)
		{
			int ic=j*nX+i;
			Sumj+=_fieldFlag(ic) ;
		}
		_signatureX.push_back(Sumj) ;
	}
}

void
Patch::infoPatch(void) const
{
	cout << "-----------------------------------------------------" << endl;
	if ( getIndex()==-1 )
		cout << "                 Information patch                 " << endl;
	else
		cout << "                 Information patch " << getIndex() << endl;
	cout << "-----------------------------------------------------" << endl;
	cout << "- coordinates [" << getIndexCorners()[0] << "," << getIndexCorners()[1] << "," << getIndexCorners()[2] << "," << getIndexCorners()[3] << "]" << endl;
    cout << "- number of cells = [" << getNx() << "," << getNy() << "] = " << getNumberOfCells() << endl;
    cout << "- efficiency = " << getEfficiency() << "%" << endl;
}

void
Patch::setIndex(const int index)
{
	_index=index;
}

bool
Patch::isVerbose( void ) const
{
	return _verbose;
}

Mesh
Patch::getMesh ( void ) const
{
	return _mesh;
}

Field
Patch::getFieldFlag( void ) const
{
	return _fieldFlag;
}

Cell&
Patch::getCellLeftLowCorner ( void ) const
{
	return _mesh.getCell(_indexCellLeftLowCorner);
}

Cell&
Patch::getCellLeftHighCorner ( void ) const
{
	return _mesh.getCell(_indexCellLeftHighCorner);
}

Cell&
Patch::getCellRightLowCorner( void ) const
{
	return _mesh.getCell(_indexCellRightLowCorner);
}

Cell&
Patch::getCellRightHighCorner( void ) const
{
	return _mesh.getCell(_indexCellRightHighCorner);
}

int
Patch::getIndexCellLeftLowCorner( void ) const
{
	return _indexCellLeftLowCorner;
}

int
Patch::getIndexCellLeftHighCorner( void ) const
{
	return _indexCellLeftHighCorner;
}

int
Patch::getIndexCellRightLowCorner ( void ) const
{
	return _indexCellRightLowCorner;
}

int
Patch::getIndexCellRightHighCorner ( void ) const
{
	return _indexCellRightHighCorner;
}

int
Patch::getNumberOfCellsFlags( void ) const
{
	return _numberOfCellsFlags ;
}

float
Patch::getEfficiency() const
{
	return _efficiency ;
}

int
Patch::getNumberOfCells( void ) const
{
	return getMesh().getNumberOfCells();
}

vector<int>
Patch::getPosCellX( void ) const
{
	return _posCellX ;
}

vector<int>
Patch::getPosCellY( void ) const
{
	return _posCellY ;
}

vector<int>
Patch::getSignatureX(void) const
{
	return _signatureX;
}

vector<int>
Patch::getSignatureY(void) const
{
	return _signatureY;
}

int
Patch::getNx ( void ) const
{
	return _nX;
}

int
Patch:: getNy ( void ) const
{
	return _nY;
}

int
Patch::getCoefRefinementX ( void ) const
{
	return _coefRefinementX;
}

int
Patch::getCoefRefinementY ( void ) const
{
	return _coefRefinementY;
}

int
Patch::getLevel ( void ) const
{
	return _level;
}

int
Patch::getIndex ( void ) const
{
	return _index;
}

const int*
Patch::getIndexCorners ( void ) const
{
	return _indexCorners;
}
