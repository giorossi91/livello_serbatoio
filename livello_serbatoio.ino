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
#define VERSION "v0.8"

#define CONF_DEBUG   1
#define CONF_RELEASE 0

#define SENSOR_HCSR04   0
#define SENSOR_JSNSR04T 1

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
const uint32_t    MEASURE_LF_INTERVAL   = 10 * 1000                           ; // ms
const uint32_t    MEASURE_HF_INTERVAL   =  1 * 1000                           ; // ms
const uint32_t    LCD_ON_TIME           = 30 * 1000                           ; // ms
const uint32_t    SLEEP_TIME            =  100                                ; // ms
const uint32_t    SHOW_STAT_TIME        = 5000                                ; // ms
const uint32_t    SHOW_PARAM_TIME       = 5000                                ; // ms
const uint32_t    SHOW_VERSION_TIME     = 3000                                ; // ms
const uint32_t    LED_CONTROL_TIME      = 1000                                ; // ms
const uint32_t    READ_DISTANCE_TIME    = 1000                                ; // ms
const uint32_t    BTN_INTERVAL_1_TIME   =  200                                ; // ms
const uint32_t    BTN_INTERVAL_2_TIME   = 2000                                ; // ms
const uint32_t    BTN_INTERVAL_3_TIME   = 3000                                ; // ms
const uint32_t    BTN_INTERVAL_4_TIME   = 4000                                ; // ms
const uint32_t    BTN_INTERVAL_5_TIME   = 5000                                ; // ms
const uint32_t    BTN_INTERVAL_6_TIME   = 6000                                ; // ms

// Sensor parameters
#if SENSOR == SENSOR_JSNSR04T

const float64_t   SENSOR_CALIBRATION    = 1.7                                 ; // cm
const float64_t   SENSOR_MIN_RANGE      = 20.0                                ; // cm
const float64_t   SENSOR_MAX_RANGE      = 600.0                               ; // cm
const float64_t   SENSOR_LSB            = 58.0                                ; // usec / cm
const int32_t     SENSOR_ACTIVATION_TH  = 15                                  ; // usec

const uint32_t    SENSOR_RESPONSE_TIMEOUT_US = 500000UL                       ; // 500 ms
const uint32_t    SENSOR_NO_OBSTACLE_US      = 38000UL                        ; // 38 ms

#elif SENSOR == SENSOR_HCSR04

const float64_t   SENSOR_CALIBRATION    = 0.0                                 ; // cm
const float64_t   SENSOR_MIN_RANGE      = 2.0                                 ; // cm
const float64_t   SENSOR_MAX_RANGE      = 400.0                               ; // cm
const float64_t   SENSOR_LSB            = 58.0                                ; // usec / cm
const int32_t     SENSOR_ACTIVATION_TH  = 15                                  ; // usec

const uint32_t    SENSOR_RESPONSE_TIMEOUT_US = 500000UL                       ; // 500 ms
const uint32_t    SENSOR_NO_OBSTACLE_US      = 38000UL                        ; // 38 ms

#else

const float64_t   SENSOR_CALIBRATION    = 0.0                                 ; // cm
const float64_t   SENSOR_MIN_RANGE      = 0.0                                 ; // cm
const float64_t   SENSOR_MAX_RANGE      = 0.0                                 ; // cm
const float64_t   SENSOR_LSB            = 1.0                                 ; // usec / cm
const int32_t     SENSOR_ACTIVATION_TH  = 15                                  ; // usec

const uint32_t    SENSOR_RESPONSE_TIMEOUT_US = 500000UL                       ; // 500 ms
const uint32_t    SENSOR_NO_OBSTACLE_US      = 38000UL                        ; // 38 ms

#endif

// LCD constants
const int32_t     LCD_COLS              = 16                                  ;
const int32_t     LCD_ROWS              =  2                                  ;

// LCD special characters
const byte        PROGRESS_CHAR         = 1                                   ;
const int32_t     LCD_CHAR_SIZE         = 8                                   ;
const byte        LCD_PROGRESS[LCD_CHAR_SIZE] = { 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F };

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
static volatile bool        led_on                    ;
static volatile bool        led_status                ;
static volatile bool        must_update_lcd           ;
static volatile bool        was_error                 ;
static volatile bool        in_debug                  ;
static volatile bool        first_measure_done        ;
static volatile byte        btn_status                ;
static volatile byte        last_btn_status           ;
static volatile int32_t     err_code                  ;
static volatile bool        is_displaying_slot_1      ;
static volatile bool        is_displaying_slot_2      ;
static volatile bool        is_displaying_slot_3      ;
static volatile bool        is_displaying_slot_4      ;
static volatile bool        is_displaying_slot_5      ;
static volatile bool        is_displaying_slot_6      ;

// Timestamps
static volatile uint32_t    timestamp_lcd_on          ;
static volatile uint32_t    timestamp_measurement     ;
static volatile uint32_t    timestamp_last_led_ctrl   ;
static volatile uint32_t    timestamp_last_read_dist  ;
static volatile uint32_t    timestamp_last_filling    ;
static volatile uint32_t    timestamp_btn_press       ;

// Timings
static uint32_t             measure_interval          ;

// Computed tank(s) parameters
static float64_t            tank_capacity             ;
static float64_t            maximum_capacity          ;

// Current reads
static uint16_t             number_measures_done      ;   
static float64_t            percentage                ;
static volatile float64_t   distance                  ;
static volatile float64_t   previous_liters           ;

// Classes

class LcdHelper {
public:
  LcdHelper ( void ) {
    lcd_instance = nullptr;
  }

