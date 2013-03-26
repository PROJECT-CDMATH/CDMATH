# coding: latin-1 

from cdmath import *
import unittest
from math import sqrt

class MeshTest(unittest.TestCase):
    def testClassPoint(self):
        P1=Point(1.,2.,3.)
        self.assertTrue(P1.x()==1.)
        self.assertTrue(P1.y()==2.)
        self.assertTrue(P1.z()==3.)
        self.assertTrue(P1[0]==1.)
        self.assertTrue(P1[1]==2.)
        self.assertTrue(P1[2]==3.)
        P2=Point(1., 2., 3.);
        self.assertTrue( 14., P1.dot(P2) );

        P3=P1+P2;
        self.assertTrue( 2., P3.x());
        self.assertTrue( 4., P3.y());
        self.assertTrue( 6., P3.z());

        P5=Point(1., 2., 3.);
        P6=Point(3., 5., 0.);
        P4=P5-P6;
        self.assertTrue( -2., P4.x());
        self.assertTrue( -3., P4.y());
        self.assertTrue( 3., P4.z());

        P5+=P6;
        self.assertTrue( 4., P5.x());
        self.assertTrue( 7., P5.y());
        self.assertTrue( 3., P5.z());

        P7=Point();
        P7[0]=1.0;
        P7[1]=2.0;
        P7[2]=3.0;
        P8=Point(3., 5., 0.);
        P7-=P8;
        self.assertTrue( -2., P7.x());
        self.assertTrue( -3., P7.y());
        self.assertTrue( 3., P7.z());

        P9=Point();
        P9=P1*3.0;
        self.assertTrue( 3., P9.x());
        self.assertTrue( 6., P9.y());
        self.assertTrue( 9., P9.z());

        P10=Point(1., 2., 3.);
        P10*=3.0;
        self.assertTrue( 3., P10.x());
        self.assertTrue( 6., P10.y());
        self.assertTrue( 9., P10.z());

        norm=P1.norm();
        self.assertTrue( sqrt(14.), norm);

        P11=Point(1., 2., 3.);
        P12=Point(4., 5., 6.);
        dx=P12.x()-P11.x();
        dy=P12.y()-P11.y();
        dz=P12.z()-P11.z();
        distance=sqrt(dx*dx+dy*dy+dz*dz);
        self.assertTrue( distance, P11.distance(P12));

        P13=Point(3., 6., 9.);
        P14=Point();
        P14=P13/3.0;
        self.assertTrue( 1., P14.x());
        self.assertTrue( 2., P14.y());
        self.assertTrue( 3., P14.z());

        P15=Point(3., 6., 9.);
        P15/=3.0;
        self.assertTrue( 1., P15.x());
        self.assertTrue( 2., P15.y());
        self.assertTrue( 3., P15.z());        
        return
    
    def testClassField(self):
        M=Mesh(0.0,1.0,10,0.,1.,5);

        conc1=Field("CONCENTRATION",CELLS,M,2,1.2) ;
        self.assertTrue( 1.2==conc1.getTime() );
        for i in range(conc1.getNumberOfElements()):
            conc1[0,i]=i*1.0;
            pass

        conc1n=Field("CONCENTRATION",NODES,M,2,1.2) ;
        self.assertTrue( 1.2==conc1n.getTime() );
        for i in range(conc1n.getNumberOfElements()):
            conc1n[0,i]=i*1.0;
            pass

        fileNameVTK="champc.vtu";
        conc1.writeVTK(fileNameVTK);

        fileNameMED="champc.med";
        conc1.writeMED(fileNameMED);
        conc1.setTime(2.3,1);
        conc1.writeMED(fileNameMED,False);
        for i in range(conc1.getNumberOfElements()):
            self.assertTrue( 1.0*i==conc1[i] );
            pass
            
        self.assertTrue( 2==conc1.getNumberOfComponents() );
        self.assertTrue( 50==conc1.getNumberOfElements() );
        self.assertTrue( 2.3==conc1.getTime() );

        fileNameVTK="champn.vtu";
        conc1n.writeVTK(fileNameVTK);

        fileNameMED="champn.med";
        conc1n.writeMED(fileNameMED);
        conc1n.setTime(2.3,1);
        conc1n.writeMED(fileNameMED,False);
        for i in range(conc1n.getNumberOfElements()):
            self.assertTrue( 1.0*i==conc1n[i] );
            pass
            
        self.assertTrue( 2==conc1n.getNumberOfComponents() );
        self.assertTrue( 66==conc1n.getNumberOfElements() );
        self.assertTrue( 2.3==conc1n.getTime() );

        conc6=Field("CONCENTRATION",NODES,M,2);
        for i in range(conc6.getNumberOfComponents()):
            for j in range(conc6.getNumberOfElements()):
                conc6[i,j]=i*1.0+2.*j;
                pass
            pass
        for i in range(conc6.getNumberOfComponents()):
            for j in range(conc6.getNumberOfElements()):
                self.assertTrue( 1.0*i+2.*j == conc6[i,j] );
                self.assertTrue( 1.0*i+2.*j == conc6.getValues()[i+j*conc6.getNumberOfComponents()] );
                pass
            pass

        conc6=Field("CONCENTRATION",CELLS,M,2);
        for i in range(conc6.getNumberOfComponents()):
            for j in range(conc6.getNumberOfElements()):
                conc6[i,j]=i*1.0+2.*j;
                pass
            pass
        for i in range(conc6.getNumberOfComponents()):
            for j in range(conc6.getNumberOfElements()):
                self.assertTrue( 1.0*i+2.*j == conc6[i,j] );
                self.assertTrue( 1.0*i+2.*j == conc6.getValues()[i+j*conc6.getNumberOfComponents()] );
                pass
            pass

        self.assertTrue( 2==conc1.getNumberOfComponents() );
        self.assertTrue( 50==conc1.getNumberOfElements() );

        conc3=conc1 ;
        for i in range(conc3.getNumberOfElements()):
            conc3[0,i]=i*1.0;
            pass

        x=conc3[2];
        self.assertTrue(x==2.0);

        for i in range(conc3.getNumberOfElements()):
            self.assertTrue( 1.0*i == conc3[i] );
            pass
        self.assertTrue( 2 == conc3.getNumberOfComponents() );
        self.assertTrue( 50 == conc3.getNumberOfElements() );

        conc6=conc3+conc1;
        for i in range(conc6.getNumberOfElements()):
            self.assertTrue( 2.0*i == conc6[i] );
            pass
        self.assertTrue( 2 == conc6.getNumberOfComponents() );
        self.assertTrue( 50 == conc6.getNumberOfElements() );

        conc6=conc3-conc1;
        for i in range(conc6.getNumberOfElements()):
            self.assertTrue( 0.0 == conc6[i] );
            pass
        self.assertTrue( 2 == conc6.getNumberOfComponents() );
        self.assertTrue( 50 == conc6.getNumberOfElements() );

        conc6=conc1;
        conc6+=conc1;
        for i in range(conc6.getNumberOfElements()):
            self.assertTrue( 2.0*i == conc6[0,i] );
            pass
        self.assertTrue( 2 == conc6.getNumberOfComponents() );
        self.assertTrue( 50 == conc6.getNumberOfElements() );

        for i in range(conc6.getNumberOfElements()):
            conc6[0,i]=i*1.0;
            pass
        conc6*=2.0;
        for i in range(conc6.getNumberOfElements()):
            self.assertTrue( 2.0*i == conc6[i] );
        self.assertTrue( 2 == conc6.getNumberOfComponents() );
        self.assertTrue( 50 == conc6.getNumberOfElements() );

        conc7=Field("CONCENTRATION",NODES,M,2) ;
        conc7.setField(conc1n.getField());
        conc7.setName("CONC")
        self.assertTrue( conc7.getName() == "CONC" );
        for i in range(conc7.getNumberOfElements()):
            self.assertTrue( conc1n[i] == conc7[i] );
            pass
        self.assertTrue( 2 == conc7.getNumberOfComponents() );
        self.assertTrue( 66 == conc7.getNumberOfElements() );

        conc7=Field("CONCENTRATION",CELLS,M,2) ;
        conc7.setField(conc1.getField());
        conc7.setName("CONC")
        self.assertTrue( conc7.getName() == "CONC" );
        for i in range(conc7.getNumberOfElements()):
            self.assertTrue( conc1[i] == conc7[i] );
            pass
        self.assertTrue( 2 == conc7.getNumberOfComponents() );
        self.assertTrue( 50 == conc7.getNumberOfElements() );

        conc8=Field("CONCENTRATION",CELLS,M) ;
        for i in range(conc8.getNumberOfElements()):
            conc8[i]=i*1.0;
            pass
        for i in range(conc8.getNumberOfElements()):
            self.assertTrue( 1.0*i == conc8[i] );
            pass
        self.assertTrue( 1 == conc8.getNumberOfComponents() );
        self.assertTrue( 50 == conc8.getNumberOfElements() );

        conc8=Field("CONCENTRATION",NODES,M) ;
        for i in range(conc8.getNumberOfElements()):
            conc8[i]=i*1.0;
            pass
        for i in range(conc8.getNumberOfElements()):
            self.assertTrue( 1.0*i == conc8[i] );
            pass
        self.assertTrue( 1 == conc8.getNumberOfComponents() );
        self.assertTrue( 66 == conc8.getNumberOfElements() );

        conc9=Field("CONCENTRATION",CELLS,M) ;
        for i in range(conc9.getNumberOfElements()):
            conc9[i]=i*1.0;
            pass
        conc9/=2.0;
        for i in range(conc9.getNumberOfElements()):
            self.assertTrue( 1.0*i/2. == conc9[i] );
            pass

        conc10=conc8 ;
        for i in range(conc10.getNumberOfElements()):
            conc10[i]=i*1.0;
            pass
        conc10-=2.0;
        for i in range(conc10.getNumberOfElements()):
            self.assertTrue( 1.0*i-2.0 == conc10[i] );

        conc11=conc8 ;
        for i in range(conc11.getNumberOfElements()):
            conc11[i]=i*1.0;
            pass
        conc11+=2.0;
        for i in range(conc11.getNumberOfElements()):
            self.assertTrue( 1.0*i+2. == conc11[i] );

        conc12=conc8 ;
        for i in range(conc12.getNumberOfElements()):
            conc12[i]=i*1.0;
            pass
        conc12+=conc8;
        for i in range(conc12.getNumberOfElements()):
            self.assertTrue( 2.0*i == conc12[i] );

        conc13=conc8 ;
        for i in range(conc13.getNumberOfElements()):
            conc13[i]=i*1.0;
            pass
        conc13-=conc8;
        for i in range(conc13.getNumberOfElements()):
            self.assertTrue( 0.0 == conc13[i] );
        return

    def testClassCell(self):
        P=Point(0.5,0.5,0.0);
        c1=Cell(4,4,1.0,P);
        c1.addNormalVector(0,0.2,0.3,0.0);
        self.assertTrue( 0.2==c1.getNormalVector(0,0) );
        self.assertTrue( 0.3==c1.getNormalVector(0,1) );
        self.assertTrue( 0.2==c1.getNormalVectors()[0] );
        self.assertTrue( 0.3==c1.getNormalVectors()[1] );
        c=Cell();
        c=c1
        self.assertTrue( 1.0==c.getMeasure() );
        self.assertTrue( 4==c.getNumberOfNodes() );
        self.assertTrue( 4==c.getNumberOfFaces() );
        self.assertTrue( 0.5==c.getBarryCenter().x() );
        self.assertTrue( 0.5==c.getBarryCenter().y() );
        self.assertTrue( 0.0==c.getBarryCenter().z() );
        self.assertTrue( 0.5==c.x() );
        self.assertTrue( 0.5==c.y() );
        self.assertTrue( 0.0==c.z() );
        c2=c1;
        c2.addNormalVector(1,0.4,0.6,0.0);
        self.assertTrue( 0.2==c2.getNormalVector(0,0) );
        self.assertTrue( 0.3==c2.getNormalVector(0,1) );
        self.assertTrue( 0.4==c2.getNormalVector(1,0) );
        self.assertTrue( 0.6==c2.getNormalVector(1,1) );
    
        c2=c1;
        c2.addFaceId(0,10);
        c2.addFaceId(1,11);
        c2.addFaceId(2,12);
        c2.addFaceId(3,13);
        c2.addNodeId(0,20);
        c2.addNodeId(1,21);
        c2.addNodeId(2,22);
        c2.addNodeId(3,23);
    
        self.assertTrue( 10==c2.getFacesId()[0] );
        self.assertTrue( 11==c2.getFacesId()[1] );
        self.assertTrue( 12==c2.getFacesId()[2] );
        self.assertTrue( 13==c2.getFacesId()[3] );
        self.assertTrue( 20==c2.getNodesId()[0] );
        self.assertTrue( 21==c2.getNodesId()[1] );
        self.assertTrue( 22==c2.getNodesId()[2] );
        self.assertTrue( 23==c2.getNodesId()[3] );
        return

    def testClassNode(self):
        P=Point(0.5,0.5,0.0);
        n1=Node(4,4,P);
        n=Node()
        n=n1;
        self.assertTrue( 4==n.getNumberOfCells() );
        self.assertTrue( 4, n.getNumberOfFaces() );
        self.assertTrue( 0.5==n.getPoint().x() );
        self.assertTrue( 0.5==n.getPoint().y() );
        self.assertTrue( 0.0==n.getPoint().z() );
        self.assertTrue( 0.5==n.x() );
        self.assertTrue( 0.5==n.y() );
        self.assertTrue( 0.0==n.z() );
        n2=n1;
        n2.addFaceId(0,10);
        n2.addFaceId(1,11);
        n2.addFaceId(2,12);
        n2.addFaceId(3,13);
        n2.addCellId(0,20);
        n2.addCellId(1,21);
        n2.addCellId(2,22);
        n2.addCellId(3,23);
        
        self.assertTrue( 10==n2.getFacesId()[0] );
        self.assertTrue( 11==n2.getFacesId()[1] );
        self.assertTrue( 12==n2.getFacesId()[2] );
        self.assertTrue( 13==n2.getFacesId()[3] );
        self.assertTrue( 20==n2.getCellsId()[0] );
        self.assertTrue( 21==n2.getCellsId()[1] );
        self.assertTrue( 22==n2.getCellsId()[2] );
        self.assertTrue( 23==n2.getCellsId()[3] );
        n2=n;
        self.assertTrue( 0.==n.distance(n2) );
        return

    def testClassFace(self):
        p=Point(0,1,2);
        f1=Face(2,2,1.0,p);
        f=Face();
        f=f1;
        self.assertTrue( 1.0==f.getMeasure() );
        self.assertTrue( 2==f.getNumberOfNodes() );
        self.assertTrue( 2==f.getNumberOfCells() );
        self.assertTrue( p.x()==f.getBarryCenter().x() );
        self.assertTrue( p.y()==f.getBarryCenter().y() );
        self.assertTrue( p.z()==f.getBarryCenter().z() );
        self.assertTrue( p.x()==f.x() );
        self.assertTrue( p.y()==f.y() );
        self.assertTrue( p.z()==f.z() );
        self.assertTrue( -1==f.getRegion() );
        self.assertTrue( False==f.isBorder() );
        f.setGroupName("Bord1")
        self.assertTrue( 0==f.getRegion() );
        self.assertTrue( True==f.isBorder() );
        self.assertTrue( "Bord1"==f.getGroupName() );
        
        f2=f1;
        f2.addCellId(0,10);
        f2.addCellId(1,11);
        f2.addNodeId(0,20);
        f2.addNodeId(1,21);
    
        self.assertTrue( 10==f2.getCellsId()[0] );
        self.assertTrue( 11==f2.getCellsId()[1] );
        self.assertTrue( 20==f2.getNodesId()[0] );
        self.assertTrue( 21==f2.getNodesId()[1] );
    
        f2=f;
        self.assertTrue( 1.0==f2.getMeasure() );
        self.assertTrue( 2==f2.getNumberOfNodes() );
        self.assertTrue( 2==f2.getNumberOfCells() );
        self.assertTrue( 0==f2.getRegion() );
        self.assertTrue( True==f2.isBorder() );
        self.assertTrue( "Bord1"==f2.getGroupName() );
        return

    def testClassIntTab(self):
        T=IntTab(3);
        T[0]=1;
        T[1]=2;
        T[2]=3;
        self.assertTrue( 1==T[0] );
        self.assertTrue( 2==T[1] );
        self.assertTrue( 3==T[2] );
    
        T1=IntTab(T);
        self.assertTrue( 1==T1[0] );
        self.assertTrue( 2==T1[1] );
        self.assertTrue( 3==T1[2] );
    
        T2=IntTab();
        T2=T;
        self.assertTrue( 1==T2[0] );
        self.assertTrue( 2==T2[1] );
        self.assertTrue( 3==T2[2] );
    
    
        T4=IntTab(3,3);
        T5=IntTab(3);
        T5[0]=1;
        T5[1]=2;
        T5[2]=3;
        T5+=T4;
        self.assertTrue( 4==T5[0] );
        self.assertTrue( 5==T5[1] );
        self.assertTrue( 6==T5[2] );
    
    
        T8=IntTab(3);
        T8[0]=1;
        T8[1]=2;
        T8[2]=3;
        T8-=T4;
        self.assertTrue( -2==T8[0] );
        self.assertTrue( -1==T8[1] );
        self.assertTrue( 0==T8[2] );
    
    
        T12=IntTab(3);
        T12[0]=1;
        T12[1]=2;
        T12[2]=3;
        self.assertTrue( 1==T12.getValues()[0] );
        self.assertTrue( 2==T12.getValues()[1] );
        self.assertTrue( 3==T12.getValues()[2] );        
        return

    def testClassDoubleTab(self):
        T=DoubleTab(3);
        T[0]=1;
        T[1]=2;
        T[2]=3;
        self.assertTrue( 1==T[0] );
        self.assertTrue( 2==T[1] );
        self.assertTrue( 3==T[2] );
    
        T1=DoubleTab(T);
        self.assertTrue( 1==T1[0] );
        self.assertTrue( 2==T1[1] );
        self.assertTrue( 3==T1[2] );
    
        T2=DoubleTab();
        T2=T;
        self.assertTrue( 1==T2[0] );
        self.assertTrue( 2==T2[1] );
        self.assertTrue( 3==T2[2] );
    
    
        T4=DoubleTab(3,3);
        T5=DoubleTab(3);
        T5[0]=1;
        T5[1]=2;
        T5[2]=3;
        T5+=T4;
        self.assertTrue( 4==T5[0] );
        self.assertTrue( 5==T5[1] );
        self.assertTrue( 6==T5[2] );
    
    
        T8=DoubleTab(3);
        T8[0]=1;
        T8[1]=2;
        T8[2]=3;
        T8-=T4;
        self.assertTrue( -2==T8[0] );
        self.assertTrue( -1==T8[1] );
        self.assertTrue( 0==T8[2] );
    
    
        T12=DoubleTab(3);
        T12[0]=1;
        T12[1]=2;
        T12[2]=3;
        self.assertTrue( 1==T12.getValues()[0] );
        self.assertTrue( 2==T12.getValues()[1] );
        self.assertTrue( 3==T12.getValues()[2] );        
        return

    def testClassMesh(self):
        M1=Mesh(0.0,1.0,4);
        self.assertTrue( 1==M1.getDim() );
        self.assertTrue( 5==M1.getNumberOfNodes() );
        self.assertTrue( 4==M1.getNumberOfCells() );
        self.assertTrue( 5==M1.getNumberOfFaces() );

        xinf=0.0;
        xsup=4.0;
        yinf=0.0;
        ysup=4.0;
        M2=Mesh(xinf,xsup,4,yinf,ysup,4);
        self.assertTrue( 4==M2.getNx() );
        self.assertTrue( 4==M2.getNy() );
        self.assertTrue( 0==M2.getNz() );
        self.assertTrue( 2==M2.getDim() );
        self.assertTrue( 25==M2.getNumberOfNodes() );
        self.assertTrue( 16==M2.getNumberOfCells() );
        self.assertTrue( 40==M2.getNumberOfFaces() );
