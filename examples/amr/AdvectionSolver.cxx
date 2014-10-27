/*
 * AdvectionSolver.cxx
 *
 *  Created on: 3 juil. 2014
 *      Author: mekkas
 */

#include <MEDCouplingFieldDouble.hxx>
#include <MEDCouplingIMesh.hxx>
#include <MEDCouplingCartesianAMRMesh.hxx>
#include <MEDCouplingAMRAttribute.hxx>

#include "AMR.hxx"

#include "AdvectionSolver.hxx"

using namespace ParaMEDMEM;
using namespace std;


AdvectionSolver::AdvectionSolver(double finalTime,
                                 int maximumNumberbOfIter,
                                 bool isAlternatingDirection,
                                 int numberOfGhostCells,
                                 int frequencyOfPostTreatment,
                                 double cfl):IterativeProblem(finalTime,maximumNumberbOfIter,isAlternatingDirection,numberOfGhostCells,frequencyOfPostTreatment)
{
    _cfl=cfl;
}

AdvectionSolver::~AdvectionSolver()
{
}

double
AdvectionSolver::getCfl( void ) const
{
    return _cfl;
}

void
AdvectionSolver::setCfl( double cfl )
{
    _cfl=cfl;
}

ParaMEDMEM::MEDCouplingFieldDouble*
AdvectionSolver::initialConditions(const ParaMEDMEM::MEDCouplingIMesh* mesh) const
{
    double rayon=0.15;
    double xcentre=0.5;
    double ycentre=0.75;
    int nbCells=mesh->getNumberOfCells();
    DataArrayDouble *baryCell = mesh->getBarycenterAndOwner() ;
    const double *coorBary=baryCell->getConstPointer();
    DataArrayDouble *array=DataArrayDouble::New();
    array->alloc(nbCells,1);
    double* vals=array->getPointer();
    int k=0;
    for (int i=0;i<nbCells;i++)
    {
        double xb=coorBary[k];
        double yb=coorBary[k+1];

        double valX=(xb-xcentre)*(xb-xcentre);
        double valY=(yb-ycentre)*(yb-ycentre);
        double val=sqrt(valX+valY);
        if (val<rayon)
            vals[i] = 1.0;
        else
            vals[i] = 0.0;
        k+=2;
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
AdvectionSolver::refinementCriterion(const ParaMEDMEM::MEDCouplingFieldDouble* field) const
{
    double triggeringVariation = 0.1;

    const MEDCouplingIMesh* mesh=dynamic_cast<const MEDCouplingIMesh *>(field->getMesh());
    MEDCouplingFieldDouble* field_WG=AMR::buildFieldWithGhostFromFieldWithoutGhost(getNumberOfGhostCells(),mesh,field);
    const double* yy=field_WG->getArray()->getConstPointer();


    int numberOfCellsGhost=getNumberOfGhostCells();
    MEDCouplingIMesh* m1=mesh->buildWithGhost(numberOfCellsGhost);
    vector<int> nxyz=m1->getCellGridStructure();
    int nx=nxyz[0];
    int ny=nxyz[1];
    m1->decrRef();
    int nbCells = mesh->getNumberOfCells();
    DataArrayDouble *array=DataArrayDouble::New();
    array->alloc(nbCells,1);
    double* vals=array->getPointer();

    double grad_max_YY=0.;
    for (int j=numberOfCellsGhost;j<ny-numberOfCellsGhost;j++)
    {
        for (int i=numberOfCellsGhost;i<nx-numberOfCellsGhost;i++)
        {
            int ij=i+j*nx;
            int ijWithoutGhost=(i-numberOfCellsGhost)+(j-numberOfCellsGhost)*(nx-2*numberOfCellsGhost);
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

    for (int j=numberOfCellsGhost;j<ny-numberOfCellsGhost;j++)
    {
        for (int i=numberOfCellsGhost;i<nx-numberOfCellsGhost;i++)
        {
            int ij=i+j*nx;
            int ijWithoutGhost=(i-numberOfCellsGhost)+(j-numberOfCellsGhost)*(nx-2*numberOfCellsGhost);
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
    /* avoir avec A.GEAY : fuite mémoire */
//  FLAG->writeVTK("FLAGAMR");
    field_WG->decrRef();
    return FLAG;
}

MEDCouplingFieldDouble*
AdvectionSolver::computeVelocity(const MEDCouplingIMesh* mesh, double currentTime, double finalTime) const
{
    int nbCells = mesh->getNumberOfCells();
    int dim=mesh->getSpaceDimension();
    DataArrayDouble *array=DataArrayDouble::New();
    array->alloc(nbCells,dim);
    double* vals=array->getPointer();

    DataArrayDouble* barry=mesh->getBarycenterAndOwner();
    const double *coorBary=barry->getConstPointer();

    double timePeriod=3.;
    double speedAmplifier = cos(2*M_PI*currentTime/timePeriod);
    int k=0;
    for (int ic=0;ic<nbCells;ic++,k+=dim)
    {
        double xb=coorBary[k];
        double yb=coorBary[k+1];
        if (dim==3)
        {
            double zb=coorBary[k+2];
            vals[k] = speedAmplifier * 2 * sin(M_PI*xb)*sin(M_PI*xb) * sin(2.*M_PI*yb) * sin(2.*M_PI*zb);
            vals[k+1] = speedAmplifier * (-1) * sin(M_PI*yb)*sin(M_PI*yb) * sin(2.*M_PI*xb) * sin(2.*M_PI*zb);
            vals[k+2] = speedAmplifier * (-1) * sin(M_PI*zb)*sin(M_PI*zb) * sin(2.*M_PI*xb) * sin(2.*M_PI*yb);
        }
        if (dim==2)
        {
            vals[k] = speedAmplifier * (-1) * sin(M_PI*xb)*sin(M_PI*xb) * sin(2*M_PI*yb);
            vals[k+1] = speedAmplifier * sin(M_PI*yb)*sin(M_PI*yb) * sin(2*M_PI*xb);
        }
    }

    MEDCouplingFieldDouble* velocity=MEDCouplingFieldDouble::New(ON_CELLS);
    velocity->setName("Velocity") ;
    velocity->setMesh(const_cast<MEDCouplingIMesh *>(mesh));
    velocity->setArray(array);
    velocity->setTime(currentTime,0,0);
    velocity->writeVTK("velocity");
    velocity->checkCoherency();
    array->decrRef();
    barry->decrRef();
    return velocity;
}

MEDCouplingFieldDouble*
AdvectionSolver::computeFluxUpwind(int direction,
                               double dspace,
                               int numberOfCellsGhost,
                               const MEDCouplingIMesh* mesh,
                               const MEDCouplingFieldDouble* velocity,
                               const MEDCouplingFieldDouble* YY_WITH_GHOST) const
{
    //velocity without ghost
    //YY with ghost

    int nbCells = mesh->getNumberOfCells();
    DataArrayDouble *array=DataArrayDouble::New();
    array->alloc(nbCells,2);
    for(int i=0;i<nbCells;i++)
    {
        array->setIJ(i,0,0.);
        array->setIJ(i,1,0.);
    }
//  double* vals=array->getPointer();
    MEDCouplingIMesh* m1=mesh->buildWithGhost(numberOfCellsGhost);

    vector<int> nxyz=m1->getCellGridStructure();
    int nx=nxyz[0];
    int ny=nxyz[1];
    int nz;
    if (m1->getSpaceDimension()==2)
        nz=2*numberOfCellsGhost+1;
    else
        nz=nxyz[2];
    m1->decrRef();
    const double *yy=YY_WITH_GHOST->getArray()->getConstPointer();

    int saut=0;
    if (direction==0)
        saut=1;
    else if (direction==1)
        saut=nx;
    else if (direction==2)
        saut=nx*ny;

    int ijkCWithGhost;
    int dim=mesh->getSpaceDimension();
    for (int k=numberOfCellsGhost;k<nz-numberOfCellsGhost;k++)
    {
        for (int j=numberOfCellsGhost;j<ny-numberOfCellsGhost;j++)
        {
            for (int i=numberOfCellsGhost;i<nx-numberOfCellsGhost;i++)
            {
                if (dim==2)
                    ijkCWithGhost=i+j*nx;
                else
                    ijkCWithGhost=i+j*nx+k*nx*ny;
                int ijkCWithoutGhost=(i-numberOfCellsGhost)+(j-numberOfCellsGhost)*(nx-2*numberOfCellsGhost)+(k-numberOfCellsGhost)*(nx-2*numberOfCellsGhost)*(ny-2*numberOfCellsGhost);
                double u_moyen=velocity->getIJ(ijkCWithoutGhost,direction);
                if (u_moyen>0.)
                {
                    array->setIJ(ijkCWithoutGhost,0,u_moyen*yy[ijkCWithGhost-saut]);
                    array->setIJ(ijkCWithoutGhost,1,u_moyen*yy[ijkCWithGhost]);
                }else if (u_moyen<0.)
                {
                    array->setIJ(ijkCWithoutGhost,0,u_moyen*yy[ijkCWithGhost]);
                    array->setIJ(ijkCWithoutGhost,1,u_moyen*yy[ijkCWithGhost+saut]);
                }
            }
        }
    }

    MEDCouplingFieldDouble* FLUXES=MEDCouplingFieldDouble::New(ON_CELLS);
    FLUXES->setName("FLUXES") ;
    FLUXES->setMesh(const_cast<MEDCouplingIMesh *>(mesh));
    FLUXES->setArray(array);
    FLUXES->setTime(0.,0,0);
    array->decrRef();
    return FLUXES;
}

MEDCouplingFieldDouble*
AdvectionSolver::computeFluxDL(int direction,
                               double dspace,
                               double dt,
                               int numberOfCellsGhost,
                               const MEDCouplingIMesh* mesh,
                               const MEDCouplingFieldDouble* velocity,
                               const MEDCouplingFieldDouble* YY_WITH_GHOST) const
{
    //velocity without ghost
    //YY with ghost
    int nbCells = mesh->getNumberOfCells();
    DataArrayDouble *array=DataArrayDouble::New();
    array->alloc(nbCells,2);
    for(int i=0;i<nbCells;i++)
    {
        array->setIJ(i,0,0.);
        array->setIJ(i,1,0.);
    }
    MEDCouplingIMesh* m1=mesh->buildWithGhost(numberOfCellsGhost);

    vector<int> nxyz=m1->getCellGridStructure();
    int nx=nxyz[0];
    int ny=nxyz[1];
    int nz;
    if (m1->getSpaceDimension()==2)
        nz=2*numberOfCellsGhost+1;
    else
        nz=nxyz[2];
    m1->decrRef();
    const double *yy=YY_WITH_GHOST->getArray()->getConstPointer();

    int saut=0;
    if (direction==0)
        saut=1;
    else if (direction==1)
        saut=nx;
    else if (direction==2)
        saut=nx*ny;

    int ijkCWithGhost;
    int dim=mesh->getSpaceDimension();
    for (int k=numberOfCellsGhost;k<nz-numberOfCellsGhost;k++)
    {
        for (int j=numberOfCellsGhost;j<ny-numberOfCellsGhost;j++)
        {
            for (int i=numberOfCellsGhost;i<nx-numberOfCellsGhost;i++)
            {
                if (dim==2)
                    ijkCWithGhost=i+j*nx;
                else
                    ijkCWithGhost=i+j*nx+k*nx*ny;
                int ijkCWithoutGhost=(i-numberOfCellsGhost)+(j-numberOfCellsGhost)*(nx-2*numberOfCellsGhost)+(k-numberOfCellsGhost)*(nx-2*numberOfCellsGhost)*(ny-2*numberOfCellsGhost);
                double u_moyen=velocity->getIJ(ijkCWithoutGhost,direction);
                if (u_moyen>0.)
                {
                    double min_Y_1=min(yy[ijkCWithGhost-saut],yy[ijkCWithGhost-2*saut]);
                    double max_Y_1=max(yy[ijkCWithGhost-saut],yy[ijkCWithGhost-2*saut]);
                    double petit_b=1./(u_moyen*dt/dspace)*(yy[ijkCWithGhost-saut]-max_Y_1)+max_Y_1;
                    double grand_b=1./(u_moyen*dt/dspace)*(yy[ijkCWithGhost-saut]-min_Y_1)+min_Y_1;
                    array->setIJ(ijkCWithoutGhost,0,u_moyen*yy[ijkCWithGhost]);
                    if (yy[ijkCWithGhost]<=petit_b)
                        array->setIJ(ijkCWithoutGhost,0,u_moyen*petit_b);
                    if (yy[ijkCWithGhost]>=grand_b)
                        array->setIJ(ijkCWithoutGhost,0,u_moyen*grand_b);

                    min_Y_1=min(yy[ijkCWithGhost],yy[ijkCWithGhost-saut]);
                    max_Y_1=max(yy[ijkCWithGhost],yy[ijkCWithGhost-saut]);
                    petit_b=1./(u_moyen*dt/dspace)*(yy[ijkCWithGhost]-max_Y_1)+max_Y_1;
                    grand_b=1./(u_moyen*dt/dspace)*(yy[ijkCWithGhost]-min_Y_1)+min_Y_1;

                    array->setIJ(ijkCWithoutGhost,1,u_moyen*yy[ijkCWithGhost+saut]);

                    if (yy[ijkCWithGhost+saut]<=petit_b)
                        array->setIJ(ijkCWithoutGhost,1,u_moyen*petit_b);

                    if (yy[ijkCWithGhost+saut]>=grand_b)
                        array->setIJ(ijkCWithoutGhost,1,u_moyen*grand_b);
                }else if (u_moyen<0.)
                {
                    double min_Y_1=min(yy[ijkCWithGhost],yy[ijkCWithGhost+saut]);
                    double max_Y_1=max(yy[ijkCWithGhost],yy[ijkCWithGhost+saut]);
                    double petit_b=1./(-u_moyen*dt/dspace)*(yy[ijkCWithGhost]-max_Y_1)+max_Y_1;
                    double grand_b=1./(-u_moyen*dt/dspace)*(yy[ijkCWithGhost]-min_Y_1)+min_Y_1;

                    array->setIJ(ijkCWithoutGhost,0,u_moyen*yy[ijkCWithGhost-saut]);

                    if (yy[ijkCWithGhost-saut]<=petit_b)
                        array->setIJ(ijkCWithoutGhost,0,u_moyen*petit_b);
                    if (yy[ijkCWithGhost-saut]>=grand_b)
                        array->setIJ(ijkCWithoutGhost,0,u_moyen*grand_b);

                    min_Y_1=min(yy[ijkCWithGhost+saut],yy[ijkCWithGhost+2*saut]);
                    max_Y_1=max(yy[ijkCWithGhost+saut],yy[ijkCWithGhost+2*saut]);
                    petit_b=1./(-u_moyen*dt/dspace)*(yy[ijkCWithGhost+saut]-max_Y_1)+max_Y_1;
                    grand_b=1./(-u_moyen*dt/dspace)*(yy[ijkCWithGhost+saut]-min_Y_1)+min_Y_1;

                    array->setIJ(ijkCWithoutGhost,1,u_moyen*yy[ijkCWithGhost]);
                    if (yy[ijkCWithGhost]<=petit_b)
                        array->setIJ(ijkCWithoutGhost,1,u_moyen*petit_b);
                    if (yy[ijkCWithGhost]>=grand_b)
                        array->setIJ(ijkCWithoutGhost,1,u_moyen*grand_b);
                }
            }
        }
    }

    MEDCouplingFieldDouble* FLUXES=MEDCouplingFieldDouble::New(ON_CELLS);
    FLUXES->setName("FLUXES") ;
    FLUXES->setMesh(const_cast<MEDCouplingIMesh *>(mesh));
    FLUXES->setArray(array);
    FLUXES->setTime(0.,0,0);
    array->decrRef();
    return FLUXES;
}

void
AdvectionSolver::advanceTimeDL(int numberOfCellsGhost,
                               double dspace,
                               double dt,
                               const MEDCouplingIMesh* imesh,
                               const MEDCouplingFieldDouble* fluxes,
                               MEDCouplingFieldDouble* yyWithGhost) const
{
    MEDCouplingFieldDouble* YY2_F=AMR::buildFieldWithoutGhostFromFieldWithGhost(numberOfCellsGhost,imesh,yyWithGhost);
    DataArrayDouble* YY2=YY2_F->getArray();
    double* YY=YY2->getPointer();
    int nbCells = YY2->getNumberOfTuples();
    for(int i=0;i<nbCells;i++)
        YY[i]=YY[i] - dt/dspace*(fluxes->getIJ(i,1)-fluxes->getIJ(i,0));

    MEDCouplingFieldDouble* YY3=AMR::buildFieldWithGhostFromFieldWithoutGhost(numberOfCellsGhost,imesh,YY2_F);
    yyWithGhost->setArray(YY3->getArray());
    YY3->decrRef();
    YY2_F->decrRef();
}

void
AdvectionSolver::SolverDL(int idir,int numberOfCellsGhost,double dt,const MEDCouplingIMesh *imesh,const MEDCouplingFieldDouble* velocity,MEDCouplingFieldDouble* yyWithGhost) const
{
    vector<double> dxyz;
    dxyz=imesh->getDXYZ();

    velocity->writeVTK("velocityDL.vti");
    MEDCouplingFieldDouble* fluxes=computeFluxDL(idir,dxyz[idir],dt,numberOfCellsGhost,imesh,velocity,yyWithGhost);
    advanceTimeDL(numberOfCellsGhost,dxyz[idir],dt,imesh,fluxes,yyWithGhost);
    fluxes->decrRef();
}

double
AdvectionSolver::advancingTimeStep(
        int idir,
        double currentTime,
        ParaMEDMEM::MEDCouplingAMRAttribute* fields,
        const ParaMEDMEM::MEDCouplingCartesianAMRPatchGen* grid) const
{
    int numberOfCellsGhost=getNumberOfGhostCells();
    MEDCouplingCartesianAMRMeshGen* mesh=const_cast<MEDCouplingCartesianAMRMeshGen *>(grid->getMesh());
    double finalTime=getFinalTime();
    MEDCouplingFieldDouble* velocity=computeVelocity(grid->getMesh()->getImageMesh(),currentTime,finalTime);
    vector<double> dxyz=grid->getMesh()->getImageMesh()->getDXYZ();
    double cfl=getCfl() ;
    double dt=computeDt(cfl,dxyz,velocity);
    MEDCouplingIMesh* m1=grid->getMesh()->getImageMesh()->buildWithGhost(numberOfCellsGhost);
    MEDCouplingFieldDouble* yyWithGhost_F=MEDCouplingFieldDouble::New(ON_CELLS);
    yyWithGhost_F->setName("YY") ;
    yyWithGhost_F->setMesh(m1);
    DataArrayDouble* yyWithGhost=const_cast<DataArrayDouble *>(fields->getFieldOn(mesh,"YY"));
    yyWithGhost_F->setArray(yyWithGhost);
    yyWithGhost_F->setTime(0.,0,0);
    yyWithGhost_F->checkCoherency();

    SolverDL(idir,numberOfCellsGhost,dt,grid->getMesh()->getImageMesh(),velocity,yyWithGhost_F);

    std::copy(yyWithGhost_F->getArray()->getPointer(),yyWithGhost_F->getArray()->getPointer()+yyWithGhost_F->getArray()->getNumberOfTuples(),yyWithGhost->getPointer());
    yyWithGhost_F->decrRef();
    velocity->decrRef();
    m1->decrRef();
    return dt;
}

double
AdvectionSolver::computeDt(double cfl, vector<double> dxyz, const MEDCouplingFieldDouble* velocity) const
{
    double dim=velocity->getMesh()->getSpaceDimension();
    double minLength=1.E30;
    minLength=dxyz[0];
    if (dim==1)
        minLength=dxyz[0];
    else if (dim==2)
        minLength = min(dxyz[0],dxyz[1]);
    else if (dim==3)
        minLength = min(min(dxyz[0],dxyz[1]),dxyz[2]);

    DataArrayDouble* magn=velocity->getArray()->magnitude();
    double maxNormU = magn->getMaxValueInArray();
    magn->decrRef();
    return cfl * minLength / maxNormU;
}
