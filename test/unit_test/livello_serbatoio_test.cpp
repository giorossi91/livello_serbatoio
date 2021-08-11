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

#define private public
#define volatile 

namespace uut {
#include "livello_serbatoio_uut.hpp"
}

#undef private
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
    distance = uut::SENSOR_DISTANCE_CM - 1.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1008.2, uut::compute_liters(distance), 0.1);

    distance = uut::SENSOR_DISTANCE_CM;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1000.5, uut::compute_liters(distance), 0.1);

    distance = uut::SENSOR_DISTANCE_CM + 1.0;
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

    harness_getBoard()->harness_setTimePassedFromBoot(rand() % 1000); // any

    uut::turn_on_lcd_light();

    uint32_t uut_timestamp_lcd = uut::timestamp_lcd_on;

    CPPUNIT_ASSERT_EQUAL(HIGH, harness_getBoard()->harness_getPinInternalValue(uut::LCD_LIGHT_DPIN));
    CPPUNIT_ASSERT_EQUAL(harness_getBoard()->harness_getTimePassedFromBoot(), uut_timestamp_lcd);
}

void LivelloSerbatoioTests::test_turn_off_lcd_light  ( void ) {
    uut::turn_off_lcd_light();

    CPPUNIT_ASSERT_EQUAL(LOW, harness_getBoard()->harness_getPinInternalValue(uut::LCD_LIGHT_DPIN));
}

void LivelloSerbatoioTests::test_measure_level ( void ) {
    double level;

    // timeout
    harness_getBoard()->harness_setPulseTime(uut::ECHO_DPIN, 0);

    level = uut::measure_level();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, level, 0.1);

    //
    harness_getBoard()->harness_setPulseTime(uut::ECHO_DPIN, static_cast<uint32_t> ( ( uut::SENSOR_MIN_RANGE - 1.0 ) * uut::SENSOR_LSB ) ); // from datasheet ( range = 2 - 400 cm - lsb is 1/58 cm/us )

    level = uut::measure_level();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, level, 0.1);

    //
    harness_getBoard()->harness_setPulseTime(uut::ECHO_DPIN, static_cast<uint32_t> ( ( uut::SENSOR_MIN_RANGE ) * uut::SENSOR_LSB ) );

    level = uut::measure_level();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(uut::SENSOR_MIN_RANGE + uut::SENSOR_CALIBRATION, level, 0.1);

    //
    harness_getBoard()->harness_setPulseTime(uut::ECHO_DPIN, static_cast<uint32_t> ( ( uut::SENSOR_MIN_RANGE + 1.0) * uut::SENSOR_LSB ) );

    level = uut::measure_level();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(uut::SENSOR_MIN_RANGE + 1.0 + uut::SENSOR_CALIBRATION, level, 0.1);

    //
    harness_getBoard()->harness_setPulseTime(uut::ECHO_DPIN, static_cast<uint32_t> ( ( uut::SENSOR_MAX_RANGE - 1.0 ) * uut::SENSOR_LSB ) );

    level = uut::measure_level();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(( uut::SENSOR_MAX_RANGE - 1.0 ) + uut::SENSOR_CALIBRATION, level, 0.1);

    //
    harness_getBoard()->harness_setPulseTime(uut::ECHO_DPIN, static_cast<uint32_t> ( ( uut::SENSOR_MAX_RANGE ) * uut::SENSOR_LSB ) );

    level = uut::measure_level();

    CPPUNIT_ASSERT_DOUBLES_EQUAL( ( uut::SENSOR_MAX_RANGE ) + uut::SENSOR_CALIBRATION, level, 0.1);

    //
    harness_getBoard()->harness_setPulseTime(uut::ECHO_DPIN, static_cast<uint32_t> ( ( uut::SENSOR_MAX_RANGE + 1.0 ) * uut::SENSOR_LSB ) );

    level = uut::measure_level();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, level, 0.1);

    //
    harness_getBoard()->harness_setPulseTime(uut::ECHO_DPIN, UINT32_MAX );

    level = uut::measure_level();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, level, 0.1);
}