  void begin ( LiquidCrystal * const lcd ) {

    this->lcd_instance = lcd;

    for ( uint16_t i = 0U; i < LCD_ROWS; i++ ) {
      for ( uint16_t j = 0U; j < LCD_COLS; j++ ) {
         this->lcd_matrix[i][j] = EMPTY_CHAR;
         this->last_lcd_matrix[i][j] = EMPTY_CHAR;
         this->special_char_map[i][j] = false;
         this->last_special_char_map[i][j] = false;
      }
    }
    this->home();

    this->lcd_instance->clear();
  }

  void clear ( void ) {    
    for ( uint16_t i = 0U; i < LCD_ROWS; i++ ) {
      for ( uint16_t j = 0U; j < LCD_COLS; j++ ) {
         this->lcd_matrix[i][j] = EMPTY_CHAR;
         this->special_char_map[i][j] = false;
      }
    }
    this->home();
  }

  void setCursor ( const uint16_t c, const uint16_t r ) {
    if ( ( c < LCD_COLS ) && ( r < LCD_ROWS ) ) {
      this->cursor_c = c;
      this->cursor_r = r;
    }
  }

  void home ( void ) {
    setCursor ( 0U, 0U );
  }

  void print ( const char * str, const uint16_t str_length = LCD_COLS ) {
    for ( uint16_t str_i = 0U; str_i < str_length; str_i ++ ) {
      if ( str[str_i] == '\0' ) {
        break;
      }

      if ( this->cursor_c < LCD_COLS ) {
        this->lcd_matrix[this->cursor_r][this->cursor_c] = str[str_i];
        this->cursor_c ++;
      }
    }
        
  }

  void print ( const int32_t number ) {
    const String number_str = String ( number );
    this->print ( number_str.c_str() );
  }

  void print ( const String& str ) {
    this->print ( str.c_str() );
  }

  void print ( const int16_t number ) {
    const String number_str = String ( number );
    this->print ( number_str.c_str() );
  }

  void print ( const uint16_t number ) {
    const String number_str = String ( number );
    this->print ( number_str.c_str() );
  }

  void print ( const float64_t number ) {
    const String number_str = String ( number );
    this->print ( number_str.c_str() );
  }

  void write ( const int32_t special_char_index ) {
    if ( ( special_char_index >= 0 ) && ( special_char_index <= 9 ) ) {
      this->special_char_map[this->cursor_r][this->cursor_c] = true;
  
      this->print ( special_char_index );
      
      cursor_c ++;
    }
  }

  void update_lcd ( void ) {
    if ( need_update() == true ) {
      // write different chars
      for ( uint16_t i = 0U; i < LCD_ROWS; i++ ) {
        for ( uint16_t j = 0U; j < LCD_COLS; j++ ) {
          if ( ( this->lcd_matrix[i][j] != this->last_lcd_matrix[i][j]             ) ||
               ( this->special_char_map[i][j] != this->last_special_char_map[i][j] )    ) {
            this->lcd_instance->setCursor(j, i);
            if ( is_special_symbol ( i, j ) == true ) {
              this->lcd_instance->write ( String(this->lcd_matrix[i][j]).toInt() );
            } else {
              this->lcd_instance->print ( this->lcd_matrix[i][j] );
            }
            
            this->last_lcd_matrix[i][j] = this->lcd_matrix[i][j];
            this->last_special_char_map[i][j] = this->special_char_map[i][j];
          }
        }
      }      
    }
  }

private:
  bool is_special_symbol ( const uint16_t r, const uint16_t c ) {
    bool is_ss = false;
    if ( ( r < LCD_ROWS ) && ( c < LCD_COLS ) ) {
      char symbol = this->lcd_matrix[r][c];
      if ( ( symbol >= '0' ) && ( symbol <= '9' ) && special_char_map[r][c] == true ) {
        is_ss = true;
      }
    }
    return is_ss;
  }

  bool need_update ( void ) {
    bool is_update_needed = false;
    
    for ( uint16_t i = 0U; i < LCD_ROWS; i++ ) {
      for ( uint16_t j = 0U; j < LCD_COLS; j++ ) {
        if ( last_lcd_matrix[i][j] != lcd_matrix[i][j] ) {
          is_update_needed = true;
        }
      }
    }

    return is_update_needed;
  }

  uint16_t cursor_r;
  
  uint16_t cursor_c;

  static const char EMPTY_CHAR = ' ';
  
  char lcd_matrix[LCD_ROWS][LCD_COLS];

  char last_lcd_matrix[LCD_ROWS][LCD_COLS];

  bool special_char_map[LCD_ROWS][LCD_COLS];

  bool last_special_char_map[LCD_ROWS][LCD_COLS];

