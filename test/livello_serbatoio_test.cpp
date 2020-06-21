#include "livello_serbatoio_test.h"

#include <string>
#include <cstdint>
#include <cmath>

// dependencies
#include "arduino_types.h"
#include "arduino_stubs.h"
#include "Serial.h"
#include "LiquidCrystal.h"

// --> UUT
#define UNIT_TEST

namespace uut {
#include "livello_serbatoio_uut.hpp"
}
// <--

using namespace livelloSerbatoio_tests;

const uint32_t LivelloSebatoioTests::TIME_STEP_MS = 100U;

// Unit tests

void LivelloSebatoioTests::testPinSetup ( void ) {
  CPPUNIT_ASSERT_EQUAL(static_cast<int16_t>(D2 ), uut::LCD_BUTTON_DPIN   );
  CPPUNIT_ASSERT_EQUAL(static_cast<int16_t>(D3 ), uut::RS                );
  CPPUNIT_ASSERT_EQUAL(static_cast<int16_t>(D4 ), uut::E                 );
  CPPUNIT_ASSERT_EQUAL(static_cast<int16_t>(D5 ), uut::DB4               );
  CPPUNIT_ASSERT_EQUAL(static_cast<int16_t>(D6 ), uut::DB5               );
  CPPUNIT_ASSERT_EQUAL(static_cast<int16_t>(D7 ), uut::DB6               );
  CPPUNIT_ASSERT_EQUAL(static_cast<int16_t>(D8 ), uut::DB7               );
  CPPUNIT_ASSERT_EQUAL(static_cast<int16_t>(D9 ), uut::LCD_LIGHT_DPIN    );
  CPPUNIT_ASSERT_EQUAL(static_cast<int16_t>(D11), uut::TRIG_DPIN         );
  CPPUNIT_ASSERT_EQUAL(static_cast<int16_t>(D12), uut::ECHO_DPIN         );
  CPPUNIT_ASSERT_EQUAL(static_cast<int16_t>(D13), uut::LED_CAPACITY_DPIN );
}


void LivelloSebatoioTests::testSetup ( void ) {

  uut::setup();
  
  arduino_pins[uut::ECHO_DPIN].pulse_time = 10;
  
  loopNTimes ( 10 );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL(100.0, uut::percentage, 0.1); 
}

// Harness utilities

void LivelloSebatoioTests::timesim_thread ( void ) {
  while ( time_on == true ) {
    usleep(TIME_STEP_MS);
    
    timePassedFromBootMs += TIME_STEP_MS;
  }
}

void LivelloSebatoioTests::setUp ( void ) {
  std::cout << std::endl << HARNESS_PREFIX << "Set-Up" << std::endl;
  
  timePassedFromBootMs = 0U;
  time_on              = true;
  
  cycle_num            = 0U;
  
  time_thread = std::thread(&LivelloSebatoioTests::timesim_thread, this);
}

void LivelloSebatoioTests::tearDown ( void ) {
  std::cout << std::endl << HARNESS_PREFIX << "Tear-Down" << std::endl;
  
  time_on = false;
  time_thread.join();
  timePassedFromBootMs = 0U;
}

void LivelloSebatoioTests::loopNTimes ( uint32_t n ) {
  for (uint32_t i = 0U; i < n; i++) {
    cycle_num ++;
    
    std::cout << "====== CYCLE " << cycle_num << " ======" << std::endl;
    
    uut::loop();
  }
}
