/*
 * DoubleTabTests.hxx
 *
 *  Created on: 18 mars 2013
 *      Author: mekkas
 */

#ifndef SPARSEMATRIXTESTS_HXX_
#define SPARSEMATRIXTESTS_HXX_

#include <string>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>

#include "SparseMatrix.hxx"

class SparseMatrixTests : public CppUnit::TestCase
{
    public: //----------------------------------------------------------------
      void testClassSparseMatrix( void );
      static CppUnit::Test *suite()
      {
        CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("Sample Unit Class DoubleTab Tests");
        suiteOfTests->addTest(new CppUnit::TestCaller<SparseMatrixTests>("testClassSparseMatrix", &SparseMatrixTests::testClassSparseMatrix));
        return suiteOfTests;
    }
};

#endif /* SparseMatrixTESTS_HXX_ */