  LiquidCrystal *lcd_instance; 
};

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
    for ( byte i = 0; i < medFilterWin; i++ ) {
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
  int16_t in ( const int16_t value ) {
    // sort sizeMap
    // small vaues on the left (-)
    // larger values on the right (+)
    
    boolean dataMoved = false;

    // adjusted for zero indexed array
    const byte rightEdge = medFilterWin - 1;

    // store new data in location of oldest data in ring buffer
    data[oldestDataPoint] = value;
    
    // SORT LEFT (-) <======(n) (+)
    if ( locationMap[oldestDataPoint] > 0 ) {
      // don't check left neighbours if at the extreme left
      for ( byte i = locationMap[oldestDataPoint]; i > 0; i-- ) {
        //index through left adjacent data

        // neighbour location
        byte n = i - 1;

        if ( data[oldestDataPoint] < data[sizeMap[n]] ) {
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
      
      for ( byte i = locationMap[oldestDataPoint]; i < rightEdge; i++ ) {
        //index through left adjacent data

        // neighbour location
        uint16_t n = i + 1;  
    
        if ( data[oldestDataPoint] > data[sizeMap[n]] ) {
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
    if ( oldestDataPoint == medFilterWin ) {
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
  int16_t out ( void ) const {
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
  ConsumptionData ( void ) {
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
  void updateTime ( void ) {

    // get current milliseconds passed from boot
    uint32_t now         = millis();
    uint32_t millis_diff = 0U;

    // compute delta ms with respect to previous invocation
    millis_diff = static_cast< uint32_t >( now - last_millis );

    // update ms of previous invocation
    last_millis = now;

    // increase count of milliseconds passed.
    millis_passed += millis_diff;

    if ( millis_passed >= ONE_SECOND_MS ) {
      // after 1 second

      // update buffer
      updateBuffer();

      uint32_t n_sec = static_cast< uint32_t >( millis_passed / ONE_SECOND_MS );

      // increase seconds counter
      seconds_passed += n_sec;
      millis_passed   = millis_passed - ( n_sec * ONE_SECOND_MS ); 
    }

    if ( seconds_passed >= ONE_HOUR_S ) {
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
  void updateConsumption ( const uint32_t current_liters ) {
    if ( current_liters > last_liters ) {
      // filling
      const uint32_t temp_refill = ( current_liters - last_liters );
      if ( temp_refill > LITERS_THRESHOLD ) {
        // hysteresis (sensor readings are often noisy)
        last_liters = current_liters;
      }
    } else { 
      // emptying
      const uint32_t temp_consumption = ( last_liters - current_liters );
      if ( temp_consumption > LITERS_THRESHOLD ) {
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
  uint32_t getConsumption1h ( void ) const {
    return sumSamples ( 1U );
  }

  //!
  //! \brief Gives the total consumption in the last 12 hours.
  //!
  //! \details Sums the stored data.
  //! 
  //! \return The water consumption in liters.
  //!
  uint32_t getConsumption12h ( void ) const {
    return sumSamples ( 12U );
  }

  //!
  //! \brief Gives the total consumption in the last day.
  //!
  //! \details Sums the stored data.
  //! 
  //! \return The water consumption in liters.
  //!
  uint32_t getConsumption1d ( void ) const {
    return sumSamples ( 24U );
  }


  //!
  //! \brief Gives the total consumption in the last 3 days.
  //!
  //! \details Sums the stored data.
  //! 
  //! \return The water consumption in liters.
  //!
  uint32_t getConsumption3d ( void ) const {
    return sumSamples ( 24U * 3U );
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
  uint32_t sumSamples ( const uint32_t back_time ) const {
    uint32_t sum = 0U;
    
    if ( back_time >= STAT_SIZE ) {      
      // impossible value in 3 days (see assumption above).
      sum = 9999U;
    } else {
      int32_t i = static_cast< int32_t > ( index );
      uint32_t time_index = back_time;

      // go back to #back_time hours
      while ( time_index != 0U ) {

        // sum the consumption
        sum += consumption_samples[i];
        i--;

        // restart from beginning if buffer is terminated
        if ( i < 0 ) {
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
  void updateBuffer ( void ) {
    consumption_samples[index] += consumption;
    consumption = 0U;
  }

  //!
  //! \brief Updates the buffer index.
  //!
  //! \details Updates the buffer index and reset the older values to 0.
  //!
  void updateIndex ( void ) {
    index ++;
    if ( index >= STAT_SIZE ) {
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
static LiquidCrystal lcd_if ( RS, E, DB4, DB5, DB6, DB7 );

static LcdHelper lcd;

static MedianFilter filter;

static ConsumptionData stats;

// SW Functions

//!
//! \brief Determines if two float values are equal.
//!
//! \param[in] x The first value.
//! \param[in] y The second value.
//! \param[in] th The threshold.
//!
//! \return 0 if different, 1 if equal (distance between #x and #y is below #th).
//!
inline int32_t compare_float ( const float64_t x, const float64_t y, const float64_t th ) {
  int32_t ret_value = 0;
  
  if ( abs ( x - y ) <= th ) {
    ret_value = 1;
  }

  return ret_value;
}

//!
//! \brief Round the given float value to the nearest integer.
//!
//! \param[in] x The value.
//!
//! \return The rounded integer value.
//!
inline int32_t round_float_value ( const float64_t x ) {
  int32_t rounded_x = 0;
  if ( x >= 0.0 ) {
    rounded_x = static_cast < int32_t > ( x + 0.5 );
  } else {
    rounded_x = static_cast < int32_t > ( x - 0.5 );
  }
  return rounded_x;
}

//!
//! \brief Initializes all variables.
//!
inline void initialize ( void ) {
  led_on                   = false               ;
  led_status               = false               ;
  must_update_lcd          = true               ; // TODO
  first_measure_done       = false               ;
  was_error                = false               ;
  err_code                 = ERR_OK              ;
  btn_status               = LOW                 ;
  last_btn_status          = LOW                 ;
  is_displaying_slot_1     = false               ;
  is_displaying_slot_2     = false               ;
  is_displaying_slot_3     = false               ;
  is_displaying_slot_4     = false               ;
  is_displaying_slot_5     = false               ;
  is_displaying_slot_6     = false               ;
  
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
  const float64_t filling_percentage = ( read_liters / maximum_capacity ) * 100.0;
  
  return filling_percentage;
}

//!
//! \brief Turns on the LCD backlight.
//!
//! \details Raises the backlight digital pin and activates the timer to turn off the LCD backlight.
//!
inline void turn_on_lcd_light ( void ) {
  // activate backlight
  digitalWrite ( LCD_LIGHT_DPIN, HIGH );

  // get timestamp for future deactivation
  timestamp_lcd_on = millis();
#if DEBUG
  Serial.print   ( "LCD on -> t:"   );
  Serial.println ( timestamp_lcd_on );
#endif
}

//!
//! \brief Turns off the LCD backlight.
//!
//! \details Lowers the backlight digital pin.
//!
inline void turn_off_lcd_light ( void ) {

  // deactivate backlight
  digitalWrite ( LCD_LIGHT_DPIN, LOW );
#if DEBUG
  Serial.println ( "LCD off" );
#endif
}

//!
//! \brief Prints an error message to LCD if not in debug mode.
//!
inline void print_error ( void ) {

  if ( in_debug == false ) {
    // only in normal mode
    lcd.clear();
    lcd.setCursor(5, 1);
    lcd.print("Errore");

    // the LCD is printing an error message, may need an update
    was_error = true;
  }
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
  if ( ( in_debug == true ) && ( last_btn_status == LOW ) ) {
    // if in debug mode and menu is not shown (the button in released)
    
    lcd.setCursor ( 12, 0 );
    lcd.print ( "e: " );
    lcd.setCursor ( 14, 0 );
    lcd.print ( err_code );

    must_update_lcd = true;
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
inline void update_lcd_debug ( const float64_t distance_to_print, const float64_t distance_comp ) {
  if ( ( in_debug == true ) && ( must_update_lcd == true ) ) {
    // if in debug mode and LCD needs an update

    // reset update LCD flag
    // TODO must_update_lcd = false;

    // clear the LCD
    lcd.clear();

    // show distance
    lcd.setCursor ( 0, 0 );
    lcd.print ( "D:" );
    lcd.print ( static_cast< int32_t > ( distance_to_print ) );
    lcd.print ( " cm" );

    // show the calibrated distance (depending on sensor)
    lcd.setCursor ( 0, 1 );
    lcd.print ( "DC:" );
    lcd.print ( static_cast< int32_t > ( distance_comp ) );

    // show volume
    lcd.print ( " L:" );
    lcd.print ( static_cast< int32_t > ( compute_liters ( distance_comp ) ) );
  }
}

//!
//! \brief Executes a measurement of water level.
//!
//! \details Activates the ultrasonic sensor and applies the needed corrections (e.g. calibration).
//! 
//! \return The distance from water in centimeters or -1 in case of errors.
//!
inline float64_t measure_level ( void ) {
  float64_t distance_output = 0.0;
  
  // activate distance measurement
  digitalWrite ( TRIG_DPIN, HIGH );
  delayMicroseconds( SENSOR_ACTIVATION_TH );
  digitalWrite ( TRIG_DPIN, LOW );

  // read sensor response (pulse time)
  const uint32_t pulse_time = pulseIn ( ECHO_DPIN, HIGH, SENSOR_RESPONSE_TIMEOUT_US );
  if ( ( pulse_time == 0U ) || ( pulse_time >= SENSOR_NO_OBSTACLE_US ) ) {
    // assign proper error code
    err_code = ERR_SENS;

    // show error code if in debug
    show_err_code_debug();

    // assign -1 to return value
    distance_output = -1.0;
  }

  if ( compare_float ( distance_output, -1.0, 0.1 ) == 0 ) {

    // convert to cm
    const float64_t distance_read = static_cast< float64_t > ( pulse_time ) / SENSOR_LSB;
    
    // apply calibration
    const float64_t dist_compensated = distance_read + SENSOR_CALIBRATION;
  
    if ( ( in_debug == true ) && ( last_btn_status == LOW ) ) {
      // if in debug and not showing the menu (button is released)
      must_update_lcd = true;

      // update LCD with debug data
      update_lcd_debug ( distance_read, dist_compensated );
    }
  
    // check ranges
    if ( ( distance_read < SENSOR_MIN_RANGE ) || ( distance_read > SENSOR_MAX_RANGE ) ) {
      // if out of range with respect to sensor limits

      // assign proper error code
      err_code = ERR_RANGE;

      // show error code on LCD if in debug mode
      show_err_code_debug();

      // assign -1 to return value
      distance_output = -1.0;
    }
    
#if DEBUG
    Serial.print   ( "dist: "                               );
    Serial.print   ( distance_read                          );
    Serial.print   ( " + calib: "                           );
    Serial.print   ( dist_compensated                       );
    Serial.print   ( " round: "                             );
    Serial.println ( round_float_value ( dist_compensated ) );
#endif

    // if all ok
    if ( compare_float ( distance_output, -1.0, 0.1 ) == 0 ) {

      // assign timestamp to measure (to compute next time of measure)
      timestamp_measurement = millis();
    
      // if we are doing the first measures
      if(first_measure_done == false) {
        number_measures_done++;
      }

      // if we have done enough measures to feed the median filter.
      if ( number_measures_done > ( FILTER_SIZE / 2 ) ) {

        // flag first measure done
        first_measure_done = true;
        
        if ( in_debug == true ) {
          // if in debug, use a higher frequency measure timing for better responsiveness
          measure_interval = MEASURE_HF_INTERVAL; //ms
        } else {
          // if not in debug, use a lower frequency measure timing (low responsiveness required)
          measure_interval = MEASURE_LF_INTERVAL; //ms
        }
      }

      // reset error
      err_code = ERR_OK;

      // show "no error" info on LCD in debug mode
      show_err_code_debug();

      // assign distance to be returned
      distance_output = dist_compensated;
    }
  }
    
  return distance_output;
}

//!
//! \brief Controls the LED status depending on water level.
//!
//! \details If the tank filling is under LOW_LEVEL_THRESHOLD but over EMPTY_LEVEL_THRESHOLD turns on the LED.
//!          If the tank filling is under EMPTY_LEVEL_THRESHOLD makes the LED to blink.
//!          In all other cases the LED is off.
//!          
//! \param[in] water_percentage The percentage of filling of the tank (in range [0, 100]).
//!
inline void control_led ( const float64_t water_percentage ) {
  if ( water_percentage < 0.0 ) {
    // below 0, all OFF
    led_status = false;
    led_on     = false;
  } else if ( water_percentage <= EMPTY_LEVEL_THRESHOLD ) {
    // LED blinks
    led_status = true;
    led_on     = !led_on;
  } else if ( water_percentage <= LOW_LEVEL_THRESHOLD ) {
    // LED on
    led_status = true;
    led_on     = true;
  } else {
    // LED off
    led_status = false;
    led_on     = false;
  }

  // write new led status
  digitalWrite ( LED_CAPACITY_DPIN, led_on );
}

//!
//! \brief Sanitizes the input value and contraints it in specified valid range.
//!
//! \details If #data is under #min_threshold it returns #min_val.
//!          If #data is over #max_threshold it returns #max_val.
//!          If in range it returns #data.
//!        
//! \param[in] data The value to be sanitized.
//! \param[in] min_val The minimum value in the admissible range.
//! \param[in] max_val The maximum value in the admissible range.
//! \param[in] min_threshold The lowest threshold value.
//! \param[in] max_threshold The highest threshold value.
//! 
//! \return The value in range [#min_val, #max_val].
//!
inline float64_t sanitize_data ( const float64_t data, const float64_t min_val, const float64_t max_val, const float64_t min_threshold, const float64_t max_threshold ) {
  float64_t sanitized_data = min_val;
  
  if ( data < min_threshold ) {
    // if below the minimum value threshold assign to minimum
    sanitized_data = min_val;
  } else if ( data > max_threshold ) {
    // if over the maximum value threshold assign to maximum
    sanitized_data = max_val;
  } else {
    // else output the same value
    sanitized_data = data;
  }

  // return the sanitized value
  return sanitized_data;
}


//!
//! \brief Updates the LCD with the actual parameters if needed.
//! \details The data are printed as follows:
//! 
//!  |----------------| 
//!  | xxxx L   yyy % |
//!  | ############## |
//!  |----------------|
//!  
//!  ex:
//!  |----------------| 
//!  |  876 L    87 % |
//!  | ############   |
//!  |----------------|
//!  
//!  |----------------| 
//!  | xxxx L   yyy % |
//!  | ##  Riserva    |
//!  |----------------|
//!  
//!  |----------------| 
//!  | xxxx L   yyy % |
//!  |     Vuoto      |
//!  |----------------|
//!  
//!  <xxxx> = [0, 9999] L
//!  <yyy> = [0, 100] %
//!  # X 16 => 0 = 0%, 16 = 100%
//!  
//! \param[in] water_percentage The tank filling percentage.
//! \param[in] liters The water volume in liters.
//!
inline void update_lcd ( const float64_t water_percentage, const float64_t liters ) {
  if( ( water_percentage < 0.0 ) || ( water_percentage > 100.0 ) || ( liters < 0.0 ) || ( liters > 9999.9 ) ) {
    // if data are invalid, don't update the LCD
    // TODO must_update_lcd = false;
  }
  
  if ( must_update_lcd == true ) {
    // if the LCD must be updates
    // TODO must_update_lcd = false;

    // Clear the LCD
    lcd.clear();

    // Print unit symbols in specified locations
    lcd.setCursor ( 6, 0 );
    lcd.print ( "L" );
    lcd.setCursor ( 14, 0 );
    lcd.print ( "%" );

    // print the liters

    // determine the index of the first cipher of #liters to be written.
    int16_t index = 4;
    const int16_t l = static_cast< int16_t > ( liters );

    // 2 digits
    if ( l >= 10 ) {
      index--;
    }
    
    // 3 digits
    if ( l >= 100 ) {
      index--;
    }
    
    // 4 digits
    if ( l >= 1000 ) {
      index--;
    }

    // set the cursor
    lcd.setCursor ( index, 0 );

    // print the value
    lcd.print ( l );

    // print the percentage

    // sanitize the value (constrained in 0, 100).
    const float64_t sanitized_water_percentage = sanitize_data(water_percentage, 0.0, 100.0, 0.5, 99.5);

    // compute the starting index to print the #water_percentage
    const int16_t p = static_cast< int16_t > ( sanitized_water_percentage );
    index = 12;

    // 2 digits
    if ( p >= 10 ) {
      index --;
    }

    // 3 digits
    if( p >= 100 ) {
      index --;
    }

    // set the cursor
    lcd.setCursor ( index, 0 );

    // print the value
    lcd.print ( p );

    // set the cursor to print the progress bar
    lcd.setCursor( 1, 1 );

    // determine how many symbols must be printed to represent the percentage of filling

    // center the progress bar (leave 2 space at the edges)
    const int16_t howmany = ( ( LCD_COLS - 2 ) * p ) / 100;
    for ( int16_t i = 0; i < howmany; i++ ) {
      // print each symbol
      lcd.write(PROGRESS_CHAR);
    }
  
    if ( howmany == 0 ) {
      // special case: empty
      lcd.setCursor ( 5, 1 );
      lcd.print ( "Vuoto" );
    } else if ( howmany <= 2 ) {
      // special case: under minimum threashold (for better readability)
      lcd.setCursor ( 5, 1 );
      lcd.print ( "Riserva" );
    }
  }
}

//!
//! \brief Executes an autotest of LCD and LED.
//!
inline void autotest ( void ) {
  // 1. Reset state of LCD backlight and text and LED (all off)
  digitalWrite ( LCD_LIGHT_DPIN, LOW );
  digitalWrite ( LED_CAPACITY_DPIN, LOW );
  lcd.clear();

  // wait 1 second to let user to see result
  delay ( 1000 );

  // 2. Turn on all
  digitalWrite ( LCD_LIGHT_DPIN, HIGH );
  digitalWrite ( LED_CAPACITY_DPIN, HIGH );

  // wait 1 second to let user to see result
  delay ( 1000 );

  // 3. Write character to fill the LCD
  lcd.clear();
  lcd.setCursor ( 0, 0 );
  for ( uint16_t i = 0; i < 16; i++ ) {
    // this symbol turns on all pixels of the LCD (first row)
    lcd.write ( PROGRESS_CHAR );
  }
  lcd.setCursor ( 0, 1 );
  for ( uint16_t i = 0; i < 16; i++ ) {
    // this symbol turns on all pixels of the LCD (second row)
    lcd.write ( PROGRESS_CHAR );
  }

  // wait 3 second to let user to see result
  delay ( 3000 );

  // 4. Reset state
  digitalWrite ( LED_CAPACITY_DPIN, LOW );
  lcd.clear();

  // wait and back to normal
  delay ( 1000 );
}

//!
//! \brief Prints on LCD the consumption statistic passed.
//!
//! \details Follows the format:
//!  |----------------| 
//!  |<      str     >|
//!  |Tot. <stat>   L |
//!  |----------------|
//!  
//! \param[in] str The statistic title.
//! \param[in] stat The statistic value.
//! 
inline void print_stat ( String str, const uint32_t stat ) {
  if ( stat == 9999U ) {
    // assign proper error code to display
    err_code = ERR_STAT;
  } else {
    // all ok, the value is valid
    err_code = ERR_OK;
  }

  // show the error code in debug mode
  show_err_code_debug();

  // print the title
  lcd.setCursor ( 0, 0 );
  lcd.print ( str );

  // print the value
  lcd.setCursor ( 0, 1 );
  lcd.print ( "Tot. " );
  lcd.print ( static_cast< int32_t >( stat ) );
  lcd.setCursor ( 14, 1 );
  lcd.print ( "L" );

  // wait to let user see the value
  delay ( SHOW_STAT_TIME );

  // clear all
  lcd.clear(); 
}

//!
//! \brief Prints the SW version on LCD.
//!
//! \details Follows the format:
//!  |----------------| 
//!  | Versione:      |
//!  | <num_version>  |
//!  |----------------|
//!
inline void show_version ( void ) {
  lcd.clear();

  // print the version
  lcd.setCursor ( 0, 0 );
  lcd.print ( " Versione:" );
  lcd.setCursor ( 0, 1 );
  lcd.print ( " " );
  lcd.print ( VERSION );

  lcd.setCursor ( 12, 1 );
  lcd.print ( "GR21" );

  // wait to let the user see
  delay(SHOW_VERSION_TIME);

  // clear all
  lcd.clear();
}

//!
//! \brief Prints the tank parameters (diameter, number of tanks, max water heigh, max total capacity).
//!
//! \details Follows the format:
//!  |----------------| 
//!  |D:xxxxx N:xx    |
//!  |H:xxxxx C:xxxxx |
//!  |----------------|
//!
inline void show_params ( void ) {
  lcd.clear();
  
  lcd.setCursor ( 0, 0 ); 
  lcd.print ( "D:" );
  lcd.print ( static_cast < int32_t > ( TANK_RADIUS_CM * 2.0 ) );

  lcd.print ( " N:" );
  lcd.print ( TANK_NUMBER );

  lcd.setCursor ( 0, 1 );

  lcd.print ( "H:" );
  lcd.print ( static_cast < int32_t > ( WATER_MAX_HEIGHT_CM ) );

  lcd.print ( " C:" );
  lcd.print ( static_cast < int32_t > ( maximum_capacity ) );

  // wait to let user see the value
  delay ( SHOW_PARAM_TIME );

  // clear all
  lcd.clear();
}

//!
//! \brief Prints on LCD all the statistics.
//!
//! \details Prints consumptions since:
//!          - 1 hour;
//!          - 12 hours;
//!          - 1 day;
//!          - 3 days.
//!
inline void show_stats ( void ) {
  lcd.clear();
  
  // print 1h
  print_stat ( "Consumo 1 ora", stats.getConsumption1h() );
  
  // print 12h
  print_stat ( "Consumo 12 ore", stats.getConsumption12h() );
  
  // print 1d
  print_stat ( "Consumo 1 giorno", stats.getConsumption1d() );

  // print 3d
  print_stat ( "Consumo 3 giorni", stats.getConsumption3d() );
}

//!
//! \brief Changes operative mode to DEBUG.
//!
inline void enter_debug ( void ) {
  // update status variable
  in_debug = true;

  // notify the user by writing on LCD
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("      Modo      ");
  lcd.setCursor(0,1);
  lcd.print("  Manutenzione  ");

  // wait to let user read
  delay(3000);

  // perform an autotest
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" 1. Autotest    ");

  // wait to let user read
  delay ( 1000 );

  // do autotest
  autotest();

  // perform real-time readings
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" 2. Lettura     ");

  // wait to let user read
  delay(1000);

  // update the LCD
  must_update_lcd = true;

  // change measure frequency to the highest value
  measure_interval = MEASURE_HF_INTERVAL; //ms

  // turn on LCD
  turn_on_lcd_light();
  timestamp_lcd_on  = millis();
}

//!
//! \brief Changes operative mode to NORMAL.
//!
inline void exit_debug ( void ) {
  // update status variable
  in_debug = false;

  // print the information on LCD
  lcd.clear();
  lcd.setCursor ( 0,0 );
  lcd.print("      Modo      ");
  lcd.setCursor(0,1);
  lcd.print("    Normale     ");

  // wait to let user read
  delay(3000);

  // update the LCD
  must_update_lcd = true;

  // change measure frequency to the lowest value
  measure_interval = MEASURE_LF_INTERVAL; //ms
}

//!
//! \brief Actions for sixth menu slot.
//!
inline void button_interval_6_handle ( void ) {
  must_update_lcd = true;
}

//!
//! \brief Actions for fifth menu slot.
//!
inline void button_interval_5_handle ( void ) {
  show_params();
  must_update_lcd = true;
}

//!
//! \brief Actions for fourth menu slot.
//!
inline void button_interval_4_handle ( void ) {
  show_version();
  must_update_lcd = true;
}

//!
//! \brief Actions for third menu slot.
//!
inline void button_interval_3_handle ( void ) {
  if ( in_debug == true ) {
    exit_debug();
  } else {
    enter_debug();
  }
}

//!
//! \brief Actions for second menu slot.
//!
inline void button_interval_2_handle ( void ) {
  show_stats();
  must_update_lcd = true;
}

//!
//! \brief Actions for first menu slot.
//!
inline void button_interval_1_handle ( void ) {
  // do nothing
}

//!
//! \brief Prints on LCD the menu options based on button press time.
//!
//! \param[in] btn_press_time Time since button is kept pressed.
//!
inline void show_menu_options ( const uint32_t btn_press_time ) {
  if ( ( btn_press_time >= BTN_INTERVAL_1_TIME ) && ( btn_press_time < BTN_INTERVAL_2_TIME ) ) {
    // slot 1
    turn_on_lcd_light();
  } else if( ( btn_press_time >= BTN_INTERVAL_2_TIME ) && ( btn_press_time < BTN_INTERVAL_3_TIME ) ) {   
    // slot 2
    if ( is_displaying_slot_2 == false ) {
      lcd.clear();
      lcd.print ( " -> Consumi " );

      is_displaying_slot_1 = false;
      is_displaying_slot_2 = true;
    }
  } else if( ( btn_press_time >= BTN_INTERVAL_3_TIME ) && ( btn_press_time < BTN_INTERVAL_4_TIME ) ) {
    // slot 3
    if ( is_displaying_slot_3 == false ) {
      lcd.clear();
      lcd.print ( " -> Modo" );
      lcd.setCursor ( 0, 1 );
      if ( in_debug == false ) {
        lcd.print ( "    Manutenzione" );
      } else {
        lcd.print ( "    Normale" );
      }

      is_displaying_slot_2 = false;
      is_displaying_slot_3 = true;
    }
  } else if ( ( btn_press_time >= BTN_INTERVAL_4_TIME ) && (btn_press_time < BTN_INTERVAL_5_TIME) ) {
    // slot 4 
    if ( is_displaying_slot_4 == false ) {
      lcd.clear();
      lcd.print ( " -> Versione" );

      is_displaying_slot_3 = false;
      is_displaying_slot_4 = true;
    }
  } else if ( ( btn_press_time >= BTN_INTERVAL_5_TIME ) && (btn_press_time < BTN_INTERVAL_6_TIME) ) {
    // slot 5
    if ( is_displaying_slot_5 == false ) {
      lcd.clear();
      lcd.print ( " -> Parametri" );
      lcd.setCursor ( 0, 1 );
      lcd.print ( "    Serbatoio" );

      is_displaying_slot_4 = false;
      is_displaying_slot_5 = true;
    }
  } else if( ( btn_press_time >= BTN_INTERVAL_6_TIME ) ) {
    // slot 6
    if ( is_displaying_slot_6 == false ) {
      lcd.clear();
      lcd.print ( " -> Esci Menu'" );

      is_displaying_slot_5 = false;
      is_displaying_slot_6 = true;
    }
  }
}

//!
//! \brief Performs the menu associated actions based on button press time.
//!
//! \param[in] btn_press_time Time since button is kept pressed.
//!
inline void do_menu_actions ( const uint32_t btn_press_time ) {
  if ( btn_press_time >= BTN_INTERVAL_6_TIME ) {
    // Slot 6
    button_interval_6_handle();
  } else if ( btn_press_time >= BTN_INTERVAL_5_TIME ) {
    // Slot 5
    button_interval_5_handle();
  } else if ( btn_press_time >= BTN_INTERVAL_4_TIME ) {
    // Slot 4
    button_interval_4_handle();
  } else if ( btn_press_time >= BTN_INTERVAL_3_TIME ) {
    // Slot 3
    button_interval_3_handle();
  } else if ( btn_press_time >= BTN_INTERVAL_2_TIME ) {
    // Slot 2      
    button_interval_2_handle();
  } else if ( btn_press_time >= BTN_INTERVAL_1_TIME ) {
    // Slot 1      
    button_interval_1_handle();
  }
}

//!
//! \brief Manages the button events.
//!
inline void manage_button ( void ) {
  // read button status
  btn_status = static_cast < byte > ( digitalRead ( LCD_BUTTON_DPIN ) );

  // get current time
  const uint32_t timestamp_now = millis();
  
  if ( ( btn_status == HIGH ) && ( last_btn_status == LOW ) ) {
    // Detect the RISING EDGE 
    last_btn_status     = HIGH;
    timestamp_btn_press = timestamp_now;
    
  } else if ( ( btn_status == HIGH ) && ( last_btn_status == HIGH ) ) {
    // Detect when kept PRESSED
    
    // show options
    const uint32_t btn_press_time = ( timestamp_now - timestamp_btn_press );

    show_menu_options ( btn_press_time );
    
  } else if ( ( btn_status == LOW ) && ( last_btn_status == HIGH ) ) {
    // Detect the FALLING EDGE

    // reset display status
    is_displaying_slot_1     = false ;
    is_displaying_slot_2     = false ;
    is_displaying_slot_3     = false ;
    is_displaying_slot_4     = false ;
    is_displaying_slot_5     = false ;
    is_displaying_slot_6     = false ;
        
    // do actions
    const uint32_t btn_press_time = ( timestamp_now - timestamp_btn_press );

    do_menu_actions ( btn_press_time );

    last_btn_status     = LOW;
    timestamp_btn_press = 0;
  } else {
    // Reset all data when RELEASED.
    
    last_btn_status     = LOW;
    timestamp_btn_press = 0;
  }
}

// Setup function

void setup ( void ) {

  // initialize variables
  initialize();

  // initialize median filter
  filter.begin ( FILTER_SEED );

  // initialize consumption statistics
  stats.begin();

  // initialize the LCD
  
  // configure LCD size
  lcd_if.begin ( LCD_COLS, LCD_ROWS );

  // add special symbols

  byte lcd_progress_array [ LCD_CHAR_SIZE ];
  for ( int32_t i = 0;  i < LCD_CHAR_SIZE; i++ ) {
    lcd_progress_array [ i ] = LCD_PROGRESS [ i ];
  }
  lcd_if.createChar ( PROGRESS_CHAR, lcd_progress_array );

  lcd.begin(&lcd_if);
  
  // compute tank parameters
  tank_capacity     = ( TANK_RADIUS_CM * TANK_RADIUS_CM * PI * WATER_MAX_HEIGHT_CM ) / CM3_PER_LITER;
  maximum_capacity  = TANK_NUMBER * tank_capacity;
  
  // pin setup
  pinMode ( ECHO_DPIN,         INPUT  );
  pinMode ( TRIG_DPIN,         OUTPUT );
  pinMode ( LED_CAPACITY_DPIN, OUTPUT );
  pinMode ( LCD_LIGHT_DPIN,    OUTPUT );
  pinMode ( LCD_BUTTON_DPIN,   INPUT  );

  delay ( 1000 );

  // do autotest
  autotest();

  // turn on LCD backlight
  digitalWrite ( LCD_LIGHT_DPIN, HIGH );

  // print SW information
  lcd.print ( "Avvio..." );

  delay ( 1000 );
  
  show_version();

  // initialize status variables
  first_measure_done = false; // First measure to do
  in_debug           = false; // NORMAL mode
  led_on             = false; // LED off
  led_status         = false; // LED not blinking

  // initialize timestamps
  const uint32_t timestamp_now = millis();

  timestamp_lcd_on         = timestamp_now;
  timestamp_measurement    = timestamp_now;
  timestamp_last_led_ctrl  = timestamp_now;
  timestamp_last_read_dist = timestamp_now;
  timestamp_last_filling   = timestamp_now;

#if DEBUG
  Serial.begin ( 9600 );
  Serial.print ( "Tank Radius (cm): " );
  Serial.println ( TANK_RADIUS_CM );
  
  Serial.print ( "Water Max Height (cm): " );
  Serial.println ( WATER_MAX_HEIGHT_CM );
  
  Serial.print ( "Maximum Capacity (L): " );
  Serial.println ( maximum_capacity );
#endif

  delay ( 1000 );
  
  lcd.clear();
  lcd.print ( "Prima misura..." );
}

// Loop function

void loop ( void ) {

  // read current milliseconds passed from boot.
  const uint32_t timestamp_now = millis();

  // consumption time management
  stats.updateTime();

  // button management
  manage_button();

  lcd.update_lcd();
  
  // lcd light timer
  if ( ( timestamp_now - timestamp_lcd_on ) >= LCD_ON_TIME ) {
    turn_off_lcd_light();
  }

  // measurement timer
  if ( ( timestamp_now - timestamp_measurement ) >= measure_interval ) {
    distance = measure_level();
    if ( compare_float ( distance, -1, 0.1 ) == 1 ) {
      // measure error
      print_error();
      delay ( READ_DISTANCE_TIME );

      // skip to next cycle
      return;
    }

    // feed the filter
    ( void ) filter.in ( static_cast < int16_t > ( round_float_value ( distance ) ) );
  }

  // read distance timer
  if ( ( timestamp_now - timestamp_last_read_dist ) >= READ_DISTANCE_TIME ) {

    // get median value from filter
    distance = filter.out();

    // sanitize the value
    distance = sanitize_data ( 
      distance, 
      SENSOR_DISTANCE_CM, 
      ( WATER_MAX_HEIGHT_CM + SENSOR_DISTANCE_CM ), 
      SENSOR_DISTANCE_CM, 
      ( WATER_MAX_HEIGHT_CM + SENSOR_DISTANCE_CM )
    );

    if ( first_measure_done == true ) {

      // compute the current water volume
      float64_t liters = compute_liters ( distance );
      liters = sanitize_data ( liters, 0.0, maximum_capacity, 0.5, maximum_capacity );

      // update needed if previous message was error or volume was different
      if( ( compare_float ( previous_liters, liters, 0.1 ) == 0 ) || ( was_error == true ) ) {
        must_update_lcd = true;
        was_error       = false;
      }
      previous_liters = liters;

      // compute the percentage
      percentage = compute_percentage ( liters );

      // update statistics
      stats.updateConsumption ( static_cast< uint32_t > ( liters ) );

      // update LCD if not in debug
      if ( in_debug == false ) {       
        update_lcd ( percentage, liters );
      }
    }

    // update the timestamp for next value
    timestamp_last_read_dist = timestamp_now;
  }

  // led control timer
  if ( ( timestamp_now - timestamp_last_led_ctrl ) >= LED_CONTROL_TIME ) {
    control_led ( percentage ); 
    timestamp_last_led_ctrl = timestamp_now;
  }

  // go to sleep
  delay ( SLEEP_TIME );
}
