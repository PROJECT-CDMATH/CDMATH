/*
 * DoubleTabTests.cxx
 *
 *  Created on: 18 mars 2013
 *      Author: mekkas
 */



/*
 * celltests.cxx
 *
 *  Created on: 24 janv. 2012
 *      Authors: CDMAT
 */

#include "DoubleTabTests.hxx"

//----------------------------------------------------------------------
void
DoubleTabTests::testClassDoubleTab( void )
//----------------------------------------------------------------------
{
    DoubleTab T(3);
    T[0]=1.;
    T[1]=2.;
    T[2]=3.;
	CPPUNIT_ASSERT_EQUAL( 1.0, T[0] );
	CPPUNIT_ASSERT_EQUAL( 2.0, T[1] );
	CPPUNIT_ASSERT_EQUAL( 3.0, T[2] );

	DoubleTab T1(T);
	CPPUNIT_ASSERT_EQUAL( 1.0, T1[0] );
	CPPUNIT_ASSERT_EQUAL( 2.0, T1[1] );
	CPPUNIT_ASSERT_EQUAL( 3.0, T1[2] );

	DoubleTab T2;
	T2=T;
	CPPUNIT_ASSERT_EQUAL( 1.0, T2[0] );
	CPPUNIT_ASSERT_EQUAL( 2.0, T2[1] );
	CPPUNIT_ASSERT_EQUAL( 3.0, T2[2] );

	DoubleTab T3(3);
	T3=5.;
	CPPUNIT_ASSERT_EQUAL( 5.0, T3[0] );
	CPPUNIT_ASSERT_EQUAL( 5.0, T3[1] );
	CPPUNIT_ASSERT_EQUAL( 5.0, T3[2] );

	DoubleTab T4(3);
	DoubleTab T5(3);
	T4=3.;
	T5[0]=1.0;
	T5[1]=2.0;
	T5[2]=3.0;
	T5+=T4;
	CPPUNIT_ASSERT_EQUAL( 4.0, T5[0] );
	CPPUNIT_ASSERT_EQUAL( 5.0, T5[1] );
	CPPUNIT_ASSERT_EQUAL( 6.0, T5[2] );

	DoubleTab T6(3);
	T6[0]=1.0;
	T6[1]=2.0;
	T6[2]=3.0;
	T6+=3.0;
	CPPUNIT_ASSERT_EQUAL( 4.0, T6[0] );
	CPPUNIT_ASSERT_EQUAL( 5.0, T6[1] );
	CPPUNIT_ASSERT_EQUAL( 6.0, T6[2] );

	DoubleTab T7(3);
	DoubleTab T8(3);
	T7=3.;
	T8[0]=1.0;
	T8[1]=2.0;
	T8[2]=3.0;
	T8-=T7;
	CPPUNIT_ASSERT_EQUAL( -2., T8[0] );
	CPPUNIT_ASSERT_EQUAL( -1., T8[1] );
	CPPUNIT_ASSERT_EQUAL( 0. , T8[2] );

	DoubleTab T9(3);
	T9[0]=1.0;
	T9[1]=2.0;
	T9[2]=3.0;
	T9-=3.0;
	CPPUNIT_ASSERT_EQUAL( -2., T9[0] );
	CPPUNIT_ASSERT_EQUAL( -1., T9[1] );
	CPPUNIT_ASSERT_EQUAL( 0., T9[2] );

	DoubleTab T10(3);
	T10[0]=1.0;
	T10[1]=2.0;
	T10[2]=3.0;
	T10*=3.0;
	CPPUNIT_ASSERT_EQUAL( 3., T10[0] );
	CPPUNIT_ASSERT_EQUAL( 6., T10[1] );
	CPPUNIT_ASSERT_EQUAL( 9., T10[2] );

	DoubleTab T11(3);
	T11[0]=2.0;
	T11[1]=6.0;
	T11[2]=10.0;
	T11/=2.0;
	CPPUNIT_ASSERT_EQUAL( 1., T11[0] );
	CPPUNIT_ASSERT_EQUAL( 3., T11[1] );
	CPPUNIT_ASSERT_EQUAL( 5., T11[2] );

	DoubleTab T12(3);
	T12[0]=1.0;
	T12[1]=2.0;
	T12[2]=3.0;
	CPPUNIT_ASSERT_EQUAL( 1., T12.getPointer()[0] );
	CPPUNIT_ASSERT_EQUAL( 2., T12.getPointer()[1] );
	CPPUNIT_ASSERT_EQUAL( 3., T12.getPointer()[2] );
}

