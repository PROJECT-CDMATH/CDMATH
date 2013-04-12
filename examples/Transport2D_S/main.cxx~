//============================================================================
// Author      : Anouar MEKKAS
// Version     :
// Description : Equation de transport lineaire 2D cartesien
//============================================================================


#include "Mesh.hxx"
#include "Cell.hxx"
#include "Face.hxx"
#include "Field.hxx"

#include <iostream>
#include <string>
#include <cmath>

using namespace std;

void conditions_initiales(Field& YY)
{
	double rayon=0.15;
	double xcentre=0.25;
	double ycentre=0.25;
	Mesh M=YY.getMesh();
	int nbCells=M.getNumberOfCells();
	for (int j=0 ; j<nbCells ; j++)
	{
		double x = M.getCell(j).x() ;
		double y = M.getCell(j).y() ;
		double valX=(x-xcentre)*(x-xcentre);
		double valY=(y-ycentre)*(y-ycentre);
		double val=sqrt(valX+valY);
		if (val<rayon)
			YY(j) = 1.0;
		else
			YY(j) = 0.0;
	}
}

void sigma_flux(double VitesseX, double VitesseY, double cfl, const Field& YY, const IntTab indexFacesPerio, double& dt, Field& SumFlux)
{
	/* Calcul des flux */
	Mesh M=YY.getMesh();
	int nbCells=M.getNumberOfCells();
	double normU=sqrt(VitesseX*VitesseX+VitesseY*VitesseY);
	for (int j=0 ; j<nbCells ; j++)
	{
		Cell Cj=M.getCell(j);
		int nbFace=Cj.getNumberOfFaces();
		double SumF=0.0;
		double minlengthFk=1.E30;

		int cellCourante,cellAutre;
		for (int k=0 ; k<nbFace ; k++)
		{
			int indexFace=Cj.getFacesId()[k];
			Face Fk=M.getFace(indexFace);
			double NormalX=Cj.getNormalVector(k,0);
			double NormalY=Cj.getNormalVector(k,1);
			double LengthFk = Fk.getMeasure();
			double UN=VitesseX*NormalX+VitesseY*NormalY;

			minlengthFk=min(minlengthFk,LengthFk/fabs(UN));
			minlengthFk=min(minlengthFk,LengthFk/fabs(VitesseX));
			minlengthFk=min(minlengthFk,LengthFk/fabs(VitesseY));

			double conc=0.0;
			cellCourante=j;
			cellAutre=-1;

			if (!Fk.isBorder())
			{
				int indexC1=Fk.getCellsId()[0];
				int indexC2=Fk.getCellsId()[1];
				/* hypothese: La cellule d'index indexC1 est la cellule courante index j */
				if ( indexC1 == j )
				{
					/* hypothese verifie */
					cellCourante=indexC1;
					cellAutre=indexC2;
				} else if ( indexC2 == j )
				{
					/* hypothese non verifie */
					cellCourante=indexC2;
					cellAutre=indexC1;
				}
				// definir la cellule gauche et droite par le prduit vitesse * normale sortante
				// si u*n>0 : rien a faire sinon inverser la gauche et la droite
				if (UN>1.E-15)
					conc=YY(cellCourante);
				else
					conc=YY(cellAutre);
			}else
			{
				/* conditions aux limites neumann homogene */
				if (Fk.getGroupName().compare("LeftEdge")==0 || Fk.getGroupName().compare("RightEdge")==0)
				{
					if (UN>1.E-15)
						conc=YY(cellCourante);
					else
						conc=0.0;
				}
				/* conditions aux limites periodiques */
				if (Fk.getGroupName().compare("BottomEdge")==0 || Fk.getGroupName().compare("TopEdge")==0)
				{
						int indexFP=indexFacesPerio[indexFace];
						/* une autre manière de recuperer l'index de la face periodique */
						//int indexFP=M.getIndexFacePeriodic(indexFace);
						Face Fp=M.getFace(indexFP);
						int indexCp=Fp.getCellsId()[0];
						if (UN>1.E-15)
							conc=YY(cellCourante);
						else
							conc=YY(indexCp);
				}
			}
			SumF=SumF+UN*LengthFk*conc;
		  }
		dt=cfl*minlengthFk/normU;
		SumFlux(j)=dt*SumF/Cj.getMeasure();
	}
}

void EquationTransport2D(double tmax, double VitesseX, double VitesseY, double cfl, int freqSortie, const Mesh& M, const string file)
{
	/* Condition initiale */
	cout << "Construction de la condition initiale ... " << endl;
	Field YY("YY",CELLS,M,1) ;
	conditions_initiales(YY);

	/*
	 * Sortie MED de la condition initiale à t=0 et iter = 0
	 */
	int iter=0;
	double time=0.;
	cout << "Post-traitement MED de la solution à T=" << time << " ..." << endl;
	YY.setTime(time,iter);
	YY.writeMED(file);
	YY.writeVTK(file);
	YY.writeASCII(file);
	/* --------------------------------------------- */

	/* boucle de temps */
	cout << " Resolution de l'equation de transport par un schema UPWIND ..." << endl;
	int ntmax=10000;
	double dt;
	IntTab indexFacesPerio=M.getIndexFacePeriodic();
	while (iter<ntmax && time <= tmax )
	{
		Field SumFlux("SUM FLUX",CELLS,M,1) ;
		sigma_flux(VitesseX,VitesseY,cfl,YY,indexFacesPerio,dt,SumFlux);
		cout << "-- Iter : " << iter << " Time : " << time << " dt : " << dt << endl;

		/* Avancement en temps */
		YY-=SumFlux;

		time+=dt;
		iter+=1;
		// sortie visu tous les freq iterations
		if (iter%freqSortie==0)
		{
			YY.setTime(time,iter);
			YY.writeMED(file,false);
			YY.writeVTK(file,false);
			YY.writeASCII(file);
		}
	}
}

int main()
{
	cout << "RESOLUTION EQUATION DE TRANSPORT 2D : " << endl;
	cout << "- DOMAINE : CARREE " << endl;
	cout << "- MAILLAGE CARTESIEN : GENERATION INTERNE CDMATH " << endl;
	cout << "- CL PERIODIQUE GAUCHE ET DROITE " << endl;
	cout << "- CL NEUMANN HOMOGENE GAUCHE ET DROITE " << endl;

	// donnees du probleme
	double cfl=0.4;
	double VitesseX=1.0;
	double VitesseY=1.0;
	double tmax=1.;
	int freqSortie=10;

	cout << "Construction du maillage Cartesien ... " << endl;
	double xinf=0.0;
	double xsup=1.0;
	double yinf=0.0;
	double ysup=1.0;
	int nx=100;
	int ny=100;
	Mesh M(xinf,xsup,nx,yinf,ysup,ny);
    double eps=1.E-10;
    M.setGroupAtPlan(xsup,0,eps,"RightEdge");
    M.setGroupAtPlan(xinf,0,eps,"LeftEdge");
    M.setGroupAtPlan(yinf,1,eps,"BottomEdge");
    M.setGroupAtPlan(ysup,1,eps,"TopEdge");
	string fileOutPutCart="Exercie1";
	EquationTransport2D(tmax,VitesseX,VitesseY,cfl,freqSortie,M,fileOutPutCart);
	return 0;
}
