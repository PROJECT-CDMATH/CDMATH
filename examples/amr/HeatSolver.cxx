/*
 * HeatSolver.cxx
 *
 *  Created on: 3 juil. 2014
 *      Author: mekkas
 */

#include <MEDCouplingFieldDouble.hxx>
#include <MEDCouplingIMesh.hxx>
#include <MEDCouplingCartesianAMRMesh.hxx>
#include <MEDCouplingAMRAttribute.hxx>

#include "AMR.hxx"
#include "HeatSolver.hxx"

using namespace ParaMEDMEM;
using namespace std;


HeatSolver::HeatSolver(double finalTime,
                       int maximumNumberbOfIter,
                       bool isAlternatingDirection,
                       int numberOfGhostCells,
                       int frequencyOfPostTreatment,
                       double nu):IterativeProblem(finalTime,maximumNumberbOfIter,isAlternatingDirection,numberOfGhostCells,frequencyOfPostTreatment)
{
    _nu=nu;
}

HeatSolver::~HeatSolver()
{
}

double
HeatSolver::getDiffusion( void ) const
{
    return _nu;
}

void
HeatSolver::setDiffusion( double nu )
{
    _nu=nu;
}

ParaMEDMEM::MEDCouplingFieldDouble*
HeatSolver::initialConditions(const ParaMEDMEM::MEDCouplingIMesh* mesh) const
{
    int nbCells=mesh->getNumberOfCells();

    DataArrayDouble *baryCell = mesh->getBarycenterAndOwner() ;
    const double *coorBary=baryCell->getConstPointer();

    DataArrayDouble *array=DataArrayDouble::New();
    array->alloc(nbCells,1);
    double* vals=array->getPointer();
    int k=0;

    for (int i=0;i<nbCells;i++,k+=2)
    {
        double xb=coorBary[k];
        double yb=coorBary[k+1];
        vals[i] = 0.0;
        if (yb<1.)
            if (yb>0.5)
                if (xb<1.)
                    if (xb>0.5)
                        vals[i] = 2.0;
    }
    MEDCouplingFieldDouble* YY=MEDCouplingFieldDouble::New(ON_CELLS);
    YY->setName("YY") ;
    YY->setMesh(const_cast<MEDCouplingIMesh *>(mesh));
    YY->setArray(array);
    YY->setTime(0.,0,0);
    array->decrRef();
    baryCell->decrRef();
    return YY;
}

ParaMEDMEM::MEDCouplingFieldDouble*
HeatSolver::refinementCriterion(const ParaMEDMEM::MEDCouplingFieldDouble* field) const
{
    double triggeringVariation = 0.1;

    const MEDCouplingIMesh* mesh=dynamic_cast<const MEDCouplingIMesh *>(field->getMesh());
    MEDCouplingFieldDouble* field_WG=AMR::buildFieldWithGhostFromFieldWithoutGhost(getNumberOfGhostCells(),mesh,field);
    const double* yy=field_WG->getArray()->getConstPointer();


    int numberOfGhostCells=getNumberOfGhostCells();
    MEDCouplingIMesh* m1=mesh->buildWithGhost(numberOfGhostCells);
    vector<int> nxyz=m1->getCellGridStructure();
    int nx=nxyz[0];
    int ny=nxyz[1];
    m1->decrRef();
    int nbCells = mesh->getNumberOfCells();
    DataArrayDouble *array=DataArrayDouble::New();
    array->alloc(nbCells,1);
    double* vals=array->getPointer();

    double grad_max_YY=0.;
    for (int j=numberOfGhostCells;j<ny-numberOfGhostCells;j++)
    {
        for (int i=numberOfGhostCells;i<nx-numberOfGhostCells;i++)
        {
            int ij=i+j*nx;
            int ijWithoutGhost=(i-numberOfGhostCells)+(j-numberOfGhostCells)*(nx-2*numberOfGhostCells);
            int iminus1=ij-1;
            int iplus1=ij+1;
            int jminus1=ij-nx;
            int jplus1=ij+nx;
            double gradX = (yy[iplus1] - yy[iminus1]);
            double gradY = (yy[jplus1] - yy[jminus1]);
            grad_max_YY = max(grad_max_YY,abs(gradX)+abs(gradY));
            vals[ijWithoutGhost]=0;
        }
    }

    for (int j=numberOfGhostCells;j<ny-numberOfGhostCells;j++)
    {
        for (int i=numberOfGhostCells;i<nx-numberOfGhostCells;i++)
        {
            int ij=i+j*nx;
            int ijWithoutGhost=(i-numberOfGhostCells)+(j-numberOfGhostCells)*(nx-2*numberOfGhostCells);
            int iminus1=ij-1;
            int iplus1=ij+1;
            int jminus1=ij-nx;
            int jplus1=ij+nx;
            double gradX = (yy[iplus1] - yy[iminus1]);
            double gradY = (yy[jplus1] - yy[jminus1]);
            double err = (abs(gradX)+abs(gradY))/grad_max_YY;
            if(err>=triggeringVariation)
                vals[ijWithoutGhost]=1;
        }
    }

    MEDCouplingFieldDouble* FLAG=MEDCouplingFieldDouble::New(ON_CELLS);
    FLAG->setName("FLAG") ;
    MEDCouplingIMesh* mesh1=const_cast<MEDCouplingIMesh *>(mesh);
    FLAG->setMesh(mesh1);
    FLAG->setArray(array);
    FLAG->setTime(0.,0,0);
    array->decrRef();
    field_WG->decrRef();
    return FLAG;
}