void LivelloSerbatoioTests::test_control_led ( void ) {
    double level;

    //
    harness_getBoard()->harness_setPinValue(uut::LED_CAPACITY_DPIN, OUTPUT, LOW);
    uut::led_on = false;
    uut::led_status = false;
    level = -DBL_MAX;

    uut::control_led(level);
    CPPUNIT_ASSERT_EQUAL(LOW, harness_getBoard()->harness_getPinInternalValue( uut::LED_CAPACITY_DPIN ) );
    uut::control_led(level);
    CPPUNIT_ASSERT_EQUAL(LOW, harness_getBoard()->harness_getPinInternalValue( uut::LED_CAPACITY_DPIN ) );
    uut::control_led(level);
    CPPUNIT_ASSERT_EQUAL(LOW, harness_getBoard()->harness_getPinInternalValue( uut::LED_CAPACITY_DPIN ) );
    CPPUNIT_ASSERT_EQUAL(false, uut::led_status);

    //
    harness_getBoard()->harness_setPinValue(uut::LED_CAPACITY_DPIN, OUTPUT, LOW);
    uut::led_on = false;
    uut::led_status = false;
    level = 0.0;

    uut::control_led(level);
    CPPUNIT_ASSERT_EQUAL(HIGH, harness_getBoard()->harness_getPinInternalValue( uut::LED_CAPACITY_DPIN ) );
    uut::control_led(level);
    CPPUNIT_ASSERT_EQUAL(LOW, harness_getBoard()->harness_getPinInternalValue( uut::LED_CAPACITY_DPIN ) );
    uut::control_led(level);
    CPPUNIT_ASSERT_EQUAL(HIGH, harness_getBoard()->harness_getPinInternalValue( uut::LED_CAPACITY_DPIN ) );
    CPPUNIT_ASSERT_EQUAL(true, uut::led_status);

    //
    harness_getBoard()->harness_setPinValue(uut::LED_CAPACITY_DPIN, OUTPUT, LOW);
    uut::led_on = false;
    uut::led_status = false;
    level = uut::EMPTY_LEVEL_THRESHOLD;

    uut::control_led(level);
    CPPUNIT_ASSERT_EQUAL(HIGH, harness_getBoard()->harness_getPinInternalValue( uut::LED_CAPACITY_DPIN ) );
    uut::control_led(level);
    CPPUNIT_ASSERT_EQUAL(LOW, harness_getBoard()->harness_getPinInternalValue( uut::LED_CAPACITY_DPIN ) );
    uut::control_led(level);
    CPPUNIT_ASSERT_EQUAL(HIGH, harness_getBoard()->harness_getPinInternalValue( uut::LED_CAPACITY_DPIN ) );
    CPPUNIT_ASSERT_EQUAL(true, uut::led_status);

    //
    harness_getBoard()->harness_setPinValue(uut::LED_CAPACITY_DPIN, OUTPUT, LOW);
    uut::led_on = false;
    uut::led_status = false;
    level = uut::EMPTY_LEVEL_THRESHOLD + 1.0;

    uut::control_led(level);
    CPPUNIT_ASSERT_EQUAL(HIGH, harness_getBoard()->harness_getPinInternalValue( uut::LED_CAPACITY_DPIN ) );
    uut::control_led(level);
    CPPUNIT_ASSERT_EQUAL(HIGH, harness_getBoard()->harness_getPinInternalValue( uut::LED_CAPACITY_DPIN ) );
    uut::control_led(level);
    CPPUNIT_ASSERT_EQUAL(HIGH, harness_getBoard()->harness_getPinInternalValue( uut::LED_CAPACITY_DPIN ) );
    CPPUNIT_ASSERT_EQUAL(true, uut::led_status);

    //
    harness_getBoard()->harness_setPinValue(uut::LED_CAPACITY_DPIN, OUTPUT, LOW);
    uut::led_on = false;
    uut::led_status = false;
    level = uut::LOW_LEVEL_THRESHOLD - 1.0;

    uut::control_led(level);
    CPPUNIT_ASSERT_EQUAL(HIGH, harness_getBoard()->harness_getPinInternalValue( uut::LED_CAPACITY_DPIN ) );
    uut::control_led(level);
    CPPUNIT_ASSERT_EQUAL(HIGH, harness_getBoard()->harness_getPinInternalValue( uut::LED_CAPACITY_DPIN ) );
    uut::control_led(level);
    CPPUNIT_ASSERT_EQUAL(HIGH, harness_getBoard()->harness_getPinInternalValue( uut::LED_CAPACITY_DPIN ) );
    CPPUNIT_ASSERT_EQUAL(true, uut::led_status);

    //
    harness_getBoard()->harness_setPinValue(uut::LED_CAPACITY_DPIN, OUTPUT, LOW);
    uut::led_on = false;
    uut::led_status = false;
    level = uut::LOW_LEVEL_THRESHOLD;

    uut::control_led(level);
    CPPUNIT_ASSERT_EQUAL(HIGH, harness_getBoard()->harness_getPinInternalValue( uut::LED_CAPACITY_DPIN ) );
    uut::control_led(level);
    CPPUNIT_ASSERT_EQUAL(HIGH, harness_getBoard()->harness_getPinInternalValue( uut::LED_CAPACITY_DPIN ) );
    uut::control_led(level);
    CPPUNIT_ASSERT_EQUAL(HIGH, harness_getBoard()->harness_getPinInternalValue( uut::LED_CAPACITY_DPIN ) );
    CPPUNIT_ASSERT_EQUAL(true, uut::led_status);

    //
    harness_getBoard()->harness_setPinValue(uut::LED_CAPACITY_DPIN, OUTPUT, LOW);
    uut::led_on = false;
    uut::led_status = false;
    level = uut::LOW_LEVEL_THRESHOLD + 1.0;

    uut::control_led(level);
    CPPUNIT_ASSERT_EQUAL(LOW, harness_getBoard()->harness_getPinInternalValue( uut::LED_CAPACITY_DPIN ) );
    uut::control_led(level);
    CPPUNIT_ASSERT_EQUAL(LOW, harness_getBoard()->harness_getPinInternalValue( uut::LED_CAPACITY_DPIN ) );
    uut::control_led(level);
    CPPUNIT_ASSERT_EQUAL(LOW, harness_getBoard()->harness_getPinInternalValue( uut::LED_CAPACITY_DPIN ) );
    CPPUNIT_ASSERT_EQUAL(false, uut::led_status);

    //
    harness_getBoard()->harness_setPinValue(uut::LED_CAPACITY_DPIN, OUTPUT, LOW);
    uut::led_on = false;
    uut::led_status = false;
    level = 100.0;

    uut::control_led(level);
    CPPUNIT_ASSERT_EQUAL(LOW, harness_getBoard()->harness_getPinInternalValue( uut::LED_CAPACITY_DPIN ) );
    uut::control_led(level);
    CPPUNIT_ASSERT_EQUAL(LOW, harness_getBoard()->harness_getPinInternalValue( uut::LED_CAPACITY_DPIN ) );
    uut::control_led(level);
    CPPUNIT_ASSERT_EQUAL(LOW, harness_getBoard()->harness_getPinInternalValue( uut::LED_CAPACITY_DPIN ) );
    CPPUNIT_ASSERT_EQUAL(false, uut::led_status);

    //
    harness_getBoard()->harness_setPinValue(uut::LED_CAPACITY_DPIN, OUTPUT, LOW);
    uut::led_on = false;
    uut::led_status = false;
    level = DBL_MAX;

    uut::control_led(level);
    CPPUNIT_ASSERT_EQUAL(LOW, harness_getBoard()->harness_getPinInternalValue( uut::LED_CAPACITY_DPIN ) );
    uut::control_led(level);
    CPPUNIT_ASSERT_EQUAL(LOW, harness_getBoard()->harness_getPinInternalValue( uut::LED_CAPACITY_DPIN ) );
    uut::control_led(level);
    CPPUNIT_ASSERT_EQUAL(LOW, harness_getBoard()->harness_getPinInternalValue( uut::LED_CAPACITY_DPIN ) );

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

void LivelloSerbatoioTests::test_loop_btn ( void ) {
    start_time();

    uut::setup();

    stop_time();

}

void LivelloSerbatoioTests::test_loop ( void ) {
    start_time();

    uut::setup();


    stop_time();

}

// Harness utilities

void LivelloSerbatoioTests::timesim_thread ( void ) {
    while ( time_on == true ) {
        usleep(TIME_STEP_MS * 1000);

        uint32_t currentTimePassedFromBoot = harness_getBoard()->harness_getTimePassedFromBoot();
        harness_getBoard()->harness_setTimePassedFromBoot(currentTimePassedFromBoot + TIME_STEP_MS );
    }
}

void LivelloSerbatoioTests::setUp ( void ) {
    std::cout << std::endl << HARNESS_PREFIX << "Set-Up" << std::endl;

    ArduinoBoardStub *board = new ArduinoBoardStub();
    harness_setBoard(board);

    uut::lcd.harness_setPrintFailFunction(printFail);

    cycle_num = 0U;
}

void LivelloSerbatoioTests::start_time ( void ) {
    harness_getBoard()->harness_setTimePassedFromBoot( 0U );
    time_on              = true;
    time_thread = std::thread(&LivelloSerbatoioTests::timesim_thread, this);
}

void LivelloSerbatoioTests::stop_time ( void ) {
    time_on = false;
    time_thread.join();
    harness_getBoard()->harness_setTimePassedFromBoot( 0U );
}

void LivelloSerbatoioTests::tearDown ( void ) {
    std::cout << std::endl << HARNESS_PREFIX << "Tear-Down" << std::endl;

    delete harness_getBoard();
}

void LivelloSerbatoioTests::loopNTimes ( uint32_t n ) {
    for (uint32_t i = 0U; i < n; i++) {
        cycle_num ++;

        std::cout << "====== CYCLE " << cycle_num << " ======" << std::endl;

        uut::loop();
    }
}

void LivelloSerbatoioTests::printFail(const char *str) {
    CPPUNIT_FAIL(str);
}


void LivelloSerbatoioTests::test_MedianFilter_init ( void ) {
    int16_t init_value = rand() % 1000; // any;

    uut::MedianFilter mf1;
    mf1.begin(init_value);

    CPPUNIT_ASSERT_EQUAL(static_cast<byte>(uut::FILTER_SIZE    ), mf1.medFilterWin  );
    CPPUNIT_ASSERT_EQUAL(static_cast<byte>(uut::FILTER_SIZE / 2), mf1.medDataPointer);
    for(byte i = 0U; i < uut::FILTER_SIZE; i++) {
        CPPUNIT_ASSERT_EQUAL(init_value, mf1.data[i]);
    }
}

void LivelloSerbatoioTests::test_MedianFilter_in ( void ) {
    uut::MedianFilter mf1;                                            //  0  0  0  0  0
    mf1.begin(0);

    CPPUNIT_ASSERT_EQUAL(static_cast<int16_t> (  0 ), mf1.in( 1));    //  0  0  0  0  1
    CPPUNIT_ASSERT_EQUAL(static_cast<int16_t> (  0 ), mf1.in( 1));    //  0  0  0  1  1
    CPPUNIT_ASSERT_EQUAL(static_cast<int16_t> (  1 ), mf1.in( 1));    //  0  0  1  1  1
    CPPUNIT_ASSERT_EQUAL(static_cast<int16_t> (  1 ), mf1.in( 2));    //  0  1  1  1  2
    CPPUNIT_ASSERT_EQUAL(static_cast<int16_t> (  1 ), mf1.in( 2));    //  1  1  1  2  2
    CPPUNIT_ASSERT_EQUAL(static_cast<int16_t> (  2 ), mf1.in( 2));    //  1  1  2  2  2
    CPPUNIT_ASSERT_EQUAL(static_cast<int16_t> (  2 ), mf1.in(10));    //  1  2  2  2 10
    CPPUNIT_ASSERT_EQUAL(static_cast<int16_t> (  2 ), mf1.in( 3));    //  2  2  2 10  3
    CPPUNIT_ASSERT_EQUAL(static_cast<int16_t> (  3 ), mf1.in( 3));    //  2  2 10  3  3
    CPPUNIT_ASSERT_EQUAL(static_cast<int16_t> (  3 ), mf1.in( 0));    //  2 10  3  3  0
    CPPUNIT_ASSERT_EQUAL(static_cast<int16_t> (  3 ), mf1.in( 0));    // 10  3  3  0  0
    CPPUNIT_ASSERT_EQUAL(static_cast<int16_t> (  3 ), mf1.in(11));    //  3  3  0  0 11
    CPPUNIT_ASSERT_EQUAL(static_cast<int16_t> (  3 ), mf1.in(10));    //  3  0  0 11 10
    CPPUNIT_ASSERT_EQUAL(static_cast<int16_t> ( 10 ), mf1.in(11));    //  0  0 11 10 11
}

void LivelloSerbatoioTests::test_MedianFilter_out ( void ) {
    uut::MedianFilter mf1;
    mf1.begin(0);

    CPPUNIT_ASSERT_EQUAL(mf1.out(), mf1.in( 1));
    CPPUNIT_ASSERT_EQUAL(mf1.out(), mf1.in( 1));
    CPPUNIT_ASSERT_EQUAL(mf1.out(), mf1.in( 1));
    CPPUNIT_ASSERT_EQUAL(mf1.out(), mf1.in( 2));
    CPPUNIT_ASSERT_EQUAL(mf1.out(), mf1.in( 2));
    CPPUNIT_ASSERT_EQUAL(mf1.out(), mf1.in( 2));
    CPPUNIT_ASSERT_EQUAL(mf1.out(), mf1.in(10));
    CPPUNIT_ASSERT_EQUAL(mf1.out(), mf1.in( 3));
    CPPUNIT_ASSERT_EQUAL(mf1.out(), mf1.in( 3));
    CPPUNIT_ASSERT_EQUAL(mf1.out(), mf1.in( 0));
    CPPUNIT_ASSERT_EQUAL(mf1.out(), mf1.in( 0));
    CPPUNIT_ASSERT_EQUAL(mf1.out(), mf1.in(11));
    CPPUNIT_ASSERT_EQUAL(mf1.out(), mf1.in(10));
    CPPUNIT_ASSERT_EQUAL(mf1.out(), mf1.in(11));
}


void LivelloSerbatoioTests::test_ConsumptionData_init ( void ) {
    harness_getBoard()->harness_setTimePassedFromBoot( 100U );

    uut::ConsumptionData sc1;
    sc1.begin();

    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>( harness_getBoard()->harness_getTimePassedFromBoot() ), sc1.last_millis);
}

