//============================================================================
// Description : Equation de transport 2D avec Maillage non structure avec schéma Upwind
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
  double dt;
  int nbFace;
  double xn;
  double yn;
  double fluxX;
  double fluxY;
  double LengthFk;
  int idxFace;
  int nbCells;
  double un;
  double minlengthFk;
  int index;
  double Sum;
  
  int i=0;
  double time=0.;
  int ntmax=100000;
  double tmax=0.5;
  // vitesse de transport
  double cfl=0.4;
  double ux=1.5;
  double uy=1.5; //-0.5;
  double normU=sqrt(ux*ux+uy*uy);
  double rayon=0.15;
  double xcentre=0.25;
  double ycentre=0.25;  
  int comp=0;
  int freq=10;
  int indexC1;
  int indexC2;
  double maxnormU;

  char outPutNameFile[100]="ConcentrationTestMeshNonStructure_NACA.vtu";
  char outPutAnimation[100]="Animation_NACA";
  char outPutFormat[10]="VTK";
  
  Mesh M("../Mesh/MeshTestTriangleNACA.med","MED"); 

  OutPut sortieconc1(M,outPutNameFile,outPutFormat) ;  
  FieldOnCell conc1("CONCENTRATION",M,1) ;
  FieldOnCell SumFlux("SUM FLUX",M,1) ;

  //conditions iniial
  for (int j=0 ; j<M.getNumberOfCells() ; j++)
  {
   double x = M.getCell(j).x() ;
   double y = M.getCell(j).y() ;
   double valx=pow(x-xcentre,2);
   double valy=pow(y-ycentre,2);
   double val=sqrt(valx+valy);
   if (val<rayon)
     conc1(j) = 1.0;
   else
     conc1(j) = 0.0;
  }

  cout << "DEBUT CALCUL " << endl;
  while (i<ntmax && time < tmax )
  {
   // sortie visu tous les freq iterations
   if (i%freq==0)
   {
    char* fileName= new char[50];
    char* numfile=new char[10];
    strcpy(fileName,outPutAnimation);
    fileName=strcat(fileName,"_");
    sprintf(numfile,"%i",comp);
    fileName=strcat(fileName,numfile);
    fileName=strcat(fileName,".vtu");
    OutPut sortieconcTime(M,fileName,outPutFormat) ;
    cout << "-- OutPut Format : " << outPutFormat << " File name : " << fileName << " Field name : " << conc1.getFieldName() << endl;
    sortieconcTime.addOutPut(conc1) ;
    delete [] fileName ;
    delete [] numfile ;
    comp++;
   }

   Cell Cj;
   for (int j=0 ; j<M.getNumberOfCells() ; j++)
   {
    Cj=M.getCell(j);
    nbFace=Cj.getNumberOfFaces();
    Sum=0.0;
    minlengthFk=1.E30;
    maxnormU=0.0;
    // calcul des flux 
    for (int k=0 ; k<nbFace ; k++)
    {
     idxFace=Cj.getFacesId()[k];
     Face Fk=M.getFace(idxFace);
     nbCells=Fk.getNumberOfCells();
     xn=Cj.getNormalVector(k,0);
     yn=Cj.getNormalVector(k,1);
     LengthFk = Fk.getLength();
     un=ux*xn+uy*yn;
     minlengthFk=min(minlengthFk,LengthFk/fabs(un));
     minlengthFk=min(minlengthFk,LengthFk/fabs(ux));
     minlengthFk=min(minlengthFk,LengthFk/fabs(uy));

     double conc=0.0;
     int cellcourante=j;
     int cellautre=-1;
     if (nbCells==2)
     {
       indexC1=Fk.getCellsId()[0];
       indexC2=Fk.getCellsId()[1];
       // hypothese 1 : cellule gauche: cellule courante index j 
       if (indexC1==j)
       {
        cellcourante=indexC1;
        cellautre=indexC2;
       } else if (indexC2==j)
       {
        cellcourante=indexC2;
        cellautre=indexC1;
       }
       // definir la cellule gauche et droite par le prduit vitesse * normale sortante 
       // si u*n>0 : rien a faire sinon inverser la gauche et la droite
       if (un>1.E-15)
        conc=conc1(cellcourante);
       else
        conc=conc1(cellautre);
     }else
     {
       if (un>1.E-15)
        conc=conc1(cellcourante);
       else
        conc=0.0;
     }

     double fluxk=un*LengthFk*conc;
     Sum=Sum+fluxk;
    }
    dt=cfl*minlengthFk/normU;
    SumFlux(j)=dt*Sum/Cj.getSurface();
   }

   // mise a jour de la concentration 
   for (int j=0 ; j<M.getNumberOfCells() ; j++)
   {
    conc1(j)=conc1(j)-SumFlux(j);
    if (fabs(conc1(j))<1.E-14)
      conc1(j)=0.0;
   }
   cout << "-- Iter : " << i << " Time : " << time << " dt : " << dt << endl;
   time+=dt;
   // sortie visu de la derniere iteration
   if (time>=tmax)
   {
    char* fileName= new char[50];
    char* numfile=new char[10];
    strcpy(fileName,outPutAnimation);
    fileName=strcat(fileName,"_");
    sprintf(numfile,"%i",comp);
    fileName=strcat(fileName,numfile);
    fileName=strcat(fileName,".vtu");
    OutPut sortieconcTime(M,fileName,outPutFormat) ;
    cout << "-- OutPut Format : " << outPutFormat << " File name : " << fileName << " Field name : " << conc1.getFieldName() << endl;
    sortieconcTime.addOutPut(conc1) ;
    delete [] fileName ;
    delete [] numfile ;
    comp++;
   }
   i+=1;
  }
  sortieconc1.addOutPut(conc1) ;

  return 0;

}