double
HeatSolver::advancingTimeStep(
        int idir,
        double currentTime,
        ParaMEDMEM::MEDCouplingAMRAttribute* fields,
        const ParaMEDMEM::MEDCouplingCartesianAMRPatchGen* grid) const
{
    int numberOfGhostCells=getNumberOfGhostCells();
    MEDCouplingCartesianAMRMeshGen* mesh=const_cast<MEDCouplingCartesianAMRMeshGen *>(grid->getMesh());
    vector<double> dxyz=grid->getMesh()->getImageMesh()->getDXYZ();
    double nu=getDiffusion();
    double dt=computeDt(nu,dxyz);
    MEDCouplingIMesh* m1=grid->getMesh()->getImageMesh()->buildWithGhost(numberOfGhostCells);
    MEDCouplingFieldDouble* yyWithGhost_F=MEDCouplingFieldDouble::New(ON_CELLS);
    yyWithGhost_F->setName("YY") ;
    yyWithGhost_F->setMesh(m1);
    DataArrayDouble* yyWithGhost=const_cast<DataArrayDouble *>(fields->getFieldOn(mesh,"YY"));
    yyWithGhost_F->setArray(yyWithGhost);
    yyWithGhost_F->setTime(currentTime,0,0);
    yyWithGhost_F->checkCoherency();

    SolverHeat2D(numberOfGhostCells,grid->getMesh()->getImageMesh(),dt,yyWithGhost_F);

    std::copy(yyWithGhost_F->getArray()->getPointer(),yyWithGhost_F->getArray()->getPointer()+yyWithGhost_F->getArray()->getNumberOfTuples(),yyWithGhost->getPointer());
    yyWithGhost_F->decrRef();
    m1->decrRef();
    return dt;
}

void
HeatSolver::SolverHeat2D(int numberOfGhostCells,const MEDCouplingIMesh *imesh,double dt,MEDCouplingFieldDouble* yyWithGhost) const
{
    MEDCouplingFieldDouble* YY2_F=AMR::buildFieldWithoutGhostFromFieldWithGhost(numberOfGhostCells,imesh,yyWithGhost);
    DataArrayDouble* YY2=YY2_F->getArray();
    double* YY=YY2->getPointer();
    double nu=getDiffusion();
    vector<double> dxyz;
    dxyz=imesh->getDXYZ();

    const double *yy=yyWithGhost->getArray()->getConstPointer();

    MEDCouplingIMesh* m1=imesh->buildWithGhost(numberOfGhostCells);

    vector<int> nxyz=m1->getCellGridStructure();
    int nx=nxyz[0];
    int ny=nxyz[1];
    int nz;
    if (m1->getSpaceDimension()==2)
        nz=2*numberOfGhostCells+1;
    else
        nz=nxyz[2];

    int dim=imesh->getSpaceDimension();
    for (int k=numberOfGhostCells;k<nz-numberOfGhostCells;k++)
    {
        for (int j=numberOfGhostCells;j<ny-numberOfGhostCells;j++)
        {
            for (int i=numberOfGhostCells;i<nx-numberOfGhostCells;i++)
            {
                int ijkCWithGhost;
                if (dim==2)
                    ijkCWithGhost=i+j*nx;
                else
                    ijkCWithGhost=i+j*nx+k*nx*ny;
                int ijkCWithoutGhost=(i-numberOfGhostCells)+(j-numberOfGhostCells)*(nx-2*numberOfGhostCells)+(k-numberOfGhostCells)*(nx-2*numberOfGhostCells)*(ny-2*numberOfGhostCells);

                double ax=yy[ijkCWithGhost+1]-2*yy[ijkCWithGhost]+yy[ijkCWithGhost-1];
                double ay=yy[ijkCWithGhost+nx]-2*yy[ijkCWithGhost]+yy[ijkCWithGhost-nx];

                YY[ijkCWithoutGhost]+=(nu*dt/(dxyz[0]*dxyz[0]))*(ax) + (nu*dt/(dxyz[1]*dxyz[1]))*(ay);
            }
        }
    }

    MEDCouplingFieldDouble* YY3=AMR::buildFieldWithGhostFromFieldWithoutGhost(numberOfGhostCells,imesh,YY2_F);
    yyWithGhost->setArray(YY3->getArray());
    YY3->decrRef();
    YY2_F->decrRef();
    m1->decrRef();
}

double
HeatSolver::computeDt(double cfl, vector<double> dxyz) const
{
        double nu=getDiffusion();
        double dx2=dxyz[0]*dxyz[0];
        double dy2=dxyz[0]*dxyz[1];
        return dx2*dy2/( 2*nu*(dx2+dy2) );
}
