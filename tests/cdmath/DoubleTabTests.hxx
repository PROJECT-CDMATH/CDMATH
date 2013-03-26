/*
 * DoubleTabTests.hxx
 *
 *  Created on: 18 mars 2013
 *      Author: mekkas
 */

#ifndef DOUBLETABTESTS_HXX_
#define DOUBLETABTESTS_HXX_

#include <string>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>

#include "DoubleTab.hxx"

class DoubleTabTests : public CppUnit::TestCase
{
    public: //----------------------------------------------------------------
      void testClassDoubleTab( void );
      static CppUnit::Test *suite()
      {
        CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("Sample Unit Class DoubleTab Tests");
        suiteOfTests->addTest(new CppUnit::TestCaller<DoubleTabTests>("testClassDoubleTab", &DoubleTabTests::testClassDoubleTab));
        return suiteOfTests;
    }
};

#endif /* DOUBLETABTESTS_HXX_ */
