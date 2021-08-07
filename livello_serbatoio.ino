//!
//! \brief Software for water tank level monitoring.
//!
//! \details This Software measures and monitors the water level in a tank using an ultrasonic distance sensor.
//!          It shows essential information (such as remaining liters and fill percentage) in a 16x2 LCD and 
//!          warns about low water level using a LED.
//!
//!          Pinout and links:
//!          - LCD (1602A):
//!            - Vss  : GND
//!            - Vdd  : +5V
//!            - Vo   : OUT Potentiometer (for contrast regulation)
//!            - RS   : DIGITAL 12
//!            - RW   : GND (Write)
//!            - E    : DIGITAL 11
//! 
//!            - DB4, DB5, DB6, DB7
//!            -   4 ,  5 ,  6 ,  7  (DIGITAL)
//!
//!
//! \author Giovanni Rossi
//!
//! \copyright This project is released under the GNU General Public License v3.0.
//!

// Includes
#ifndef UNIT_TEST
#include <LiquidCrystal.h>
#endif

// Defines
#define VERSION "v0.5"

#define CONF_DEBUG   1
#define CONF_RELEASE 0

#define SENSOR_HCSR04   0
#define SENSOR_JSNSR04T 1

#define roundfvalue(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))

// Configuration
#ifndef UNIT_TEST
# define DEBUG  CONF_RELEASE
# define SENSOR SENSOR_HCSR04
#endif

// Types
typedef double float64_t;

// Constants

// Median filter
const int16_t     FILTER_SEED           = 0                                   ;
const int16_t     FILTER_SIZE           = 5                                   ;

// Pins
const int16_t     ECHO_DPIN             = 12                                  ;
const int16_t     TRIG_DPIN             = 11                                  ;
const int16_t     LED_CAPACITY_DPIN     = 13                                  ;
const int16_t     LCD_BUTTON_DPIN       = 2                                   ;
const int16_t     LCD_LIGHT_DPIN        = 9                                   ;

// Tank and volume
const uint16_t    TANK_NUMBER           = 2U                                  ;
const float64_t   TANK_RADIUS_CM        = 35.0                                ; // cm
const float64_t   TANK_HEIGHT_CM        = 156.0                               ; // cm
const float64_t   SENSOR_DISTANCE_CM    = 26.0                                ; // cm
const float64_t   WATER_MAX_HEIGHT_CM   = TANK_HEIGHT_CM - SENSOR_DISTANCE_CM ; // cm
const float64_t   LOW_LEVEL_THRESHOLD   = 30.0                                ; // %
const float64_t   EMPTY_LEVEL_THRESHOLD = 10.0                                ; // %

// Conversion units
const float64_t   CM3_PER_LITER         = 1000.0                              ; // 1 l = 1000 cm^3

// Timers
const int32_t     MEASURE_LF_INTERVAL   = 10 * 1000                           ; // ms
const int32_t     MEASURE_HF_INTERVAL   =  1 * 1000                           ; // ms
const int32_t     LCD_ON_TIME           = 30 * 1000                           ; // ms
const int32_t     SLEEP_TIME            =  100                                ; // ms
const int32_t     SHOW_STAT_TIME        = 5000                                ; // ms
const int32_t     SHOW_VERSION_TIME     = 3000                                ; // ms
const int32_t     LED_CONTROL_TIME      = 1000                                ; // ms
const int32_t     READ_DISTANCE_TIME    = 1000                                ; // ms
const int32_t     BTN_INTERVAL_1_TIME   =  200                                ; // ms
const int32_t     BTN_INTERVAL_2_TIME   = 2000                                ; // ms
const int32_t     BTN_INTERVAL_3_TIME   = 3000                                ; // ms
const int32_t     BTN_INTERVAL_4_TIME   = 4000                                ; // ms
const int32_t     BTN_INTERVAL_5_TIME   = 5000                                ; // ms

// Sensor parameters
#if SENSOR == SENSOR_JSNSR04T

const float64_t   SENSOR_CALIBRATION    = 1.7                                 ; // cm
const float64_t   SENSOR_MIN_RANGE      = 20.0                                ; // cm
const float64_t   SENSOR_MAX_RANGE      = 600.0                               ; // cm
const float64_t   SENSOR_LSB            = 58.0                                ; // usec / cm

const uint32_t    SENSOR_RESPONSE_TIMEOUT_US = 500000UL                       ; // 500 ms
const uint32_t    SENSOR_NO_OBSTACLE_US      = 38000UL                        ; // 38 ms

#elif SENSOR == SENSOR_HCSR04

const float64_t   SENSOR_CALIBRATION    = 0.0                                 ; // cm
const float64_t   SENSOR_MIN_RANGE      = 2.0                                 ; // cm
const float64_t   SENSOR_MAX_RANGE      = 400.0                               ; // cm
const float64_t   SENSOR_LSB            = 58.0                                ; // usec / cm

const uint32_t    SENSOR_RESPONSE_TIMEOUT_US = 500000UL                       ; // 500 ms
const uint32_t    SENSOR_NO_OBSTACLE_US      = 38000UL                        ; // 38 ms

#else

const float64_t   SENSOR_CALIBRATION    = 0.0                                 ; // cm
const float64_t   SENSOR_MIN_RANGE      = 0.0                                 ; // cm
const float64_t   SENSOR_MAX_RANGE      = 0.0                                 ; // cm
const float64_t   SENSOR_LSB            = 1.0                                 ; // usec / cm

