/*
 * IntTabTests.hxx
 *
 *  Created on: 18 mars 2013
 *      Author: mekkas
 */

#ifndef INTTABTESTS_HXX_
#define INTTABTESTS_HXX_



#include <string>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>

#include "IntTab.hxx"

class IntTabTests : public CppUnit::TestCase
{
    public: //----------------------------------------------------------------
      void testClassIntTab( void );
      static CppUnit::Test *suite()
      {
        CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("Sample Unit Class IntTab Tests");
        suiteOfTests->addTest(new CppUnit::TestCaller<IntTabTests>("testClassIntTab", &IntTabTests::testClassIntTab));
        return suiteOfTests;
    }
};


#endif /* INTTABTESTS_HXX_ */
