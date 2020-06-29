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

#define DEBUG  CONF_RELEASE  //<-- release configuration
#define SENSOR SENSOR_HCSR04 //<-- sensor actually used

namespace uut {
#include "livello_serbatoio_uut.hpp"
}
// <--

using namespace livelloSerbatoio_tests;

const uint32_t LivelloSerbatoioTests::TIME_STEP_MS = 100U;

// Unit tests

void LivelloSerbatoioTests::test_PinSetup ( void ) {
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

void LivelloSerbatoioTests::test_compute_percentage ( void ) {
	uut::setup(); // to compute maximum capacity

	double liters;

	liters = -DBL_MAX;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut::compute_percentage(liters), 0.1);

	liters = -1.0;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut::compute_percentage(liters), 0.1);

	liters = 0.0;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut::compute_percentage(liters), 0.1);

	liters = 1.0;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1, uut::compute_percentage(liters), 0.1);

	liters = uut::maximum_capacity - 1.0;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(99.9, uut::compute_percentage(liters), 0.1);

	liters = uut::maximum_capacity;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(100.0, uut::compute_percentage(liters), 0.1);

	liters = uut::maximum_capacity + 1.0;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(100.0, uut::compute_percentage(liters), 0.1);

	liters = DBL_MAX;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(100.0, uut::compute_percentage(liters), 0.1);
}

void LivelloSerbatoioTests::test_compute_liters ( void ) {
	double distance;

	//
	distance = -DBL_MAX;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1200.7, uut::compute_liters(distance), 0.1);

	distance = -1.0;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1200.7, uut::compute_liters(distance), 0.1);

	distance = 0.0;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1200.7, uut::compute_liters(distance), 0.1);

	distance = 1.0;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1193.0, uut::compute_liters(distance), 0.1);

	//
	distance = uut::SENSOR_DISTANCE - 1.0;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1008.2, uut::compute_liters(distance), 0.1);

	distance = uut::SENSOR_DISTANCE;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1000.5, uut::compute_liters(distance), 0.1);

	distance = uut::SENSOR_DISTANCE + 1.0;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(992.9, uut::compute_liters(distance), 0.1);

	//
	distance = uut::TANK_HEIGHT_CM - 1.0;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(7.6, uut::compute_liters(distance), 0.1);

	distance = uut::TANK_HEIGHT_CM;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut::compute_liters(distance), 0.1);

	distance = uut::TANK_HEIGHT_CM + 1.0;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut::compute_liters(distance), 0.1); //volume can't be negative

	distance = DBL_MAX;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut::compute_liters(distance), 0.1); //volume can't be negative
}

void LivelloSerbatoioTests::test_turn_on_lcd_light  ( void ) {

	timePassedFromBootMs = rand() % 1000U; // any

	uut::turn_on_lcd_light();

	uint32_t uut_timestamp_lcd = uut::timestamp_lcd_on;

	CPPUNIT_ASSERT_EQUAL(HIGH                       , arduino_pins[uut::LCD_LIGHT_DPIN].in_val);
	CPPUNIT_ASSERT_EQUAL(timePassedFromBootMs.load(), uut_timestamp_lcd                       );
}

void LivelloSerbatoioTests::test_turn_off_lcd_light  ( void ) {
	uut::turn_off_lcd_light();

	CPPUNIT_ASSERT_EQUAL(LOW, arduino_pins[uut::LCD_LIGHT_DPIN].in_val);
}

void LivelloSerbatoioTests::test_measure_level ( void ) {
	double level;

	// timeout
	arduino_pins[uut::ECHO_DPIN].pulse_time = 0;

	level = uut::measure_level();

	CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, level, 0.1);

	//
	arduino_pins[uut::ECHO_DPIN].pulse_time = 1 * 58; // from datasheet ( range = 2 - 400 cm - lsb is 1/58 cm/us )

	level = uut::measure_level();

	CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, level, 0.1);

	//
	arduino_pins[uut::ECHO_DPIN].pulse_time = 2 * 58;

	level = uut::measure_level();

	CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0 + uut::SENSOR_CALIBRATION, level, 0.1);

	//
	arduino_pins[uut::ECHO_DPIN].pulse_time = 3 * 58;

	level = uut::measure_level();

	CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0 + uut::SENSOR_CALIBRATION, level, 0.1);

	//
	arduino_pins[uut::ECHO_DPIN].pulse_time = 399 * 58;

	level = uut::measure_level();

	CPPUNIT_ASSERT_DOUBLES_EQUAL(399.0 + uut::SENSOR_CALIBRATION, level, 0.1);

	//
	arduino_pins[uut::ECHO_DPIN].pulse_time = 400 * 58;

	level = uut::measure_level();

	CPPUNIT_ASSERT_DOUBLES_EQUAL(400.0 + uut::SENSOR_CALIBRATION, level, 0.1);

	//
	arduino_pins[uut::ECHO_DPIN].pulse_time = 401 * 58;

	level = uut::measure_level();

	CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, level, 0.1);

	//
	arduino_pins[uut::ECHO_DPIN].pulse_time = UINT32_MAX;

	level = uut::measure_level();

	CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, level, 0.1);
}

