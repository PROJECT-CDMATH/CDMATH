/*
 * linearSolvertests.cxx
 *
 *  Created on: 24 janv. 2012
 *      Authors: CDMAT
 */

#include "Matrix.hxx"
#include "Vector.hxx"
#include "LinearSolverTests.hxx"

using namespace std;

//----------------------------------------------------------------------
void
LinearSolverTests::testClassLinearSolver( void )
//----------------------------------------------------------------------
{
	Matrix A(2,2);
    A(0,0)=3.;
    A(0,1)=-2.;
    A(1,0)=-2.;
    A(1,1)=4.;

    A*=A.transpose();

    Vector Xana(2);
    Xana(0)=1.;
    Xana(1)=2.;

    Vector B=A*Xana;

    LinearSolver LS(A,B,500,1.E-10,"GMRES","LU");

    Vector X=LS.solve();
	CPPUNIT_ASSERT(abs(X(0)-Xana(0))<1.E-10);
	CPPUNIT_ASSERT(abs(X(1)-Xana(1))<1.E-10);

	CPPUNIT_ASSERT_EQUAL(LS.getStatus(),true);

	CPPUNIT_ASSERT_EQUAL(LS.getNumberMaxOfIter(),500);
	CPPUNIT_ASSERT_EQUAL(LS.getTolerance(),1.E-10);
	CPPUNIT_ASSERT_EQUAL(LS.getNameOfMethod(),(string)"GMRES");
	CPPUNIT_ASSERT_EQUAL(LS.getNumberOfIter(),1);
	CPPUNIT_ASSERT_EQUAL(LS.isSingular(),false);
	CPPUNIT_ASSERT_EQUAL(LS.getNameOfPc(),(string)"LU");
	LS.setNameOfPc("");
	LinearSolver LS2;
    A(0,0)=1.;
    A(0,1)=-2.;
    A(1,0)=-2.;
    A(1,1)=4.;
	LS2=LS;
	LS2.setMatrix(-1.*A);
	LS2.setSndMember(-1*B);
	LS2.setTolerance(1.E-20);
	LS2.setNumberMaxOfIter(10);
	LS2.setNameOfMethod("CG");
	LS2.setSingularity(true);
	Vector X2=LS2.solve();

	CPPUNIT_ASSERT(abs(X2(0)-(-4.55555555556))<1.E-10);
	CPPUNIT_ASSERT(abs(X2(1)-4.55555555556)<1.E-10);
	CPPUNIT_ASSERT_EQUAL(LS.getStatus(),true);
	CPPUNIT_ASSERT_EQUAL(LS2.getNumberOfIter(),2);
	CPPUNIT_ASSERT_EQUAL(LS2.isSingular(),true);
	CPPUNIT_ASSERT_EQUAL(LS2.getNameOfMethod(),(string)"CG");

	LinearSolver LS3(LS);
	LS3.setNameOfMethod("BCG");
    Vector X3=LS3.solve();
	CPPUNIT_ASSERT(abs(X3(0)-Xana(0))<1.E-10);
	CPPUNIT_ASSERT(abs(X3(1)-2.)<1.E-10);
	CPPUNIT_ASSERT_EQUAL(LS3.getStatus(),true);
	CPPUNIT_ASSERT_EQUAL(LS3.getNameOfMethod(),(string)"BCG");

	LS3.setNameOfMethod("CR");
    X3=LS3.solve();
	CPPUNIT_ASSERT(abs(X3(0)-Xana(0))<1.E-10);
	CPPUNIT_ASSERT(abs(X3(1)-2.)<1.E-10);
	CPPUNIT_ASSERT_EQUAL(LS3.getStatus(),true);
	CPPUNIT_ASSERT_EQUAL(LS3.getNameOfMethod(),(string)"CR");

	LS3.setNameOfMethod("CGS");
    X3=LS3.solve();
	CPPUNIT_ASSERT(abs(X3(0)-Xana(0))<1.E-10);
	CPPUNIT_ASSERT(abs(X3(1)-2.)<1.E-10);
	CPPUNIT_ASSERT_EQUAL(LS3.getStatus(),true);
	CPPUNIT_ASSERT_EQUAL(LS3.getNameOfMethod(),(string)"CGS");

	LS3.setNameOfMethod("BICG");
    LS3.setNameOfPc("LU");
    X3=LS3.solve();
	CPPUNIT_ASSERT(abs(X3(0)-Xana(0))<1.E-10);
	CPPUNIT_ASSERT(abs(X3(1)-2.)<1.E-10);
	CPPUNIT_ASSERT_EQUAL(LS3.getStatus(),true);
	CPPUNIT_ASSERT_EQUAL(LS3.getNameOfMethod(),(string)"BICG");
	CPPUNIT_ASSERT_EQUAL(LS3.getNameOfPc(),(string)"LU");
	LS3.setNameOfPc("");

	LS3.setNameOfMethod("GCR");
    X3=LS3.solve();
	CPPUNIT_ASSERT(abs(X3(0)-Xana(0))<1.E-10);
	CPPUNIT_ASSERT(abs(X3(1)-2.)<1.E-10);
	CPPUNIT_ASSERT_EQUAL(LS3.getStatus(),true);
	CPPUNIT_ASSERT_EQUAL(LS3.getNameOfMethod(),(string)"GCR");

	LS3.setNameOfMethod("LSQR");
    X3=LS3.solve();
	CPPUNIT_ASSERT(abs(X3(0)-Xana(0))<1.E-10);
	CPPUNIT_ASSERT(abs(X3(1)-2.)<1.E-10);
	CPPUNIT_ASSERT_EQUAL(LS3.getStatus(),true);
	CPPUNIT_ASSERT_EQUAL(LS3.getNameOfMethod(),(string)"LSQR");

	LS3.setNameOfMethod("CHOLESKY");
    X3=LS3.solve();
	CPPUNIT_ASSERT(abs(X3(0)-Xana(0))<1.E-10);
	CPPUNIT_ASSERT(abs(X3(1)-2.)<1.E-10);
	CPPUNIT_ASSERT_EQUAL(LS3.getStatus(),true);
	CPPUNIT_ASSERT_EQUAL(LS3.getNameOfMethod(),(string)"CHOLESKY");

	LS3.setNameOfMethod("LU");
    X3=LS3.solve();
	CPPUNIT_ASSERT(abs(X3(0)-Xana(0))<1.E-10);
	CPPUNIT_ASSERT(abs(X3(1)-2.)<1.E-10);
	CPPUNIT_ASSERT_EQUAL(LS3.getStatus(),true);
	CPPUNIT_ASSERT_EQUAL(LS3.getNameOfMethod(),(string)"LU");
}
