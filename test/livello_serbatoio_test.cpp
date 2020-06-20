#include "livello_serbatoio_test.h"

#include <string>
#include <cstdint>
#include <cmath>

// dependencies
#include "Serial.h"

// UUT
#include "../livello_serbatoio.ino"

using namespace livelloSerbatoio_tests;

const uint32_t LivelloSebatoioTests::TIME_STEP_MS = 100U;

void LivelloSebatoioTests::timesim_thread ( void ) {
  while ( time_on == true ) {
    usleep(TIME_STEP_MS);
    
    timePassedFromBootMs += TIME_STEP_MS;
  }

}


void LivelloSebatoioTests::testSetup ( void ) {
  
  arduino_pins[ECHO_DPIN].pulse_time = 10;
  
  loopNTimes ( 10 );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL(100.0, percentage, 0.1); 
}


void LivelloSebatoioTests::setUp ( void ) {
  std::cout << std::endl << HARNESS_PREFIX << "Set-Up" << std::endl;
  
  timePassedFromBootMs = 0U;
  time_on              = true;
  
  cycle_num            = 0U;
  
  time_thread = std::thread(&LivelloSebatoioTests::timesim_thread, this);
  
  setup();
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
    
    loop();
  }
}