void LivelloSerbatoioTests::test_ConsumptionData_updateTime ( void ) {
    harness_getBoard()->harness_setTimePassedFromBoot( 0U );

    uut::ConsumptionData sc1;
    sc1.begin();

    harness_getBoard()->harness_setTimePassedFromBoot( 0U );
    sc1.updateTime();
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>( harness_getBoard()->harness_getTimePassedFromBoot() ), sc1.last_millis   );
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(                                                   0 ), sc1.millis_passed );
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(                                                   0 ), sc1.seconds_passed);

    harness_getBoard()->harness_setTimePassedFromBoot( 1U );
    sc1.updateTime();
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>( harness_getBoard()->harness_getTimePassedFromBoot() ), sc1.last_millis   );
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(                                                   1 ), sc1.millis_passed );
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(                                                   0 ), sc1.seconds_passed);

    harness_getBoard()->harness_setTimePassedFromBoot( 999U );
    sc1.updateTime();
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>( harness_getBoard()->harness_getTimePassedFromBoot() ), sc1.last_millis   );
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(                                                 999 ), sc1.millis_passed );
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(                                                   0 ), sc1.seconds_passed);

    harness_getBoard()->harness_setTimePassedFromBoot( 1000U );
    sc1.updateTime();
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>( harness_getBoard()->harness_getTimePassedFromBoot() ), sc1.last_millis   );
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(                                                   0 ), sc1.millis_passed );
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(                                                   1 ), sc1.seconds_passed);

    harness_getBoard()->harness_setTimePassedFromBoot( 1001U );
    sc1.updateTime();
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>( harness_getBoard()->harness_getTimePassedFromBoot() ), sc1.last_millis   );
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(                                                   1 ), sc1.millis_passed );
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(                                                   1 ), sc1.seconds_passed);

    harness_getBoard()->harness_setTimePassedFromBoot( 3600000U - 1U );
    sc1.updateTime();
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>( harness_getBoard()->harness_getTimePassedFromBoot() ), sc1.last_millis   );
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(                                                 999 ), sc1.millis_passed );
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(                                                3599 ), sc1.seconds_passed);

    harness_getBoard()->harness_setTimePassedFromBoot( 3600000U );
    sc1.updateTime();
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>( harness_getBoard()->harness_getTimePassedFromBoot() ), sc1.last_millis   );
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(                                                   0 ), sc1.millis_passed );
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(                                                   0 ), sc1.seconds_passed);

    harness_getBoard()->harness_setTimePassedFromBoot( 3600000U + 1U );
    sc1.updateTime();
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>( harness_getBoard()->harness_getTimePassedFromBoot() ), sc1.last_millis   );
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(                                                   1 ), sc1.millis_passed );
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(                                                   0 ), sc1.seconds_passed);


    // overflow management
    sc1.last_millis      = UINT32_MAX;
    harness_getBoard()->harness_setTimePassedFromBoot( 0U );

    sc1.millis_passed  = 0U;
    sc1.seconds_passed = 0U;

    sc1.updateTime();
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>( harness_getBoard()->harness_getTimePassedFromBoot() ), sc1.last_millis   );
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(                    1 ), sc1.millis_passed );
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(                    0 ), sc1.seconds_passed);
}

