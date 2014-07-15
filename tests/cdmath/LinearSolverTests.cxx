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

	LinearSolver LS2(A,B,500,1.E-10,"CG");
	Matrix A1(2,2);
    A1(0,0)=1.;
    A1(0,1)=-2.;
    A1(1,0)=-2.;
    A1(1,1)=4.;
	LS2.setMatrix(-1.*A1);
	LS2.setSndMember(-1*B);
	LS2.setTolerance(1.E-20);
	LS2.setNumberMaxOfIter(10);
	LS2.setSingularity(true);
	Vector X2=LS2.solve();

	CPPUNIT_ASSERT(abs(X2(0)-(-4.55555555556))<1.E-10);
	CPPUNIT_ASSERT(abs(X2(1)-4.55555555556)<1.E-10);
	CPPUNIT_ASSERT_EQUAL(LS.getStatus(),true);
	CPPUNIT_ASSERT_EQUAL(LS2.getNumberOfIter(),2);
	CPPUNIT_ASSERT_EQUAL(LS2.isSingular(),true);
	CPPUNIT_ASSERT_EQUAL(LS2.getNameOfMethod(),(string)"CG");

	LinearSolver LS3(A,B,500,1.E-10,"BCG");
    Vector X3=LS3.solve();
	CPPUNIT_ASSERT(abs(X3(0)-Xana(0))<1.E-10);
	CPPUNIT_ASSERT(abs(X3(1)-2.)<1.E-10);
	CPPUNIT_ASSERT_EQUAL(LS3.getStatus(),true);
	CPPUNIT_ASSERT_EQUAL(LS3.getNameOfMethod(),(string)"BCG");

	LinearSolver LS4(A,B,500,1.E-10,"CR");
    X3=LS4.solve();
	CPPUNIT_ASSERT(abs(X3(0)-Xana(0))<1.E-10);
	CPPUNIT_ASSERT(abs(X3(1)-2.)<1.E-10);
	CPPUNIT_ASSERT_EQUAL(LS4.getStatus(),true);
	CPPUNIT_ASSERT_EQUAL(LS4.getNameOfMethod(),(string)"CR");

	LinearSolver LS5(A,B,500,1.E-10,"CGS");
    X3=LS5.solve();
	CPPUNIT_ASSERT(abs(X3(0)-Xana(0))<1.E-10);
	CPPUNIT_ASSERT(abs(X3(1)-2.)<1.E-10);
	CPPUNIT_ASSERT_EQUAL(LS5.getStatus(),true);
	CPPUNIT_ASSERT_EQUAL(LS5.getNameOfMethod(),(string)"CGS");

	LinearSolver LS6(A,B,500,1.E-10,"BICG","LU");
    X3=LS6.solve();
	CPPUNIT_ASSERT(abs(X3(0)-Xana(0))<1.E-10);
	CPPUNIT_ASSERT(abs(X3(1)-2.)<1.E-10);
	CPPUNIT_ASSERT_EQUAL(LS6.getStatus(),true);
	CPPUNIT_ASSERT_EQUAL(LS6.getNameOfMethod(),(string)"BICG");
	CPPUNIT_ASSERT_EQUAL(LS6.getNameOfPc(),(string)"LU");

	LinearSolver LS7(A,B,500,1.E-10,"GCR");
    X3=LS7.solve();
	CPPUNIT_ASSERT(abs(X3(0)-Xana(0))<1.E-10);
	CPPUNIT_ASSERT(abs(X3(1)-2.)<1.E-10);
	CPPUNIT_ASSERT_EQUAL(LS7.getStatus(),true);
	CPPUNIT_ASSERT_EQUAL(LS7.getNameOfMethod(),(string)"GCR");

	LinearSolver LS8(A,B,500,1.E-10,"LSQR");
    X3=LS8.solve();
	CPPUNIT_ASSERT(abs(X3(0)-Xana(0))<1.E-10);
	CPPUNIT_ASSERT(abs(X3(1)-2.)<1.E-10);
	CPPUNIT_ASSERT_EQUAL(LS8.getStatus(),true);
	CPPUNIT_ASSERT_EQUAL(LS8.getNameOfMethod(),(string)"LSQR");

	LinearSolver LS9(A,B,500,1.E-10,"CHOLESKY");
    X3=LS9.solve();
	CPPUNIT_ASSERT(abs(X3(0)-Xana(0))<1.E-10);
	CPPUNIT_ASSERT(abs(X3(1)-2.)<1.E-10);
	CPPUNIT_ASSERT_EQUAL(LS9.getStatus(),true);
	CPPUNIT_ASSERT_EQUAL(LS9.getNameOfMethod(),(string)"CHOLESKY");

	LinearSolver LS10(A,B,500,1.E-10,"LU");
    X3=LS10.solve();
	CPPUNIT_ASSERT(abs(X3(0)-Xana(0))<1.E-10);
	CPPUNIT_ASSERT(abs(X3(1)-2.)<1.E-10);
	CPPUNIT_ASSERT_EQUAL(LS10.getStatus(),true);
	CPPUNIT_ASSERT_EQUAL(LS10.getNameOfMethod(),(string)"LU");
}
