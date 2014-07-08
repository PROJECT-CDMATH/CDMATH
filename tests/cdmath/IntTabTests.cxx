/*
 * IntTabTests.cxx
 *
 *  Created on: 18 mars 2013
 *      Author: mekkas
 */


/*
 * IntTabTests.cxx
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

#include "IntTabTests.hxx"

//----------------------------------------------------------------------
void
IntTabTests::testClassIntTab( void )
//----------------------------------------------------------------------
{
    IntTab T(3);
    T[0]=1;
    T[1]=2;
    T[2]=3;
	CPPUNIT_ASSERT_EQUAL( 1, T[0] );
	CPPUNIT_ASSERT_EQUAL( 2, T[1] );
	CPPUNIT_ASSERT_EQUAL( 3, T[2] );

	CPPUNIT_ASSERT_EQUAL( 1, T(0) );
	CPPUNIT_ASSERT_EQUAL( 2, T(1) );
	CPPUNIT_ASSERT_EQUAL( 3, T(2) );

	IntTab T1(T);
	CPPUNIT_ASSERT_EQUAL( 1, T1[0] );
	CPPUNIT_ASSERT_EQUAL( 2, T1[1] );
	CPPUNIT_ASSERT_EQUAL( 3, T1[2] );

	IntTab T2;
	T2=T;
	CPPUNIT_ASSERT_EQUAL( 1, T2[0] );
	CPPUNIT_ASSERT_EQUAL( 2, T2[1] );
	CPPUNIT_ASSERT_EQUAL( 3, T2[2] );

	IntTab T3(3);
	T3=5;
	CPPUNIT_ASSERT_EQUAL( 5, T3[0] );
	CPPUNIT_ASSERT_EQUAL( 5, T3[1] );
	CPPUNIT_ASSERT_EQUAL( 5, T3[2] );

	IntTab T4(3);
	IntTab T5(3);
	T4=3;
	T5[0]=1;
	T5[1]=2;
	T5[2]=3;
	T5+=T4;
	CPPUNIT_ASSERT_EQUAL( 4, T5[0] );
	CPPUNIT_ASSERT_EQUAL( 5, T5[1] );
	CPPUNIT_ASSERT_EQUAL( 6, T5[2] );

	IntTab T6(3);
	T6[0]=1;
	T6[1]=2;
	T6[2]=3;
	T6+=3;
	CPPUNIT_ASSERT_EQUAL( 4, T6[0] );
	CPPUNIT_ASSERT_EQUAL( 5, T6[1] );
	CPPUNIT_ASSERT_EQUAL( 6, T6[2] );

	IntTab T7(3);
	IntTab T8(3);
	T7=3;
	T8[0]=1;
	T8[1]=2;
	T8[2]=3;
	T8-=T7;
	CPPUNIT_ASSERT_EQUAL( -2, T8[0] );
	CPPUNIT_ASSERT_EQUAL( -1, T8[1] );
	CPPUNIT_ASSERT_EQUAL( 0, T8[2] );

	IntTab T9(3);
	T9[0]=1;
	T9[1]=2;
	T9[2]=3;
	T9-=3;
	CPPUNIT_ASSERT_EQUAL( -2, T9[0] );
	CPPUNIT_ASSERT_EQUAL( -1, T9[1] );
	CPPUNIT_ASSERT_EQUAL( 0, T9[2] );

	IntTab T12(3);
	T12[0]=1;
	T12[1]=2;
	T12[2]=3;
	CPPUNIT_ASSERT_EQUAL( 1, T12.getPointer()[0] );
	CPPUNIT_ASSERT_EQUAL( 2, T12.getPointer()[1] );
	CPPUNIT_ASSERT_EQUAL( 3, T12.getPointer()[2] );

	IntTab T10(3);
	T10[0]=1.0;
	T10[1]=2.0;
	T10[2]=3.0;
	T10*=3.0;
	CPPUNIT_ASSERT_EQUAL( 3, T10[0] );
	CPPUNIT_ASSERT_EQUAL( 6, T10[1] );
	CPPUNIT_ASSERT_EQUAL( 9, T10[2] );
	T10/=3.0;
	CPPUNIT_ASSERT_EQUAL( 1, T10[0] );
	CPPUNIT_ASSERT_EQUAL( 2, T10[1] );
	CPPUNIT_ASSERT_EQUAL( 3, T10[2] );
}



