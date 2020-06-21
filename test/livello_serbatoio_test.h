#ifndef LIVELLO_SERBATOIO_TEST_H_
#define LIVELLO_SERBATOIO_TEST_H_

#include <cppunit/extensions/HelperMacros.h>

#include <thread>
#include <atomic>
#include <cstdint>

namespace livelloSerbatoio_tests {

class LivelloSebatoioTests  : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(LivelloSebatoioTests);
  // === Unit tests ===
  CPPUNIT_TEST(testPinSetup);
  CPPUNIT_TEST(testSetup);
  // TODO da fare
  // ==================
  CPPUNIT_TEST_SUITE_END();
  
public:
  void setUp   ( void );
  void tearDown( void );

protected:
  // === Unit tests ===
  void testSetup    ( void );
  void testPinSetup ( void );
  
  // ==================

private:
  static const uint32_t TIME_STEP_MS;
  
  std::atomic<bool> time_on;
  std::thread       time_thread;

  uint32_t cycle_num;
  
  void timesim_thread ( void );
  
  void loopNTimes ( uint32_t n );
};

}

#endif // LIVELLO_SERBATOIO_TEST_H_