void LivelloSerbatoioTests::test_control_led ( void ) {
	double level;

	//
	arduino_pins[uut::LED_CAPACITY_DPIN].in_val = LOW;
	uut::led_on = false;
	uut::led_status = false;
	level = -DBL_MAX;

	uut::control_led(level);
	CPPUNIT_ASSERT_EQUAL(LOW, arduino_pins[uut::LED_CAPACITY_DPIN].in_val);
	uut::control_led(level);
	CPPUNIT_ASSERT_EQUAL(LOW, arduino_pins[uut::LED_CAPACITY_DPIN].in_val);
	uut::control_led(level);
	CPPUNIT_ASSERT_EQUAL(LOW, arduino_pins[uut::LED_CAPACITY_DPIN].in_val);
	CPPUNIT_ASSERT_EQUAL(false, uut::led_status);

	//
	arduino_pins[uut::LED_CAPACITY_DPIN].in_val = LOW;
	uut::led_on = false;
	uut::led_status = false;
	level = 0.0;

	uut::control_led(level);
	CPPUNIT_ASSERT_EQUAL(HIGH, arduino_pins[uut::LED_CAPACITY_DPIN].in_val);
	uut::control_led(level);
	CPPUNIT_ASSERT_EQUAL(LOW, arduino_pins[uut::LED_CAPACITY_DPIN].in_val);
	uut::control_led(level);
	CPPUNIT_ASSERT_EQUAL(HIGH, arduino_pins[uut::LED_CAPACITY_DPIN].in_val);
	CPPUNIT_ASSERT_EQUAL(true, uut::led_status);

	//
	arduino_pins[uut::LED_CAPACITY_DPIN].in_val = LOW;
	uut::led_on = false;
	uut::led_status = false;
	level = uut::EMPTY_LEVEL_THRESHOLD;

	uut::control_led(level);
	CPPUNIT_ASSERT_EQUAL(HIGH, arduino_pins[uut::LED_CAPACITY_DPIN].in_val);
	uut::control_led(level);
	CPPUNIT_ASSERT_EQUAL(LOW, arduino_pins[uut::LED_CAPACITY_DPIN].in_val);
	uut::control_led(level);
	CPPUNIT_ASSERT_EQUAL(HIGH, arduino_pins[uut::LED_CAPACITY_DPIN].in_val);
	CPPUNIT_ASSERT_EQUAL(true, uut::led_status);

	//
	arduino_pins[uut::LED_CAPACITY_DPIN].in_val = LOW;
	uut::led_on = false;
	uut::led_status = false;
	level = uut::EMPTY_LEVEL_THRESHOLD + 1.0;

	uut::control_led(level);
	CPPUNIT_ASSERT_EQUAL(HIGH, arduino_pins[uut::LED_CAPACITY_DPIN].in_val);
	uut::control_led(level);
	CPPUNIT_ASSERT_EQUAL(HIGH, arduino_pins[uut::LED_CAPACITY_DPIN].in_val);
	uut::control_led(level);
	CPPUNIT_ASSERT_EQUAL(HIGH, arduino_pins[uut::LED_CAPACITY_DPIN].in_val);
	CPPUNIT_ASSERT_EQUAL(true, uut::led_status);

	//
	arduino_pins[uut::LED_CAPACITY_DPIN].in_val = LOW;
	uut::led_on = false;
	uut::led_status = false;
	level = uut::LOW_LEVEL_THRESHOLD - 1.0;

	uut::control_led(level);
	CPPUNIT_ASSERT_EQUAL(HIGH, arduino_pins[uut::LED_CAPACITY_DPIN].in_val);
	uut::control_led(level);
	CPPUNIT_ASSERT_EQUAL(HIGH, arduino_pins[uut::LED_CAPACITY_DPIN].in_val);
	uut::control_led(level);
	CPPUNIT_ASSERT_EQUAL(HIGH, arduino_pins[uut::LED_CAPACITY_DPIN].in_val);
	CPPUNIT_ASSERT_EQUAL(true, uut::led_status);

	//
	arduino_pins[uut::LED_CAPACITY_DPIN].in_val = LOW;
	uut::led_on = false;
	uut::led_status = false;
	level = uut::LOW_LEVEL_THRESHOLD;

	uut::control_led(level);
	CPPUNIT_ASSERT_EQUAL(HIGH, arduino_pins[uut::LED_CAPACITY_DPIN].in_val);
	uut::control_led(level);
	CPPUNIT_ASSERT_EQUAL(HIGH, arduino_pins[uut::LED_CAPACITY_DPIN].in_val);
	uut::control_led(level);
	CPPUNIT_ASSERT_EQUAL(HIGH, arduino_pins[uut::LED_CAPACITY_DPIN].in_val);
	CPPUNIT_ASSERT_EQUAL(true, uut::led_status);


	//
	arduino_pins[uut::LED_CAPACITY_DPIN].in_val = LOW;
	uut::led_on = false;
	uut::led_status = false;
	level = uut::LOW_LEVEL_THRESHOLD + 1.0;

	uut::control_led(level);
	CPPUNIT_ASSERT_EQUAL(LOW, arduino_pins[uut::LED_CAPACITY_DPIN].in_val);
	uut::control_led(level);
	CPPUNIT_ASSERT_EQUAL(LOW, arduino_pins[uut::LED_CAPACITY_DPIN].in_val);
	uut::control_led(level);
	CPPUNIT_ASSERT_EQUAL(LOW, arduino_pins[uut::LED_CAPACITY_DPIN].in_val);
	CPPUNIT_ASSERT_EQUAL(false, uut::led_status);

	//
	arduino_pins[uut::LED_CAPACITY_DPIN].in_val = LOW;
	uut::led_on = false;
	uut::led_status = false;
	level = 100.0;

	uut::control_led(level);
	CPPUNIT_ASSERT_EQUAL(LOW, arduino_pins[uut::LED_CAPACITY_DPIN].in_val);
	uut::control_led(level);
	CPPUNIT_ASSERT_EQUAL(LOW, arduino_pins[uut::LED_CAPACITY_DPIN].in_val);
	uut::control_led(level);
	CPPUNIT_ASSERT_EQUAL(LOW, arduino_pins[uut::LED_CAPACITY_DPIN].in_val);
	CPPUNIT_ASSERT_EQUAL(false, uut::led_status);

	//
	arduino_pins[uut::LED_CAPACITY_DPIN].in_val = LOW;
	uut::led_on = false;
	uut::led_status = false;
	level = DBL_MAX;

	uut::control_led(level);
	CPPUNIT_ASSERT_EQUAL(LOW, arduino_pins[uut::LED_CAPACITY_DPIN].in_val);
	uut::control_led(level);
	CPPUNIT_ASSERT_EQUAL(LOW, arduino_pins[uut::LED_CAPACITY_DPIN].in_val);
	uut::control_led(level);
	CPPUNIT_ASSERT_EQUAL(LOW, arduino_pins[uut::LED_CAPACITY_DPIN].in_val);

	CPPUNIT_ASSERT_EQUAL(false, uut::led_status);

}