const uint32_t    SENSOR_RESPONSE_TIMEOUT_US = 500000UL                       ; // 500 ms
const uint32_t    SENSOR_NO_OBSTACLE_US      = 38000UL                        ; // 38 ms

#endif

// LCD special characters
const byte        UP_ARROW_CHAR         = 0                                   ;
const byte        PROGRESS_CHAR         = 1                                   ;

      byte        LCD_UP_ARROW[]        = { 4, 14, 21, 4, 4, 0, 0, 0 }                      ;
      byte        LCD_PROGRESS[]        = { 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F };

// LCD pins
const int16_t     RS                    = 3                                   ;
const int16_t     E                     = 4                                   ;
const int16_t     DB4                   = 5                                   ;
const int16_t     DB5                   = 6                                   ;
const int16_t     DB6                   = 7                                   ;
const int16_t     DB7                   = 8                                   ;

// Error codes
const int32_t     ERR_OK                = 0                                   ; //<! All ok
const int32_t     ERR_SENS              = 1                                   ; //<! Sensor reading error
const int32_t     ERR_RANGE             = 2                                   ; //<! Sensor reading out of range
const int32_t     ERR_STAT              = 3                                   ; //<! Consumption calculation error

// Global variables

// Status flags
volatile bool        led_on                    ;
volatile bool        led_status                ;
volatile bool        must_update_lcd           ;
volatile bool        was_error                 ;
volatile bool        in_debug                  ;
volatile bool        first_measure_done        ;
volatile byte        btn_status                ;
volatile byte        last_btn_status           ;
volatile int32_t     err_code                  ;

// Timestamps
volatile int32_t     timestamp_lcd_on          ;
volatile int32_t     timestamp_measurement     ;
volatile int32_t     timestamp_last_led_ctrl   ;
volatile int32_t     timestamp_last_read_dist  ;
volatile int32_t     timestamp_last_filling    ;
volatile int32_t     timestamp_btn_press       ;

// Timings
uint16_t             measure_interval          ;

// Computed tank(s) parameters
float64_t            tank_capacity             ;
float64_t            maximum_capacity          ;

// Current reads
uint16_t             number_measures_done      ;   
float64_t            percentage                ;
volatile float64_t   distance                  ;
volatile float64_t   previous_liters           ;

// Classes

//!
//! \brief Median filter manager.
//!
//! \details It uses a buffer to store data and compute the median value.
//!
class MedianFilter {
public:

  //!
  //! \brief Default constructor.
  //!
  //! \details Creates and instance of this class.
  //! 
  //! \return An instance of this class to be initialized with #begin method.
  //!
  MedianFilter ( void ) {
  }

  //!
  //! \brief Initializes this instance.
  //! 
  //! \param[in] seed Buffer start value.
  //! 
  void begin ( const int16_t seed ) {
    // set number of samples in sliding median filter window - usually odd
    medFilterWin    = FILTER_SIZE;
    
    // mid point of window
    medDataPointer  = FILTER_SIZE >> 1;

    // set oldest data point location in data array
    oldestDataPoint = medDataPointer;                         

    // initialize the arrays
    for (byte i = 0; i < medFilterWin; i++) {
      // start map with straight run
      sizeMap    [i] = i;
      locationMap[i] = i;
      
      // populate with seed value
      data       [i] = seed; 
    }
  }

  //!
  //! \brief Insert a value in the filter.
  //!
  //! \details Insert the given value in the filter and than computes the median among inserted values.
  //! 
  //! \param[in] value The value to be inserted.
  //!
  //! \return The median element.
  //!
  int16_t in(int16_t value) {
    // sort sizeMap
    // small vaues on the left (-)
    // larger values on the right (+)
    
    boolean dataMoved = false;

    // adjusted for zero indexed array
    const byte rightEdge = medFilterWin - 1;

    // store new data in location of oldest data in ring buffer
    data[oldestDataPoint] = value;
    
    // SORT LEFT (-) <======(n) (+)
    if (locationMap[oldestDataPoint] > 0) {
      // don't check left neighbours if at the extreme left
      for (byte i = locationMap[oldestDataPoint]; i > 0; i--) {
        //index through left adjacent data

        // neighbour location
        byte n = i - 1;

        if (data[oldestDataPoint] < data[sizeMap[n]]) {
          // find insertion point, move old data into position

          // move existing data right so the new data can go left
          sizeMap[i] = sizeMap[n];
          locationMap[sizeMap[n]]++;

          // assign new data to neighbor position
          sizeMap[n] = oldestDataPoint;
          locationMap[oldestDataPoint]--;
  
          dataMoved = true;
        } else {
          // stop checking once a smaller value is found on the left 
          break;
        }
      }
    }
    
    // SORT RIGHT (-) (n)======> (+)
    if ( ( dataMoved == false ) && ( locationMap[oldestDataPoint] < rightEdge ) ) {
      // don't check right if at right border, or the data has already moved
      
      for (byte i = locationMap[oldestDataPoint]; i < rightEdge; i++) {
        //index through left adjacent data

        // neighbour location
        uint16_t n = i + 1;  
    
        if (data[oldestDataPoint] > data[sizeMap[n]]) {
          // find insertion point, move old data into position

          // move existing data left so the new data can go right
          sizeMap[i] = sizeMap[n];
          locationMap[sizeMap[n]]--;

          // assign new data to neighbor position
          sizeMap[n] = oldestDataPoint;
          locationMap[oldestDataPoint]++;
        } else {
          // stop checking once a smaller value is found on the right 
          break;
        }
      }
    }

    // increment and wrap
    oldestDataPoint++;
    if (oldestDataPoint == medFilterWin) {
      oldestDataPoint = 0;
    }
    
    return data[sizeMap[medDataPointer]];
  }

