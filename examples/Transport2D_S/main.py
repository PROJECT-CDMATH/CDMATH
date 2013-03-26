# coding: latin-1 

from cdmath import *
from math import sqrt

def conditions_initiales(M):
    rayon=0.15
    xcentre=0.25
    ycentre=0.25
    YY=Field("YY",CELLS,M,1)
    nbCells=M.getNumberOfCells()
    for j in range(nbCells):
        x = M.getCell(j).x() 
        y = M.getCell(j).y() 
        valX=(x-xcentre)*(x-xcentre)
        valY=(y-ycentre)*(y-ycentre)
        val=sqrt(valX+valY)
        if val<rayon:
            YY[j] = 1.0
            pass
        else:
            YY[j] = 0.0
            pass
        pass
    return YY

def sigma_flux(VitesseX,VitesseY,cfl,YY,indexFacesPerio):
    # Calcul des flux #
    SumFlux=Field("Fluxes",CELLS,YY.getMesh(),1)
    M=YY.getMesh();
    nbCells=M.getNumberOfCells();
    normU=sqrt(VitesseX*VitesseX+VitesseY*VitesseY);
    for j in range(nbCells):
        Cj=M.getCell(j);
        nbFace=Cj.getNumberOfFaces();
        SumF=0.0;
        minlengthFk=1.E30;
        for k in range(nbFace):
            indexFace=Cj.getFacesId()[k];
            Fk=M.getFace(indexFace);
            NormalX=Cj.getNormalVector(k,0);
            NormalY=Cj.getNormalVector(k,1);
            LengthFk = Fk.getMeasure();
            UN=VitesseX*NormalX+VitesseY*NormalY;
            minlengthFk=min(minlengthFk,LengthFk/abs(UN));
            minlengthFk=min(minlengthFk,LengthFk/abs(VitesseX));
            minlengthFk=min(minlengthFk,LengthFk/abs(VitesseY));
            conc=0.0;
            cellCourante=j;
            cellAutre=-1;
            if (not Fk.isBorder()):
                indexC1=Fk.getCellsId()[0];
                indexC2=Fk.getCellsId()[1];
                # hypothese: La cellule d'index indexC1 est la cellule courante index j #
                if (indexC1 == j ):
                    # hypothese verifie #
                    cellCourante=indexC1;
                    cellAutre=indexC2;
                    pass
                elif ( indexC2 == j ):
                    # hypothese non verifie #
                    cellCourante=indexC2;
                    cellAutre=indexC1;
                    pass
                # definir la cellule gauche et droite par le prduit vitesse * normale sortante
                # si u*n>0 : rien a faire sinon inverser la gauche et la droite
                if (UN>1.E-15):
                    conc=YY[cellCourante];
                    pass
                else:
                    conc=YY[cellAutre];
                    pass
                pass
            else:
                # conditions aux limites neumann homogene #
                if (Fk.getGroupName()=="LeftEdge" or Fk.getGroupName()=="RightEdge"):
                    if (UN>1.E-15):
                        conc=YY[cellCourante];
                        pass
                    else:
                        conc=0.0;
                        pass
                    pass
                # conditions aux limites periodiques #
                if (Fk.getGroupName()=="BottomEdge" or Fk.getGroupName()=="TopEdge"):
                    indexFP=indexFacesPerio[indexFace];
                    # une autre maniÃ¨re de recuperer l'index de la face periodique #
                    #int indexFP=M.getIndexFacePeriodic(indexFace);
                    Fp=M.getFace(indexFP);
                    indexCp=Fp.getCellsId()[0];
                    if (UN>1.E-15):
                        conc=YY[cellCourante];
                        pass
                    else:
                        conc=YY[indexCp];
                        pass
                    pass
                pass
            SumF=SumF+UN*LengthFk*conc;
            pass        
        dt=cfl*minlengthFk/normU;
        SumFlux[j]=dt*SumF/Cj.getMeasure();
        pass
    return dt,SumFlux

def EquationTransport2D(tmax,VitesseX,VitesseY,cfl,freqSortie,M,fileMED):

    # Condition initiale #
    print "Construction de la condition initiale ... "
    YY=conditions_initiales(M)
    #
    #   Sortie MED de la condition initiale ÃÂÃÂ  t=0 et iter = 0
    #

    iter=0;
    time=0.;
    print "Post-traitement MED de la solution aÂ  T=%s ..."%time
    YY.setTime(time,iter);
    YY.writeMED(fileMED);

    # boucle de temps #
    print " Resolution de l'equation de transport par un schema UPWIND ..."
    ntmax=100000;
    indexFacesPerio=M.getIndexFacePeriodic();
    dt=0.
    while (iter<ntmax and time <= tmax ):
        dt,SumFlux=sigma_flux(VitesseX,VitesseY,cfl,YY,indexFacesPerio);
        print "-- Iter : ",iter," Time : ",time," dt : ",dt 

        #Avancement en temps#
        YY-=SumFlux;
        time+=dt;
        iter+=1;
        #sortie visu tous les freq iterations
        if (iter%freqSortie==0):
            YY.setTime(time,iter);
            YY.writeMED(fileMED,False);
            pass
        pass
    return

def main():
    print "RESOLUTION EQUATION DE TRANSPORT 2D : "
    print "- DOMAINE : NON CARREE "
    print "- MAILLAGE CARTESIEN : GENERATION INTERNE CDMATH "
    print "- CL PERIODIQUE GAUCHE ET DROITE "
    print "- CL NEUMANN HOMOGENE GAUCHE ET DROITE "

    # donnees du probleme
    cfl=0.4
    VitesseX=1.0
    VitesseY=1.0
    tmax=1.
    freqSortie=10

    print "Construction du maillage Cartesien ... "
    xinf=0.0
    xsup=1.0
    yinf=0.0
    ysup=1.0
    nx=100
    ny=100
    M=Mesh(xinf,xsup,nx,yinf,ysup,ny)
    eps=1.E-10
    M.setGroupAtPlan(xsup,0,eps,"RightEdge")
    M.setGroupAtPlan(xinf,0,eps,"LeftEdge")
    M.setGroupAtPlan(yinf,1,eps,"BottomEdge")
    M.setGroupAtPlan(ysup,1,eps,"TopEdge")
    fileOutPutMEDCart="Exercie1PyTest.med"
    EquationTransport2D(tmax,VitesseX,VitesseY,cfl,freqSortie,M,fileOutPutMEDCart)
    return

if __name__ == '__main__':
    main()