void LivelloSerbatoioTests::test_sanitize_data ( void ) {

	double data;
	double min_val;
	double max_val;
	double min_thr;
	double max_thr;

	min_val = 0.0;
	max_val = 100.0;

	min_thr = 0.5;
	max_thr = 99.5;

	data = min_val;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(min_val, uut::sanitize_data(data, min_val, max_val, min_thr, max_thr), 0.1);

	data = min_thr - 0.1;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(min_val, uut::sanitize_data(data, min_val, max_val, min_thr, max_thr), 0.1);

	data = min_thr;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(data, uut::sanitize_data(data, min_val, max_val, min_thr, max_thr), 0.1);

	data = max_thr;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(data, uut::sanitize_data(data, min_val, max_val, min_thr, max_thr), 0.1);

	data = max_thr + 0.1;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(max_val, uut::sanitize_data(data, min_val, max_val, min_thr, max_thr), 0.1);

	data = max_val;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(max_val, uut::sanitize_data(data, min_val, max_val, min_thr, max_thr), 0.1);
}

void LivelloSerbatoioTests::test_update_lcd ( void ) {
	std::string text = "";
	uut::lcd.clear();
	
	
	// Update off
	uut::must_update_lcd = false;
	uut::update_lcd(0.0, 0.0);
	
	text = uut::lcd.harness_getLcdText();
	CPPUNIT_ASSERT_EQUAL(std::string("                \n                "), text);
	
	
	// Update on
	uut::must_update_lcd = true;
	uut::update_lcd(0.0, 0.0);
	
	text = uut::lcd.harness_getLcdText();
	CPPUNIT_ASSERT_EQUAL(std::string("    0 L     0 % \n     Vuoto      "), text);
	
	uut::must_update_lcd = true;
	uut::update_lcd(15.0, 150.0);
	
	text = uut::lcd.harness_getLcdText();
	CPPUNIT_ASSERT_EQUAL(std::string("  150 L    15 % \n ##  Riserva    "), text);
	
	uut::must_update_lcd = true;
	uut::update_lcd(14.0, 140.0);
	
	text = uut::lcd.harness_getLcdText();
	CPPUNIT_ASSERT_EQUAL(std::string("  140 L    14 % \n #   Riserva    "), text);
	
	uut::must_update_lcd = true;
	uut::update_lcd(9.0, 90.0);
	
	text = uut::lcd.harness_getLcdText();
	CPPUNIT_ASSERT_EQUAL(std::string("   90 L     9 % \n #   Riserva    "), text);
	
	uut::must_update_lcd = true;
	uut::update_lcd(100.0, 1000.0);
	
	text = uut::lcd.harness_getLcdText();
	CPPUNIT_ASSERT_EQUAL(std::string(" 1000 L   100 % \n ############## "), text);
	
	// check ranges
	text = uut::lcd.harness_getLcdText();

	uut::must_update_lcd = true;
	uut::update_lcd(-0.1, 0.0);
	CPPUNIT_ASSERT_EQUAL( false, uut::must_update_lcd         );
	CPPUNIT_ASSERT_EQUAL( text , uut::lcd.harness_getLcdText());
	
	uut::must_update_lcd = true;
	uut::update_lcd(100.1, 0.0);
	CPPUNIT_ASSERT_EQUAL( false, uut::must_update_lcd         );
	CPPUNIT_ASSERT_EQUAL( text , uut::lcd.harness_getLcdText());
	
	uut::must_update_lcd = true;
	uut::update_lcd(0.0, -0.1);
	CPPUNIT_ASSERT_EQUAL( false, uut::must_update_lcd         );
	CPPUNIT_ASSERT_EQUAL( text , uut::lcd.harness_getLcdText());
	
	uut::must_update_lcd = true;
	uut::update_lcd(0.0, 10000.0);
	CPPUNIT_ASSERT_EQUAL( false, uut::must_update_lcd         );
	CPPUNIT_ASSERT_EQUAL( text , uut::lcd.harness_getLcdText());
}