  //!
  //! \brief Gets the median values.
  //!
  //! \details Accesses the buffer position where the median value is stored in O(1).
  //! 
  //! \return The median.
  //!
  int16_t out(void) {
    return data[sizeMap[medDataPointer]];
  }

private:
  byte    medFilterWin;                //<! Samples number in the sliding window of the filter - usually an odd value.
  byte    medDataPointer;              //<! Central point of the filter sliding window.
  int16_t data        [FILTER_SIZE];   //<! Array of data ordered by age (Circular buffer).
  byte    sizeMap     [FILTER_SIZE];   //<! Array used to store data locations ordered by size.
  byte    locationMap [FILTER_SIZE];   //<! Array used to store data locations in the map.
  byte    oldestDataPoint;             //<! Position of the oldest value in the circular buffer.
};


//!
//! @brief Manages the consumption statistics.
//!
class ConsumptionData {
public:

  //!
  //! \brief Default constructor.
  //!
  //! \details Creates and instance of this class.
  //! 
  //! \return An instance of this class to be initialized with #begin method.
  //!
  ConsumptionData(void) {
  }

  //!
  //! \brief Initializes this instance.
  //! 
  void begin ( void ) {
    last_millis = millis();

    for ( uint16_t i = 0U; i < STAT_SIZE; i++ ) {
      consumption_samples[i] = 0U;
    }
    
    index           = 0U;
    last_liters     = 0U;
    consumption     = 0U;
    millis_passed   = 0U;
    seconds_passed  = 0U;
  }

  //!
  //! \brief Updates the time data.
  //!
  //! \details Computes seconds, minutes, hours and days passed from power on.
  //!          Every hours it updates the current consumption (an estimation).
  //! 
  //! \attention It must be invoked it the #loop function.
  //!
  void updateTime(void) {

    // get current milliseconds passed from boot
    uint32_t now         = millis();
    uint32_t millis_diff = 0U;

    // compute delta ms with respect to previous invocation
    millis_diff = (uint32_t)(now - last_millis);

    // update ms of previous invocation
    last_millis = now;

    // increase count of milliseconds passed.
    millis_passed += millis_diff;

    if(millis_passed >= ONE_SECOND_MS) {
      // after 1 second

      // update buffer
      updateBuffer();

      uint32_t n_sec = (uint32_t)(millis_passed / ONE_SECOND_MS);

      // increase seconds counter
      seconds_passed += n_sec;
      millis_passed   = millis_passed - (n_sec * ONE_SECOND_MS); 
    }

    if(seconds_passed >= ONE_HOUR_S) {
      // after 1 hour

      // update index and pass to next slot
      updateIndex();

      // reset seconds counter
      seconds_passed = seconds_passed - ONE_HOUR_S;
    }
  }

  //!
  //! \brief Updates the consumption if necessary.
  //!
  //! \details Computes the current consumption and updates buffer.
  //! 
  //! \param current_liters[in] The water volume in the tank(s).
  //! 
  //! \attention It shall be called in #loop function when new measurement updates are available.
  //!
  void updateConsumption(uint32_t current_liters) {
    if(current_liters > last_liters) {
      // filling
      uint32_t temp_refill = (current_liters - last_liters);
      if(temp_refill > LITERS_THRESHOLD) {
        // hysteresis (sensor readings are often noisy)
        last_liters = current_liters; 
      }
    } else { 
      // emptying
      uint32_t temp_consumption = (last_liters - current_liters);
      if(temp_consumption > LITERS_THRESHOLD) {
        // hysteresis (sensor readings are often noisy)
        consumption += temp_consumption; 
        last_liters = current_liters; 
      }
    }
  }

  //!
  //! \brief Gives the total consumption in the last hour.
  //!
  //! \details Sums the stored data.
  //! 
  //! \return The water consumption in liters.
  //!
  uint32_t getConsumption1h(void) {
    return sumSamples(1U);
  }

  //!
  //! \brief Gives the total consumption in the last 12 hours.
  //!
  //! \details Sums the stored data.
  //! 
  //! \return The water consumption in liters.
  //!
  uint32_t getConsumption12h(void) {
    return sumSamples(12U);
  }

  //!
  //! \brief Gives the total consumption in the last day.
  //!
  //! \details Sums the stored data.
  //! 
  //! \return The water consumption in liters.
  //!
  uint32_t getConsumption1d(void) {
    return sumSamples(24U);
  }


  //!
  //! \brief Gives the total consumption in the last 3 days.
  //!
  //! \details Sums the stored data.
  //! 
  //! \return The water consumption in liters.
  //!
  uint32_t getConsumption3d(void) {
    return sumSamples(24U * 3U);
  }
  
private:

  //!
  //! \brief Sums the consumption samples.
  //! 
  //! \param back_time[in] The hours passed.
  //! 
  //! \return 9999 if back_time is >= STAT_SIZE (3d) otherwise the total consumption in liters.
  //!
  //! \attention Assumption: The tank(s) contain(s) 1000L maximum and there no more than 2 fillings per day.
  //!
  uint32_t sumSamples(uint32_t back_time) {
    uint32_t sum = 0U;
    
    if(back_time >= STAT_SIZE) {      
      // impossible value in 3 days (see assumption above).
      sum = 9999U;
    } else {
      int16_t i = index;
      uint32_t time_index = back_time;

      // go back to #back_time hours
      while(time_index != 0) {

        // sum the consumption
        sum += consumption_samples[i];
        i--;

        // restart from beginning if buffer is terminated
        if(i < 0) {
          i = STAT_SIZE - 1;
        }

        time_index--;
      }
    }

    return sum;
  }

  //!
  //! \brief Updates the consumptions buffer.
  //!
  //! \details Updates the consumption accumulated in the last hour.
  //!
  void updateBuffer(void) {
    consumption_samples[index] += consumption;
    consumption = 0;
  }

  //!
  //! \brief Updates the buffer index.
  //!
  //! \details Updates the buffer index and reset the older values to 0.
  //!
  void updateIndex(void) {
    index ++;
    if(index >= STAT_SIZE) {
      index = 0;
    }
    
    //reset old data
    consumption_samples[index] = 0U;
  }

  static const uint16_t STAT_SIZE         = (24U * 3U) + 1U; //<! The sample dimension (3 days).
  static const uint32_t LITERS_THRESHOLD  = 15U;             //<! Minimum consumption threshold to be inserted as sample.

  static const uint32_t ONE_SECOND_MS     = 1000U;           //<! 1 second constant.
  static const uint32_t ONE_HOUR_S        = (60U * 60U);     //<! 1 hour constant.
  
  uint32_t index                          ;        //<! Buffer current index.
  uint32_t consumption_samples[STAT_SIZE] ;        //<! The buffer with samples. 
  volatile uint32_t last_liters           ;        //<! The last known volume of water in the tank (in liters).
  volatile uint32_t consumption           ;        //<! The last hour water consumption.

  volatile uint32_t last_millis           ;        //<! Last timestamp (value from power-on).
  volatile uint32_t millis_passed         ;        //<! Passed milliseconds from last invocation of #updateTime.
  volatile uint32_t seconds_passed        ;        //<! Passed seconds from last invocation of #updateTime.
};

// Instances of the classes

//Format (RS, E, DB4, DB5, DB6, DB7)
LiquidCrystal lcd(RS, E, DB4, DB5, DB6, DB7);

MedianFilter filter;

ConsumptionData stats;

// SW Functions

//!
//! \brief Initializes all variables.
//!
inline void initialize ( void ) {
  led_on                   = false               ;
  led_status               = false               ;
  must_update_lcd          = false               ;
  first_measure_done       = false               ;
  was_error                = false               ;
  err_code                 = ERR_OK              ;
  btn_status               = LOW                 ;
  last_btn_status          = LOW                 ;
  
  tank_capacity            = 0.0                 ; // L
  maximum_capacity         = 0.0                 ; // L

  number_measures_done     = 0                   ;   
  distance                 = 0.0                 ; // cm
  previous_liters          = 0.0                 ; // L
  percentage               = 100.0               ; // %
  
  measure_interval         = MEASURE_HF_INTERVAL ; // ms

  timestamp_lcd_on         = 0                   ; // ms
  timestamp_measurement    = 0                   ; // ms
  timestamp_last_filling   = 0                   ; // ms
  timestamp_last_read_dist = 0                   ; // ms 
  timestamp_btn_press      = 0                   ; // ms
  timestamp_last_led_ctrl  = 0                   ; // ms
}

//!
//! \brief Computes the volume of water in the tank.
//!
//! \details See below for details on computation.
//!           
//!            sensor
//!       +----| W |----+ ---  TANK_HEIGHT_CM
//!       |      |      |    } 
//!       |  (distance) |    > SENSOR_DISTANCE_CM
//!       |      |      |    }
//!    ----------------------> WATER_MAX_HEIGHT_CM
//!       |      |      |
//!       |      V      |   
//!    ----------------------> current water level
//!       |             |      
//!       |             |
//!    ----------------------> level 0
//!       
//!   real distance = distance - SENSOR_DISTANCE_CM
//!
//! \param read_distance[in] The water distance from sensor.
//!
//! \return The volume in liters.
//!
inline float64_t compute_liters ( float64_t read_distance ) {

  // sanitize data below 0
  if ( read_distance < 0.0 ) {
    read_distance = 0.0;
  }

  // sanitize data over tank height
  if ( read_distance > TANK_HEIGHT_CM ) {
    read_distance = TANK_HEIGHT_CM;
  }

  // remove sensor distance from water
  read_distance -= SENSOR_DISTANCE_CM;

  // compute the volume
  const float64_t volume = TANK_NUMBER * ( ( TANK_RADIUS_CM * TANK_RADIUS_CM * PI * ( WATER_MAX_HEIGHT_CM - read_distance ) ) / CM3_PER_LITER );
  
  return volume;
}

