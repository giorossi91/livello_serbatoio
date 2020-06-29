#ifndef LIVELLO_SERBATOIO_TEST_H_
#define LIVELLO_SERBATOIO_TEST_H_

#include <cppunit/extensions/HelperMacros.h>

#include <thread>
#include <atomic>
#include <cstdint>

namespace livelloSerbatoio_tests {

class LivelloSerbatoioTests  : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(LivelloSerbatoioTests);
  // === Unit tests ===
  CPPUNIT_TEST(test_PinSetup);
  CPPUNIT_TEST(test_compute_liters);
  CPPUNIT_TEST(test_compute_percentage);
  CPPUNIT_TEST(test_turn_on_lcd_light);
  CPPUNIT_TEST(test_turn_off_lcd_light);
  CPPUNIT_TEST(test_measure_level);
  CPPUNIT_TEST(test_control_led);
  CPPUNIT_TEST(test_sanitize_data);
  CPPUNIT_TEST(test_update_lcd);


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

  void test_compute_liters     ( void );
  void test_compute_percentage ( void );
  void test_PinSetup           ( void );
  void test_turn_on_lcd_light  ( void );
  void test_turn_off_lcd_light ( void );
  void test_measure_level      ( void );
  void test_control_led        ( void );
  void test_sanitize_data      ( void );
  void test_update_lcd         ( void );

  
  // ==================

private:
  static const uint32_t TIME_STEP_MS;
  
  std::atomic<bool> time_on;
  std::thread       time_thread;

  uint32_t cycle_num;
  
  void timesim_thread ( void );
  
  void start_time ( void );
  void stop_time  ( void );

  void loopNTimes ( uint32_t n );
};

}

#endif // LIVELLO_SERBATOIO_TEST_H_
