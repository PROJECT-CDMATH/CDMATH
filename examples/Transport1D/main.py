# coding: latin-1 

from cdmath import *
from math import sqrt, exp, pow

def main():
    a=-5.0;
    b=5.0;
    nx=1000;
    ntmax=1000;
    dx = (b-a)/nx;
    pi=3.1415927;
    # vitesse de transport
    cfl=0.5;
    u=3.;
    dt=cfl*dx/u;

    M=Mesh(a,b,nx);
    conc=Field("CONCENTRATION",CELLS,M,1);


    #conditions iniiales
    sigma=sqrt(0.2);
    for i in xrange(M.getNumberOfCells()):
        x=M.getCell(i).x();
        conc[i] = 0.5/(sigma*sqrt(2*pi))*exp(-0.5*pow((x/sigma),2));
        pass

    time=0.;
    tmax=3.0;
    it=0;

    print "Post-traitement MED de la solution à T=",time," ..."
    file="EqTr1D";
    conc.setTime(time,it);
    conc.writeMED(file);
    conc.writeVTK(file);
    conc.writeASCII(file);
    freqSortie=10;
    #boucle en temps
    while (it<ntmax and time <= tmax ):
        print "-- Iter : ", it," Time : ",time," dt : ",dt
        conc[0] = conc[0]- u*dt/dx*(conc[0]-conc[M.getNumberOfCells()-1]);
        for j in xrange(1,M.getNumberOfCells()):
            conc[j] = conc[j] -u*dt/dx*(conc[j]-conc[j-1]);
            pass
        time+=dt;
        it+=1;
        if (it%freqSortie==0):
            conc.setTime(time,it);
            conc.writeMED(file,False);
            conc.writeVTK(file,False);
            conc.writeASCII(file);
            pass
        pass
    return

if __name__ == '__main__':
    main()