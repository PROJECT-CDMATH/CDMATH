/*
 * fieldtests.cxx
 *
 *  Created on: 24 janv. 2012
 *      Authors: CDMAT
 */

#include "FieldTests.hxx"
#include <string>

using namespace std;
using namespace ParaMEDMEM;

//----------------------------------------------------------------------
void
FieldTests::testClassField( void )
//----------------------------------------------------------------------
{
	Mesh M(0.0,1.0,10,0.,1.,5);

	Field conc1("CONCENTRATION",ON_CELLS,M,2,1.2) ;
	CPPUNIT_ASSERT_EQUAL( 1.2, conc1.getTime() );
    for (int i=0;i<conc1.getNumberOfElements();i++)
    	conc1(0,i)=i*1.0;
    string fileNameVTK="champ.vtu";
    conc1.writeVTK(fileNameVTK);

    string fileNameMED="champ.med";
    conc1.writeMED(fileNameMED);
    conc1.setTime(2.3,1);
    conc1.writeMED(fileNameMED,false);

    for (int i=0;i<conc1.getNumberOfElements();i++)
    	CPPUNIT_ASSERT_EQUAL( 1.0*i, conc1(0,i) );
	CPPUNIT_ASSERT_EQUAL( 2, conc1.getNumberOfComponents() );
	CPPUNIT_ASSERT_EQUAL( 50, conc1.getNumberOfElements() );
	CPPUNIT_ASSERT_EQUAL( 2.3, conc1.getTime() );

	Field conc1n("CONCENTRATION",ON_NODES,M,2,1.2) ;
	CPPUNIT_ASSERT_EQUAL( 1.2, conc1n.getTime() );
    for (int i=0;i<conc1n.getNumberOfElements();i++)
    	conc1n(0,i)=i*1.0;
    string fileNameVTKn="champn.vtu";
    conc1n.writeVTK(fileNameVTKn);

    string fileNameMEDn="champn.med";
    conc1n.writeMED(fileNameMEDn);
    conc1n.setTime(2.3,1);
    conc1n.writeMED(fileNameMEDn,false);

    for (int i=0;i<conc1n.getNumberOfElements();i++)
    	CPPUNIT_ASSERT_EQUAL( 1.0*i, conc1n(0,i) );
	CPPUNIT_ASSERT_EQUAL( 2, conc1n.getNumberOfComponents() );
	CPPUNIT_ASSERT_EQUAL( 66, conc1n.getNumberOfElements() );
	CPPUNIT_ASSERT_EQUAL( 2.3, conc1n.getTime() );

	Field conc6("CONCENTRATION",ON_CELLS,M,2);
    for (int i=0;i<conc6.getNumberOfComponents();i++)
    	for (int j=0;j<conc6.getNumberOfElements();j++)
    		conc6(i,j)=i*1.0+2.*j;

    for (int i=0;i<conc6.getNumberOfComponents();i++)
        for (int j=0;j<conc6.getNumberOfElements();j++)
        	CPPUNIT_ASSERT_EQUAL( 1.0*i+2.*j, conc6.getValues()[i+j*conc6.getNumberOfComponents()] );

    CPPUNIT_ASSERT_EQUAL( 2, conc6.getNumberOfComponents() );
	CPPUNIT_ASSERT_EQUAL( 50, conc6.getNumberOfElements() );

	Field conc6n("CONCENTRATION",ON_NODES,M,2);
    for (int i=0;i<conc6n.getNumberOfComponents();i++)
    	for (int j=0;j<conc6n.getNumberOfElements();j++)
    		conc6n(i,j)=i*1.0+2.*j;

    for (int i=0;i<conc6n.getNumberOfComponents();i++)
        for (int j=0;j<conc6n.getNumberOfElements();j++)
        	CPPUNIT_ASSERT_EQUAL( 1.0*i+2.*j, conc6n.getValues()[i+j*conc6n.getNumberOfComponents()] );

    CPPUNIT_ASSERT_EQUAL( 2, conc6n.getNumberOfComponents() );
	CPPUNIT_ASSERT_EQUAL( 66, conc6n.getNumberOfElements() );

	Field conc3(conc1) ;
    for (int i=0;i<conc3.getNumberOfElements();i++)
    	conc3(0,i)=i*1.0;

    double x=conc3(2);
	CPPUNIT_ASSERT_EQUAL( x, 2.0 );

    for (int i=0;i<conc3.getNumberOfElements();i++)
    	CPPUNIT_ASSERT_EQUAL( 1.0*i, conc3(i) );
	CPPUNIT_ASSERT_EQUAL( 2, conc3.getNumberOfComponents() );
	CPPUNIT_ASSERT_EQUAL( 50, conc3.getNumberOfElements() );

	conc6=conc3+conc1;
    for (int i=0;i<conc6.getNumberOfElements();i++)
    	CPPUNIT_ASSERT_EQUAL( 2.0*i, conc6[i] );
	CPPUNIT_ASSERT_EQUAL( 2, conc6.getNumberOfComponents() );
	CPPUNIT_ASSERT_EQUAL( 50, conc6.getNumberOfElements() );

	conc6=conc3-conc1;
    for (int i=0;i<conc6.getNumberOfElements();i++)
    	CPPUNIT_ASSERT_EQUAL( 0.0, conc6(i) );
	CPPUNIT_ASSERT_EQUAL( 2, conc6.getNumberOfComponents() );
	CPPUNIT_ASSERT_EQUAL( 50, conc6.getNumberOfElements() );

	conc6=conc1;
	conc6+=conc1;
    for (int i=0;i<conc6.getNumberOfElements();i++)
    	CPPUNIT_ASSERT_EQUAL( 2.0*i, conc6(0,i) );
	CPPUNIT_ASSERT_EQUAL( 2, conc6.getNumberOfComponents() );
	CPPUNIT_ASSERT_EQUAL( 50, conc6.getNumberOfElements() );

	conc6=conc1;
	conc6*=2.0;
    for (int i=0;i<conc6.getNumberOfElements();i++)
    	CPPUNIT_ASSERT_EQUAL( 2.0*i, conc6[i] );
	CPPUNIT_ASSERT_EQUAL( 2, conc6.getNumberOfComponents() );
	CPPUNIT_ASSERT_EQUAL( 50, conc6.getNumberOfElements() );

	Field conc7("CONCENTRATION",ON_CELLS,M,2) ;
	conc7.setField(conc1.getField());
    conc7.setName("CONC");
    for (int i=0;i<conc7.getNumberOfElements();i++)
    {
    	CPPUNIT_ASSERT_EQUAL( 1.0*i, conc7(i) );
    	CPPUNIT_ASSERT_EQUAL( 1.0*i, conc7[i] );
    }
	CPPUNIT_ASSERT_EQUAL( 2, conc7.getNumberOfComponents() );
	CPPUNIT_ASSERT_EQUAL( 50, conc7.getNumberOfElements() );
	CPPUNIT_ASSERT( conc7.getName().compare("CONC")==0 );

	Field conc8("CONCENTRATION",ON_CELLS,M) ;
    for (int i=0;i<conc8.getNumberOfElements();i++)
    	conc8[i]=i*1.0;
    for (int i=0;i<conc8.getNumberOfElements();i++)
    {
    	CPPUNIT_ASSERT_EQUAL( 1.0*i, conc8(i) );
    	CPPUNIT_ASSERT_EQUAL( 1.0*i, conc8[i] );
    }
	CPPUNIT_ASSERT_EQUAL( 1, conc8.getNumberOfComponents() );
	CPPUNIT_ASSERT_EQUAL( 50, conc8.getNumberOfElements() );

	Field conc8n("CONCENTRATION",ON_NODES,M) ;
    for (int i=0;i<conc8n.getNumberOfElements();i++)
    	conc8n[i]=i*1.0;
    for (int i=0;i<conc8n.getNumberOfElements();i++)
    {
    	CPPUNIT_ASSERT_EQUAL( 1.0*i, conc8n(i) );
    	CPPUNIT_ASSERT_EQUAL( 1.0*i, conc8n[i] );
    }
	CPPUNIT_ASSERT_EQUAL( 1, conc8n.getNumberOfComponents() );
	CPPUNIT_ASSERT_EQUAL( 66, conc8n.getNumberOfElements() );

	Field conc9=conc8 ;
	conc9/=2.0;
    for (int i=0;i<conc9.getNumberOfElements();i++)
    	CPPUNIT_ASSERT_EQUAL( 1.0*i/2., conc9(i) );

    Field conc10 ;
	conc10=conc8;
	conc10-=2.0;
    for (int i=0;i<conc10.getNumberOfElements();i++)
    	CPPUNIT_ASSERT_EQUAL( 1.0*i-2.0, conc10(i) );

    Field conc11=conc8 ;
	conc11+=2.0;
    for (int i=0;i<conc11.getNumberOfElements();i++)
    	CPPUNIT_ASSERT_EQUAL( 1.0*i+2.0, conc11(i) );

    Field conc12=conc8 ;
	conc12+=conc8;
    for (int i=0;i<conc12.getNumberOfElements();i++)
    	CPPUNIT_ASSERT_EQUAL( 2.0*i, conc12(i) );

    Field conc13=conc8 ;
	conc13-=conc8;
    for (int i=0;i<conc13.getNumberOfElements();i++)
    	CPPUNIT_ASSERT_EQUAL( 0.0, conc13(i) );
}