//!
//! \brief Computes the percentage of filling of the tank.
//! 
//! \details It uses the most recent reading and the maximum capacity computed in #setup.
//!
//! \param read_liters[in] The volume of water read.
//! 
//! \return The filling percentage in range [0, 100].
//!
inline float64_t compute_percentage ( float64_t read_liters ) {
  // sanitize below 0 values.
  if ( read_liters < 0.0 ) {
    read_liters = 0.0;
  }

  // sanitize values beyond maximum capacity.
  if ( read_liters > maximum_capacity ) {
    read_liters = maximum_capacity;
  }

  // compute the percentage
  const float64_t filling_percentage = (read_liters / maximum_capacity) * 100.0;
  
  return filling_percentage;
}

//!
//! \brief Turns on the LCD backlight.
//!
//! \details Raises the backlight digital pin and activates the timer to turn off the LCD backlight.
//!
inline void turn_on_lcd_light ( void ) {
  // activate backlight
  digitalWrite(LCD_LIGHT_DPIN, HIGH);

  // get timestamp for future deactivation
  timestamp_lcd_on = millis();
#if DEBUG
  Serial.print("LCD on -> t:");
  Serial.println(timestamp_lcd_on);
#endif
}

//!
//! \brief Turns off the LCD backlight.
//!
//! \details Lowers the backlight digital pin.
//!
inline void turn_off_lcd_light ( void ) {

  // deactivate backlight
  digitalWrite(LCD_LIGHT_DPIN, LOW);
#if DEBUG
  Serial.println("LCD off");
#endif
}

//!
//! \brief Prints an error message to LCD if not in debug mode.
//!
inline void print_error ( void ) {

  if ( in_debug == false ) {
    lcd.clear();
    lcd.setCursor(5, 1);
    lcd.print("Errore");
  }

  was_error = true;
}

//!
//! \brief Shows the current error code to LCD (only in debug mode).
//!
//! \details Implemented layout:
//!          |----------------|
//!          |XXXXXXXXXXX e:XX|
//!          |XXXXXXXXXXXXXXXX|
//!          |----------------|
//!
inline void show_err_code_debug ( void ) {
  if ( ( in_debug == true ) && ( last_btn_status != HIGH ) ) {
    lcd.setCursor(12,0);
    lcd.print("e: ");
    lcd.setCursor(14,0);
    if ( was_error == true ) {
      lcd.print((int32_t) err_code);
    } else {
      lcd.print((int32_t) 0);
    }
  }
}


//!
//! \brief Updates the LCD.
//!
//! \details Used only in debug mode.
//!  
//! \param distance_to_print[in] The distance from the water in centimeters.
//! \param distance_comp[in] The distance from water (calibrated value in centimeters).
//!
inline void update_lcd_debug ( float64_t distance_to_print, float64_t distance_comp ) {
  if ( must_update_lcd == true ) {   
    must_update_lcd = false;
  
    lcd.clear();

    // show distance
    lcd.setCursor(0, 0);
    lcd.print("d:");
    lcd.print((int32_t) distance_to_print);
    lcd.print(" cm");
    
    lcd.setCursor(0,1);
    lcd.print("dc:");
    lcd.print((int32_t) distance_comp);

    // show volume
    lcd.print(" L:");
    lcd.print((int32_t) compute_liters(distance_comp));
  }
}

/**
 * @brief Esegue una misurazione di livello dell'acqua.
 * @details Attiva il sensore ad ultrasuoni ed applica eventuali compensazioni necessarie.
 * 
 * @return La misura in cm oppure -1.0 in caso di errore di misura.
 */
inline float64_t measure_level(void) {
  //activate distance measurement
  digitalWrite(TRIG_DPIN, HIGH);
  delayMicroseconds(15); //>= 10 usec da datasheet
  digitalWrite(TRIG_DPIN, LOW);
  
  uint32_t time = pulseIn(ECHO_DPIN, HIGH, SENSOR_RESPONSE_TIMEOUT_US);
  if ((time == 0U) || (time >= SENSOR_NO_OBSTACLE_US)) {
    err_code = ERR_SENS;
    was_error = true;
    show_err_code_debug();
    return -1.0; //Error
  }
  
  float64_t distance_read = (float64_t) time / (SENSOR_LSB);  //convert to cm
  
  //compensate the constant error introduced by new sensor
  float64_t dist_compensated = distance_read + SENSOR_CALIBRATION;
  //

  if ( ( in_debug == true ) && ( last_btn_status == LOW ) ) {
    must_update_lcd = true;
    update_lcd_debug(distance_read, dist_compensated);
  }

  //check ranges
  if ( ( dist_compensated < SENSOR_MIN_RANGE ) || ( dist_compensated > SENSOR_MAX_RANGE ) ) {
    err_code = ERR_RANGE;
    was_error = true;
    show_err_code_debug();
    return -1.0;
  }
  //
  
#if DEBUG
  Serial.print("dist: ");
  Serial.print(distance_read);
  Serial.print(" + calib: ");
  Serial.print(dist_compensated);
  Serial.print(" round: ");
  Serial.println(roundfvalue(dist_compensated));
#endif
  
  timestamp_measurement = millis();


  if(first_measure_done == false) {
    number_measures_done++;
  }

  if(number_measures_done > (FILTER_SIZE / 2)) {
    first_measure_done = true;
    if ( in_debug == true ) {
      measure_interval = MEASURE_HF_INTERVAL; //ms
    } else {
      measure_interval = MEASURE_LF_INTERVAL; //ms
    }
  }
  
  was_error = false;
  err_code = ERR_OK;
  show_err_code_debug();
    
  return dist_compensated;
}