#        x1=M2.getCells()[4].x();
#        y1=M2.getCells()[4].y();
#        self.assertTrue( x1==0.5 );
#        self.assertTrue( y1==1.5 );

#        x2=M2.getNodes()[24].x();
#        y2=M2.getNodes()[24].y();
#        self.assertTrue( x2==4. );
#        self.assertTrue( y2==4. );
        eps=1.E-10;
        M2.setGroupAtPlan(xsup,0,eps,"RightEdge");
        M2.setGroupAtPlan(xinf,0,eps,"LeftEdge");
        M2.setGroupAtPlan(yinf,1,eps,"BottomEdge");
        M2.setGroupAtPlan(ysup,1,eps,"TopEdge");
        self.assertTrue(M2.getNamesOfGroups()[2]=="BottomEdge");
        nbFaces=M2.getNumberOfFaces();
        indexFaces=M2.getIndexFacePeriodic();
        for i in range(nbFaces):
#            x=M2.getFaces()[i].x();
#            y=M2.getFaces()[i].y();
            x=M2.getFace(i).x();
            y=M2.getFace(i).y();
            if (abs(y)<1.E-10 and abs(x-0.5)<1.E-10):
                indexFace=M2.getIndexFacePeriodic(i);
                xi=M2.getFace(indexFace).x();
                yi=M2.getFace(indexFace).y();
                self.assertTrue( xi==x );
                self.assertTrue( yi==ysup );
                self.assertTrue( True==M2.getFace(indexFace).isBorder() );
                self.assertTrue( indexFace== indexFaces[i] );
                pass    
            if (abs(y-0.5)<1.E-10 and abs(x-1.)<1.E-10):
                self.assertTrue( -1==M2.getIndexFacePeriodic(i) );
                pass
            pass
    
    
        M2.writeMED("TestMesh.med");
        M22=Mesh("TestMesh.med");
        self.assertTrue( 2==M22.getDim() );
        self.assertTrue( 25==M22.getNumberOfNodes() );
        self.assertTrue( 16==M22.getNumberOfCells() );
        self.assertTrue( 40==M22.getNumberOfFaces() );
    
        M23=Mesh("maillage.med");
        self.assertTrue(M23.getNamesOfGroups()[0]=="BORD1");
        self.assertTrue(M23.getNamesOfGroups()[1]=="BORD2");
        self.assertTrue(M23.getNamesOfGroups()[2]=="BORD3");
        self.assertTrue(M23.getNamesOfGroups()[3]=="BORD4");
    
        M3=M1
        self.assertTrue( 1==M3.getDim() );
        self.assertTrue( 5==M3.getNumberOfNodes() );
        self.assertTrue( 4==M3.getNumberOfCells() );
        self.assertTrue( 5==M3.getNumberOfFaces() );
    
        M3=M2;
        self.assertTrue( 2==M3.getDim() );
        self.assertTrue( 25==M3.getNumberOfNodes() );
        self.assertTrue( 16==M3.getNumberOfCells() );
        self.assertTrue( 40==M3.getNumberOfFaces() );
    
        M4=M3;
        self.assertTrue( 2==M4.getDim() );
        self.assertTrue( 25==M4.getNumberOfNodes() );
        self.assertTrue( 16==M4.getNumberOfCells() );
        self.assertTrue( 40==M4.getNumberOfFaces() );
    
        M5=Mesh(0.0,1.0,4,0.0,1.0,4,0.0,1.0,4);
        self.assertTrue( 3==M5.getDim() );
        fileNameVTK="TestMesh.vtu";
        M4.writeVTK(fileNameVTK) ;
        fileNameMED="TestMesh.med";
        M4.writeMED(fileNameMED) ;
        M6=Mesh(fileNameMED);
        self.assertTrue( 2==M6.getDim() );
        self.assertTrue( 25==M6.getNumberOfNodes() );
        self.assertTrue( 16==M6.getNumberOfCells() );
        self.assertTrue( 40==M6.getNumberOfFaces() );
        return

if __name__ == '__main__':
    unittest.main()