void LivelloSerbatoioTests::test_ConsumptionData_updateConsumption ( void ) {
    uut::ConsumptionData sc1;
    sc1.begin();

    // filling
    sc1.updateConsumption(0U);
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(0), sc1.consumption);
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(0), sc1.last_liters);

    sc1.updateConsumption(1U);
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(0), sc1.consumption);
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(0), sc1.last_liters);

    sc1.updateConsumption(uut::ConsumptionData::LITERS_THRESHOLD - 1U);
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(0), sc1.consumption);
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(0), sc1.last_liters);

    sc1.updateConsumption(uut::ConsumptionData::LITERS_THRESHOLD);
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(0), sc1.consumption);
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(0), sc1.last_liters);

    sc1.updateConsumption(uut::ConsumptionData::LITERS_THRESHOLD + 1U);
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(0)                                             , sc1.consumption);
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(uut::ConsumptionData::LITERS_THRESHOLD + 1U), sc1.last_liters);

    sc1.updateConsumption(1000U);
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(0)    , sc1.consumption);
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(1000U), sc1.last_liters);

    // consuming
    sc1.updateConsumption(1000U - 1U);
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(0)    , sc1.consumption);
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(1000U), sc1.last_liters);

    sc1.updateConsumption(1000U - uut::ConsumptionData::LITERS_THRESHOLD + 1U);
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(0)    , sc1.consumption);
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(1000U), sc1.last_liters);

    sc1.updateConsumption(1000U - uut::ConsumptionData::LITERS_THRESHOLD);
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(0)    , sc1.consumption);
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(1000U), sc1.last_liters);

    sc1.updateConsumption(1000U - uut::ConsumptionData::LITERS_THRESHOLD - 1U);
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(uut::ConsumptionData::LITERS_THRESHOLD + 1U)        , sc1.consumption);
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(1000U - uut::ConsumptionData::LITERS_THRESHOLD - 1U), sc1.last_liters);
}