/**
 * @brief Controlla lo stato del LED in base al livello dell'acqua.
 * @details Se il livello dell'acqua è al di sotto di LOW_LEVEL_THRESHOLD ma al di sopra di EMPTY_LEVEL_THRESHOLD
 *          lo fa accendere, se è al di sotto di EMPTY_LEVEL_THRESHOLD lo fa lampeggiare, altrimenti lo lascia spento.
 *          
 * @param percentage La percentuale di riempimento del serbatoio (in [0, 100]).
 */
inline void control_led(float64_t percentage) {
  if ( percentage < 0.0 ) {
    led_status = false;
    led_on     = false;
  } else if (percentage <= EMPTY_LEVEL_THRESHOLD) {
    led_status = true;
    led_on     = !led_on;
  } else if (percentage <= LOW_LEVEL_THRESHOLD) {
    led_status = true;
    led_on     = true;
  } else {
    led_status = false;
    led_on     = false;
  }
  digitalWrite(LED_CAPACITY_DPIN, led_on);
}

/**
 * @brief Pulisce i dati in input costringendoli all'interno di un range valido.
 * @details Se data è sotto la min_threshold lo imposta a min_val, 
 *          se data è sopra la max_threshold lo imposta a max_val, 
 *          altrimenti lo lascia inalterato.
 *        
 * @param data Il dato da ripulire.
 * @param min_val Il valore minimo ammissibile.
 * @param max_val Il valore massimo ammissibile.
 * @param min_threshold Il valore di soglia minimo ammissibile.
 * @param max_threshold Il valore di soglia massimo ammissibile.
 * 
 * @return Il dato nel range [min_val, max_val].
 */
inline float64_t sanitize_data(float64_t data, float64_t min_val, float64_t max_val, float64_t min_threshold, float64_t max_threshold) {
  float64_t sanitized_data = min_val;
  
  if (data < min_threshold) {
    sanitized_data = min_val;
  } else if (data > max_threshold) {
    sanitized_data = max_val;
  } else {
    sanitized_data = data;
  }
  
  return sanitized_data;
}


/**
 * @brief Aggiorna l'LCD con i parametri attuali se necessario.
 * @details L'aggiornamento segue lo schema:
 * 
 *  |----------------| 
 *  | xxxx L   yyy % |
 *  | ############## |
 *  |----------------|
 *  
 *  es.
 *  |----------------| 
 *  |  876 L ^  87 % |
 *  | ############   |
 *  |----------------|
 *  
 *  |----------------| 
 *  | xxxx L   yyy % |
 *  | ##  Riserva    |
 *  |----------------|
 *  
 *  |----------------| 
 *  | xxxx L   yyy % |
 *  |     Vuoto      |
 *  |----------------|
 *  
 *  <xxxx> = [0, 9999] L
 *  <yyy> = [0, 100] %
 *  # X 16 => 0 = 0%, 16 = 100%
 *  
 * @param percentage La percentuale di riempimento del serbatoio.
 * @param liters I litri di acqua rimanenti.
 */
inline void update_lcd(float64_t percentage, float64_t liters) {
  if( ( percentage < 0.0 ) || ( percentage > 100.0 ) || ( liters < 0.0 ) || ( liters > 9999.9 ) ) {
    must_update_lcd = false;
  }
  
  if(must_update_lcd == true) {
    must_update_lcd = false;
    
    lcd.clear();
    lcd.setCursor(6, 0);
    lcd.print("L");
    lcd.setCursor(14, 0);
    lcd.print("%");
  
    int16_t index = 4;
    int16_t l = (int16_t) liters;
    if (l >= 10) {
      index--;
    }
    if (l >= 100) {
      index--;
    }
    if (l >= 1000) {
      index--;
    }

    lcd.setCursor(index, 0);
    lcd.print(l);
  
    percentage = sanitize_data(percentage, 0.0, 100.0, 0.5, 99.5);
    
    int16_t p = (int16_t) percentage;
    index = 12;
    if(p >= 10) {
      index --;
    }

    if(p >= 100) {
      index --;
    }
    
    lcd.setCursor(index, 0);
    lcd.print(p);
  
    lcd.setCursor(1, 1);
    int16_t howmany = (14 * p) / 100;
    for (int16_t i = 0; i < howmany; i++) {
      lcd.write(PROGRESS_CHAR);
    }
  
    if (howmany == 0) {
      lcd.setCursor(5, 1);
      lcd.print("Vuoto");
    } else if (howmany <= 2) {
      lcd.setCursor(5, 1);
      lcd.print("Riserva");
    }
  }
}

/**
 * @brief Esegue un autotest dei controlli all'avvio.
 * @details Permette di controllare stato dei LED del bottone e dell'LCD.
 * 
 */
inline void autotest(void) {
  digitalWrite(LCD_LIGHT_DPIN, LOW);
  digitalWrite(LED_CAPACITY_DPIN, LOW);
  lcd.clear();

  delay(1000);
  
  digitalWrite(LCD_LIGHT_DPIN, HIGH);
  digitalWrite(LED_CAPACITY_DPIN, HIGH);

  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  for (uint16_t i = 0; i < 16; i++) {
      lcd.write(PROGRESS_CHAR);
  }
  lcd.setCursor(0, 1);
  for (uint16_t i = 0; i < 16; i++) {
      lcd.write(PROGRESS_CHAR);
  }

  delay(3000);

  digitalWrite(LED_CAPACITY_DPIN, LOW);
  digitalWrite(LCD_LIGHT_DPIN, LOW);
  lcd.clear();

  delay(1000);
}

