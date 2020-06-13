#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

// Test suites
#include "livello_serbatoio_test.h"


CPPUNIT_TEST_SUITE_REGISTRATION( livelloSerbatoio_tests::LivelloSebatoioTests );

int main( void ) {

  CppUnit::Test *suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();

  CppUnit::TextUi::TestRunner runner;
  runner.addTest( suite );

  runner.setOutputter( new CppUnit::CompilerOutputter( &runner.result(), std::cerr ) );
  
  // Run the tests.
  bool wasSucessful = runner.run();

  // Return error code 1 if the one of test failed.
  return wasSucessful ? 0 : 1;
}