void LivelloSerbatoioTests::testSetup ( void ) {

//  uut::setup();
//
//  arduino_pins[uut::ECHO_DPIN].pulse_time = 10;
//
//  loopNTimes ( 10 );
//
//  CPPUNIT_ASSERT_DOUBLES_EQUAL(100.0, uut::percentage, 0.1);
}

// Harness utilities

void LivelloSerbatoioTests::timesim_thread ( void ) {
  while ( time_on == true ) {
    usleep(TIME_STEP_MS);
    
    timePassedFromBootMs += TIME_STEP_MS;
  }
}

void LivelloSerbatoioTests::setUp ( void ) {
  std::cout << std::endl << HARNESS_PREFIX << "Set-Up" << std::endl;
  
  cycle_num = 0U;
}

void LivelloSerbatoioTests::start_time ( void ) {
  timePassedFromBootMs = 0U;
  time_on              = true;
  time_thread = std::thread(&LivelloSerbatoioTests::timesim_thread, this);
}

void LivelloSerbatoioTests::stop_time ( void ) {
  time_on = false;
  time_thread.join();
  timePassedFromBootMs = 0U;
}

void LivelloSerbatoioTests::tearDown ( void ) {
  std::cout << std::endl << HARNESS_PREFIX << "Tear-Down" << std::endl;
}

void LivelloSerbatoioTests::loopNTimes ( uint32_t n ) {
  for (uint32_t i = 0U; i < n; i++) {
    cycle_num ++;
    
    std::cout << "====== CYCLE " << cycle_num << " ======" << std::endl;
    
    uut::loop();
  }
}