/**
 * @brief Stampa su LCD l'informazione passata.
 * @details Segue il formato:
 *  |----------------| 
 *  |<      str     >|
 *  |Tot. <stat>   L |
 *  |----------------|
 *  
 * @param str Il titolo della statistica.
 * @param stat Il valore della statistica.
 * 
 */
inline void print_stat(String str, uint32_t stat) {
  if (stat == 9999U) {
    was_error = true;
    err_code = ERR_STAT;
  } else {
    was_error = false;
    err_code = ERR_OK;
  }
  show_err_code_debug();
  
  lcd.setCursor(0, 0);
  lcd.print(str);
  
  lcd.setCursor(0, 1);
  lcd.print("Tot. ");
  lcd.print(stat);
  //lcd.print(9999);
  
  lcd.setCursor(14, 1);
  lcd.print("L");
  
  delay(SHOW_STAT_TIME);
  lcd.clear(); 
}

/**
 * @brief Stampa su LCD la versione del SW.
 * @details Segue il formato:
 *  |----------------| 
 *  | Versione:      |
 *  | <num_version>  |
 *  |----------------|
 * 
 */
inline void show_version(void) {
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print(" Versione:");

  lcd.setCursor(0, 1);
  lcd.print(" ");
  lcd.print(VERSION);

  delay(SHOW_VERSION_TIME);
  
  lcd.clear();
}

/**
 * @brief Stampa su LCD le statistiche raccolte.
 * @details Stampa a turno i consumi totali da:
 *          - 1 ora;
 *          - 12 ore;
 *          - 1 giorno;
 *          - 3 giorni.
 *          Infine stampa la versione del SW.
 *         
 */
inline void show_stats(void) {
  lcd.clear();
  
  //stampa 1h
  print_stat("Consumo 1 ora", stats.getConsumption1h());
  
  //stampa 12h
  print_stat("Consumo 12 ore", stats.getConsumption12h());
  
  //stampa 1d
  print_stat("Consumo 1 giorno", stats.getConsumption1d());

  //stampa 3d
  print_stat("Consumo 3 giorni", stats.getConsumption3d());
}

inline void enter_debug(void) {
  in_debug = true;

  lcd.clear();
  lcd.setCursor(0,0);
  //        |----------------|
  lcd.print("      Modo      ");
  lcd.setCursor(0,1);
  lcd.print("  Manutenzione  ");
  
  delay(3000);
  
  lcd.clear();
  lcd.setCursor(0,0);
  //        |----------------|
  lcd.print(" 1. Autotest    ");
  
  delay(1000);

  // do autotest
  autotest();

  lcd.clear();
  lcd.setCursor(0,0);
  //        |----------------|
  lcd.print(" 2. Lettura     ");
  
  delay(1000);

  must_update_lcd = true;
  measure_interval = MEASURE_HF_INTERVAL; //ms
}

inline void exit_debug(void) {
  in_debug = false;

  lcd.clear();
  lcd.setCursor(0,0);
  
  lcd.print("      Modo      ");
  lcd.setCursor(0,1);
  
  lcd.print("    Normale     ");
  delay(3000);

  measure_interval = MEASURE_LF_INTERVAL; //ms
  must_update_lcd = true;
}

inline void button_interval_5_handle ( void ) {
  must_update_lcd = true;
}

inline void button_interval_4_handle ( void ) {
  show_version();
  must_update_lcd = true;
}

inline void button_interval_3_handle ( void ) {
  if ( in_debug == true ) {
    exit_debug();
  } else {
    enter_debug();
  }
}

inline void button_interval_2_handle ( void ) {
  show_stats();
  must_update_lcd = true;
}

inline void button_interval_1_handle ( void ) {
  must_update_lcd = true;
}

inline void show_menu_options ( const int32_t btn_press_time ) {
  if ( ( btn_press_time >= BTN_INTERVAL_1_TIME ) && ( btn_press_time < BTN_INTERVAL_2_TIME ) ) {
    turn_on_lcd_light();
  } else if( ( btn_press_time >= BTN_INTERVAL_2_TIME ) && ( btn_press_time < BTN_INTERVAL_3_TIME ) ) {      
    lcd.clear();
    lcd.print(" -> Consumi ");
  } else if( ( btn_press_time >= BTN_INTERVAL_3_TIME ) && ( btn_press_time < BTN_INTERVAL_4_TIME ) ) {
    lcd.clear();
    if ( in_debug == false ) {
      lcd.print(" -> Manutenzione");
    } else {
      lcd.print(" -> Normale");
    }
  } else if ( ( btn_press_time >= BTN_INTERVAL_4_TIME ) && (btn_press_time < BTN_INTERVAL_5_TIME) ) {      
    lcd.clear();
    lcd.print(" -> Versione");
  } else if( ( btn_press_time >= BTN_INTERVAL_5_TIME ) ) {      
    lcd.clear();
    lcd.print(" -> Esci Menù");
  }
}

