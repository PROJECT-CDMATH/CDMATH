//============================================================================
// Author      : Anouar MEKKAS
// Version     :
// Description : Equation de transport lineaire 1D
//============================================================================

#include "Cell.hxx"
#include "Mesh.hxx"
#include "Field.hxx"

#include <iostream>
#include <cmath>

using namespace std;

int main( void )
{
  double a=-5.0;
  double b=5.0;
  int nx=1000;
  int ntmax=1000;
  double dx = (b-a)/nx;
  double pi=3.1415927;
  // vitesse de transport
  double cfl=0.5;
  double u=3.;
  double dt=cfl*dx/u;

  Mesh M(a,b,nx);

  Field conc("CONCENTRATION",CELLS,M,1);

  //conditions iniiales
  double sigma=sqrt(0.2);
  for (int i=0 ; i<M.getNumberOfCells() ; i++)
  {
   double x=M.getCell(i).x();
   conc(i) = 0.5/(sigma*sqrt(2*pi))*exp(-0.5*pow((x/sigma),2));
  }

  double time=0.;
  double tmax=3.0;
  int iter=0;

  cout << "Post-traitement MED de la solution à T=" << time << " ..." << endl;
  string fileOutPut="EqTr1D";
  conc.setTime(time,iter);
  conc.writeMED(fileOutPut);
  conc.writeVTK(fileOutPut);
  conc.writeCSV(fileOutPut);
  int freqSortie=10;
  // boucle en temps
  while (iter<ntmax && time <= tmax )
  {
   cout << "-- Iter : " << iter << " Time : " << time << " dt : " << dt << endl;
   conc(0) = conc(0) -u*dt/dx*(conc(0)-conc(M.getNumberOfCells()-1));
   for (int j=1 ; j<M.getNumberOfCells() ; j++)
   {
    conc(j) = conc(j) -u*dt/dx*(conc(j)-conc(j-1));
   }
   time+=dt;
   iter+=1;
   if (iter%freqSortie==0)
   {
	   conc.setTime(time,iter);
	   conc.writeMED(fileOutPut,false);
	   conc.writeVTK(fileOutPut,false);
	   conc.writeCSV(fileOutPut);
   }
  }
  return 0;
}