void LivelloSerbatoioTests::test_ConsumptionData_getConsumption1h ( void ) {
    uut::ConsumptionData sc1;
    sc1.begin();

    sc1.index = 0U;

    for(uint32_t i = 0U; i < uut::ConsumptionData::STAT_SIZE; i++) {
        sc1.consumption_samples[i] = 1U;
    }

    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>( 1U ) , sc1.getConsumption1h());
}

void LivelloSerbatoioTests::test_ConsumptionData_getConsumption12h ( void ) {
    uut::ConsumptionData sc1;
    sc1.begin();

    sc1.index = 0U;

    for(uint32_t i = 0U; i < uut::ConsumptionData::STAT_SIZE; i++) {
        sc1.consumption_samples[i] = 1U;
    }

    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>( 12U ) , sc1.getConsumption12h());
}

void LivelloSerbatoioTests::test_ConsumptionData_getConsumption1d ( void ) {
    uut::ConsumptionData sc1;
    sc1.begin();

    sc1.index = 0U;

    for(uint32_t i = 0U; i < uut::ConsumptionData::STAT_SIZE; i++) {
        sc1.consumption_samples[i] = 1U;
    }

    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>( 24U ) , sc1.getConsumption1d());
}

void LivelloSerbatoioTests::test_ConsumptionData_getConsumption3d ( void ) {
    uut::ConsumptionData sc1;
    sc1.begin();

    sc1.index = 0U;

    for(uint32_t i = 0U; i < uut::ConsumptionData::STAT_SIZE; i++) {
        sc1.consumption_samples[i] = 1U;
    }

    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>( 24U * 3U ) , sc1.getConsumption3d());
}