inline void do_menu_actions ( const int32_t btn_press_time ) {
  if (btn_press_time >= BTN_INTERVAL_5_TIME) {
    button_interval_5_handle();
  } else if (btn_press_time >= BTN_INTERVAL_4_TIME) {
    button_interval_4_handle();
  } else if(btn_press_time >= BTN_INTERVAL_3_TIME) {
    button_interval_3_handle();
  } else if(btn_press_time >= BTN_INTERVAL_2_TIME) {      
    button_interval_2_handle();
  } else if(btn_press_time >= BTN_INTERVAL_1_TIME) {      
    button_interval_1_handle();
  }
}

inline void manage_button ( void ) {
  // read button status
  btn_status = digitalRead(LCD_BUTTON_DPIN);
  
  if (btn_status == HIGH && last_btn_status == LOW) {
    // Detect the RISING EDGE 
    last_btn_status     = HIGH;
    timestamp_btn_press = millis();
    
  } else if(btn_status == HIGH && last_btn_status == HIGH) {
    // Detect when kept PRESSED
    
    // show options
    const int32_t btn_press_time = (millis() - timestamp_btn_press);

    show_menu_options ( btn_press_time );
    
  } else if(btn_status == LOW && last_btn_status == HIGH) {
    // Detect the FALLING EDGE
        
    // do actions
    const int32_t btn_press_time = (millis() - timestamp_btn_press);

    do_menu_actions ( btn_press_time );

    last_btn_status     = LOW;
    timestamp_btn_press = 0;
  } else {
    // Reset all data when RELEASED.
    
    last_btn_status     = LOW;
    timestamp_btn_press = 0;
  }
  
}

void setup(void) {

  initialize();

  filter.begin(FILTER_SEED);
  stats.begin();
  
  //16 characters e 2 lines
  lcd.begin(16, 2);
  
  lcd.createChar(UP_ARROW_CHAR, LCD_UP_ARROW);
  lcd.createChar(PROGRESS_CHAR, LCD_PROGRESS);
  
  first_measure_done = false;

  in_debug = false;
  
  //compute tank parameters
  tank_capacity     = (TANK_RADIUS_CM * TANK_RADIUS_CM * PI * WATER_MAX_HEIGHT_CM) / CM3_PER_LITER;
  maximum_capacity  = TANK_NUMBER * tank_capacity;
  
  //pin setup
  pinMode(ECHO_DPIN,         INPUT );
  pinMode(TRIG_DPIN,         OUTPUT);
  pinMode(LED_CAPACITY_DPIN, OUTPUT);
  pinMode(LCD_LIGHT_DPIN,    OUTPUT);
  pinMode(LCD_BUTTON_DPIN,   INPUT );

  delay(1000);
  autotest();
  lcd.print("Avvio...");
  lcd.setCursor(0,1);
  lcd.print(VERSION);
   
  led_on     = false;
  led_status = false;

  digitalWrite(LCD_LIGHT_DPIN, HIGH);

  const int32_t timestamp_now = millis();

  timestamp_lcd_on         = timestamp_now;
  timestamp_measurement    = timestamp_now;
  timestamp_last_led_ctrl  = timestamp_now;
  timestamp_last_read_dist = timestamp_now;
  timestamp_last_filling   = timestamp_now;

#if DEBUG
  Serial.begin(9600);
  Serial.print("Tank Radius (cm): ");
  Serial.println(TANK_RADIUS_CM);
  
  Serial.print("Water Max Height (cm): ");
  Serial.println(WATER_MAX_HEIGHT_CM);
  
  Serial.print("Maximum Capacity (L): ");
  Serial.println(maximum_capacity);
#endif

  delay(1000);
  
  lcd.clear();
  lcd.print("Prima misura...");
}

void loop(void) {

  // read current milliseconds passed from boot.
  const int32_t timestamp_now = millis();

  // consumption time management
  stats.updateTime();

  // button management
  manage_button();
  
  // lcd light timer
  if ((timestamp_now - timestamp_lcd_on) >= LCD_ON_TIME) {
    turn_off_lcd_light();
  }

  // measurement timer
  if ((timestamp_now - timestamp_measurement) >= measure_interval) {
    distance = measure_level();
    if(abs((-1.0) - distance) < 0.1) {
      print_error();
      delay(READ_DISTANCE_TIME);
      return;
    }
    ( void ) filter.in(roundfvalue(distance));
  }

  // read distance timer
  if ((timestamp_now - timestamp_last_read_dist) >= READ_DISTANCE_TIME) {
    distance = filter.out();
    distance = sanitize_data(distance, SENSOR_DISTANCE_CM, (WATER_MAX_HEIGHT_CM + SENSOR_DISTANCE_CM), SENSOR_DISTANCE_CM, (WATER_MAX_HEIGHT_CM + SENSOR_DISTANCE_CM));

    if (first_measure_done) {
      float64_t liters = compute_liters(distance);
      liters = sanitize_data(liters, 0.0, maximum_capacity, 0.5, maximum_capacity);

      if( ( previous_liters != liters ) || ( was_error == true ) ) {
        must_update_lcd = true;
        was_error       = false;
      }
      previous_liters = liters;
    
      percentage = compute_percentage(liters);
      stats.updateConsumption(liters);
      
      if ( in_debug == false ) {
        update_lcd(percentage, liters);
      }
    }

    timestamp_last_read_dist = timestamp_now;
  }

  // led control timer
  if ( ( timestamp_now - timestamp_last_led_ctrl ) >= LED_CONTROL_TIME ) {
    control_led(percentage); 
    timestamp_last_led_ctrl = timestamp_now;
  }

  // go to sleep
  delay(SLEEP_TIME);
}
