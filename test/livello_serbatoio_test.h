#ifndef LIVELLO_SERBATOIO_TEST_H_
#define LIVELLO_SERBATOIO_TEST_H_

#include <cppunit/extensions/HelperMacros.h>

namespace livelloSerbatoio_tests {

class LivelloSebatoioTests  : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(LivelloSebatoioTests);
  // === Unit tests ===
  CPPUNIT_TEST(testSetup);
  // TODO da fare
  // ==================
  CPPUNIT_TEST_SUITE_END();
  
public:
  void setUp(void);
  void tearDown(void);

protected:
  // === Unit tests ===
  void testSetup(void);
  
  // ==================

private:

};

}

#endif // LIVELLO_SERBATOIO_TEST_H_