void LivelloSerbatoioTests::test_ConsumptionData_sumSamples ( void ) {
    uut::ConsumptionData sc1;
    sc1.begin();

    sc1.index = 3U;

    sc1.consumption_samples[0] = 1U;
    sc1.consumption_samples[1] = 10U;
    sc1.consumption_samples[2] = 100U;
    sc1.consumption_samples[3] = 1000U;

    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>( 0U    ) , sc1.sumSamples(0));
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>( 1000U ) , sc1.sumSamples(1));
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>( 1100U ) , sc1.sumSamples(2));
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>( 1110U ) , sc1.sumSamples(3));
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>( 1111U ) , sc1.sumSamples(4));

    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>( 1111U ) , sc1.sumSamples(uut::ConsumptionData::STAT_SIZE - 1U));
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>( 9999U ) , sc1.sumSamples(uut::ConsumptionData::STAT_SIZE));

}

void LivelloSerbatoioTests::test_ConsumptionData_updateBuffer ( void ) {
    uut::ConsumptionData sc1;
    sc1.begin();

    sc1.index = 1U;

    sc1.consumption = 1U;
    sc1.updateBuffer();
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>( 1U ) , sc1.consumption_samples[sc1.index]);

    sc1.consumption = 1U;
    sc1.updateBuffer();
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>( 2U ) , sc1.consumption_samples[sc1.index]);

    sc1.consumption = 1U;
    sc1.updateBuffer();
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>( 3U ) , sc1.consumption_samples[sc1.index]);

}

void LivelloSerbatoioTests::test_ConsumptionData_updateIndex ( void ) {
    uut::ConsumptionData sc1;
    sc1.begin();

    for(uint32_t i = 0U; i < uut::ConsumptionData::STAT_SIZE; i++) {
        sc1.consumption_samples[i] = 1U;
    }

    sc1.index = 0U;
    sc1.updateIndex();

    for(uint32_t i = 1U; i < uut::ConsumptionData::STAT_SIZE; i++) {
        CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>( i )  , sc1.index);
        CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>( 0U ) , sc1.consumption_samples[sc1.index]);

        sc1.updateIndex();
    }

    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>( 0 ) , sc1.index);
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>( 0U ) , sc1.consumption_samples[sc1.index]);
}

