//============================================================================
// Description : Equation de transport 1D avec schéma Upwind
// Author      : Mekkas Anouar
// Version     :
//============================================================================

#include <iostream>
using namespace std;

#include "mesh.hxx"
#include "fieldonnode.hxx"
#include "fieldoncell.hxx"

#include "output.hxx"

int main( void )
{

  /* ----------------------------------------------------
     - Exemple 1: 
     - Equation de transport lineaire 1D
     - Maillage 1D cartésien
     ----------------------------------------------------
  */


  double a=-5.0;
  double b=5.0;
  int nx=1000;
  int ntmax=500;
  double dx = (b-a)/nx;
  double pi=3.1415927;
  // vitesse de transport
  double cfl=0.5;
  double u=3.;
  double dt=cfl*dx/u;

  char outPutNameFile[100]="ConcentrationTestMeshCartesian_Test0.DAT";
  char outPutFormat[10]="ASCII";
  Mesh M(a,b,nx);

  OutPut sortieconc(M,outPutNameFile,outPutFormat) ;

  FieldOnCell conc("CONCENTRATION",M,1) ;

  //conditions iniial
  double sigma=sqrt(0.2);
  for (int i=0 ; i<M.getNumberOfCells() ; i++)
  {
   Cell ci=M.getCell(i);
   double x = ci.x() ;
   conc(i) = 0.5/(sigma*sqrt(2*pi))*exp(-0.5*pow((x/sigma),2));
  }

  // boucle en temps
  for (int i=0 ; i<ntmax ; i++)
  {
   conc(0) = conc(0) -u*dt/dx*(conc(0)-conc(M.getNumberOfCells()-1));
   for (int j=1 ; j<M.getNumberOfCells() ; j++)
   {
    conc(j) = conc(j) -u*dt/dx*(conc(j)-conc(j-1));
   }
  }
  // sortie paraview
  sortieconc.addOutPut(conc) ;
  return 0;
}

