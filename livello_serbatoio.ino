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
#define VERSION "v0.12"    //!< Version tag
#define BANNER  "GR25"     //!< Author and year of build

#define CONF_DEBUG   1    //!< Constant used to compile in DEBUG mode (Serial enabled).
#define CONF_RELEASE 0    //!< Constant used to compile in RELEASE mode (Serial disabled).

#define SENSOR_HCSR04   0 //!< Constant used to compile code for HC-SR04 ultrasonic sensor.
#define SENSOR_JSNSR04T 1 //!< Constant used to compile code for JSN-SR04T ultrasonic sensor (waterproof).

// Configuration
#ifndef UNIT_TEST
# define DEBUG  CONF_RELEASE   //!< Current configuration
# define SENSOR SENSOR_HCSR04  //!< Current sensor
#endif

///
/// \defgroup TypesGroup Custom types
/// @{
typedef double float64_t;
/// @}

///
/// \defgroup ConstGroup Constants
/// @{

///
/// \defgroup MedFilterGroup Median filter
/// @{
const int16_t     FILTER_SEED           = 0                                   ; //!< Initial value for Median Filter.
const int16_t     FILTER_SIZE           = 5                                   ; //!< Median Filter size.
/// @}

///
/// \defgroup PinGroup Pins used
/// @{
const int16_t     ECHO_DPIN             = 12                                  ; //!< Digital pin for ultrasonic sensor ECHO pin.
const int16_t     TRIG_DPIN             = 11                                  ; //!< Digital pin for ultrasonic sensor TRIG pin.
const int16_t     LED_CAPACITY_DPIN     = 13                                  ; //!< Digital pin for LED.
const int16_t     LCD_BUTTON_DPIN       = 2                                   ; //!< Digital pin for Button.
const int16_t     LCD_LIGHT_DPIN        = 9                                   ; //!< Digital pin for LCD backlight.
/// @}

///
/// \defgroup TankGroup Tank and volume
/// @{
const uint16_t    TANK_NUMBER           = 2U                                  ; //!< The number of tanks.
const float64_t   TANK_RADIUS_CM        = 65.0 / 2.0                          ; //!< The radius of the tank(s) in cm.
const float64_t   TANK_HEIGHT_CM        = 150.0                               ; //!< The height of the tank(s) in cm.
const float64_t   SENSOR_DISTANCE_CM    = 19.0                                ; //!< The distance between sensor and the highest water level in cm.
const float64_t   WATER_MAX_HEIGHT_CM   = TANK_HEIGHT_CM - SENSOR_DISTANCE_CM ; //!< The maximum water height in cm.
const float64_t   LOW_LEVEL_THRESHOLD   = 40.0                                ; //!< The percentage threshold used to indicate the low level of water.
const float64_t   EMPTY_LEVEL_THRESHOLD = 20.0                                ; //!< The percentage threshold used to indicate the tank(s) emptiness.
/// @}

///
/// \defgroup ConversionGroup Conversion units
/// @{
const float64_t   CM3_PER_LITER         = 1000.0                              ; //!< Constant used to convert liters in cm^3 (1 l = 1000 cm^3).
/// @}

///
/// \defgroup TimerGroup Timers constants
/// @{
const uint32_t    MEASURE_LF_INTERVAL   = 30 * 1000                           ; //!< Inteval between two measurements (low frequency) in milliseconds.
const uint32_t    MEASURE_HF_INTERVAL   =  1 * 1000                           ; //!< Inteval between two measurements (high frequency) in milliseconds.
const uint32_t    LCD_ON_TIME           = 30 * 1000                           ; //!< LCD backlight duration in milliseconds.
const uint32_t    SLEEP_TIME            =  100                                ; //!< Sleep time between two execution of #loop in milliseconds.
const uint32_t    SHOW_STAT_TIME        = 5000                                ; //!< Duration for visualization of Statistics in milliseconds.
const uint32_t    SHOW_PARAM_TIME       = 5000                                ; //!< Duration for visualization of Tank Parameters in milliseconds.
const uint32_t    SHOW_VERSION_TIME     = 3000                                ; //!< Duration for visualization of SW Version in milliseconds.
const uint32_t    LED_CONTROL_TIME      = 1000                                ; //!< Period of LED blinking in milliseconds.
const uint32_t    READ_DISTANCE_TIME    = 1000                                ; //!< Period of distance reading from Median Filter in milliseconds.
const uint32_t    BTN_INTERVAL_1_TIME   =  200                                ; //!< Duration to trig Slot 1 of menu in milliseconds.
const uint32_t    BTN_INTERVAL_2_TIME   = 2000                                ; //!< Duration to trig Slot 2 of menu in milliseconds.
const uint32_t    BTN_INTERVAL_3_TIME   = 3000                                ; //!< Duration to trig Slot 3 of menu in milliseconds.
const uint32_t    BTN_INTERVAL_4_TIME   = 4000                                ; //!< Duration to trig Slot 4 of menu in milliseconds.
const uint32_t    BTN_INTERVAL_5_TIME   = 5000                                ; //!< Duration to trig Slot 5 of menu in milliseconds.
const uint32_t    BTN_INTERVAL_6_TIME   = 6000                                ; //!< Duration to trig Slot 6 of menu in milliseconds.
/// @}

///
/// \defgroup SensorGroup Sensor parameters
/// Constants related to the ultrasonic sensors.
/// @{
#if SENSOR == SENSOR_JSNSR04T

const float64_t   SENSOR_CALIBRATION    = 1.7                                 ; //!< Sensor distance offset used for calibration in centimeters.
const float64_t   SENSOR_MIN_RANGE      = 20.0                                ; //!< Sensor minimum readable distance in centimeters.
const float64_t   SENSOR_MAX_RANGE      = 600.0                               ; //!< Sensor maximum readable distance in centimeters.
const float64_t   SENSOR_LSB            = 58.0                                ; //!< Sensor LSB to achieve distance from ECHO pulse time in usec/cm.
const int32_t     SENSOR_ACTIVATION_TH  = 15                                  ; //!< Duration of TRIG signal to perform a measurement.

const uint32_t    SENSOR_RESPONSE_TIMEOUT_US = 500000UL                       ; //!< Timeout in microseconds used to determine when the sensor reading is failed.
const uint32_t    SENSOR_NO_OBSTACLE_US      = 38000UL                        ; //!< ECHO pulse time in microseconds which indicates the absence of sensor readable obstacles.

#elif SENSOR == SENSOR_HCSR04

const float64_t   SENSOR_CALIBRATION    = 1.0                                 ; //!< Sensor distance offset used for calibration in centimeters.
const float64_t   SENSOR_MIN_RANGE      = 2.0                                 ; //!< Sensor minimum readable distance in centimeters.
const float64_t   SENSOR_MAX_RANGE      = 450.0                               ; //!< Sensor maximum readable distance in centimeters.
const float64_t   SENSOR_LSB            = 58.0                                ; //!< Sensor LSB to achieve distance from ECHO pulse time in usec/cm.
const int32_t     SENSOR_ACTIVATION_TH  = 15                                  ; //!< Duration of TRIG signal to perform a measurement.

const uint32_t    SENSOR_RESPONSE_TIMEOUT_US = 500000UL                       ; //!< Timeout in microseconds used to determine when the sensor reading is failed.
const uint32_t    SENSOR_NO_OBSTACLE_US      = 38000UL                        ; //!< ECHO pulse time in microseconds which indicates the absence of sensor readable obstacles.

#else

const float64_t   SENSOR_CALIBRATION    = 0.0                                 ; //!< Sensor distance offset used for calibration in centimeters.
const float64_t   SENSOR_MIN_RANGE      = 0.0                                 ; //!< Sensor minimum readable distance in centimeters.
const float64_t   SENSOR_MAX_RANGE      = 0.0                                 ; //!< Sensor maximum readable distance in centimeters.
const float64_t   SENSOR_LSB            = 1.0                                 ; //!< Sensor LSB to achieve distance from ECHO pulse time in usec/cm.
const int32_t     SENSOR_ACTIVATION_TH  = 15                                  ; //!< Duration of TRIG signal to perform a measurement.

const uint32_t    SENSOR_RESPONSE_TIMEOUT_US = 500000UL                       ; //!< Timeout in microseconds used to determine when the sensor reading is failed.
const uint32_t    SENSOR_NO_OBSTACLE_US      = 38000UL                        ; //!< ECHO pulse time in microseconds which indicates the absence of sensor readable obstacles.

#endif
/// @}

///
/// \defgroup LCDGroup LCD Constants
/// Constants related to the LCD.
/// @{

// LCD Dimensions
const int32_t     LCD_COLS              = 16                                  ; //!< Number of columns of LCD.
const int32_t     LCD_ROWS              =  2                                  ; //!< Number of rows of LCD.

//LCD Special Characters
const byte        PROGRESS_CHAR         = 1                                   ;                      //!< Index of special character used for progress bar.
const byte        ARROW_UP_CHAR         = 2                                   ;                      //!< Index of special character used for arrow up.
const byte        ARROW_DOWN_CHAR       = 3                                   ;                      //!< Index of special character used for arrow downn.
const int32_t     LCD_CHAR_SIZE         = 8                                   ;                      //!< Size of special character definition array.
const byte        LCD_PROGRESS[LCD_CHAR_SIZE]   = { 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F };//!< Definition of special character used for progress bar.
const byte        LCD_ARROW_DOWN[LCD_CHAR_SIZE] = { 0x00, 0x00, 0x00, 0x1F, 0x0E, 0x04, 0x00, 0x00 };//!< Definition of special character used for arrow down.
const byte        LCD_ARROW_UP[LCD_CHAR_SIZE]   = { 0x00, 0x00, 0x04, 0x0E, 0x1F, 0x00, 0x00, 0x00 };//!< Definition of special character used for arrow up.


// LCD pins
const int16_t     RS                    = 3                                   ; //!< LCD's RS digital pin.
const int16_t     E                     = 4                                   ; //!< LCD's E digital pin.
const int16_t     DB4                   = 5                                   ; //!< LCD's DB4 digital pin.
const int16_t     DB5                   = 6                                   ; //!< LCD's DB5 digital pin.
const int16_t     DB6                   = 7                                   ; //!< LCD's DB6 digital pin.
const int16_t     DB7                   = 8                                   ; //!< LCD's DB7 digital pin.
/// @}

///
/// \defgroup ErrCodesGroup Error Codes
/// Error codes used to determine the status in DEBUG mode.
/// @{
const int32_t     ERR_OK                = 0                                   ; //!< All ok. No error occurred.
const int32_t     ERR_SENS              = 1                                   ; //!< Sensor reading error.
const int32_t     ERR_RANGE             = 2                                   ; //!< Sensor reading out of range.
const int32_t     ERR_STAT              = 3                                   ; //!< Consumption calculation error.
const int32_t     ERR_NOISY             = 4                                   ; //!< Noisy error.
/// @}

/// @}

/// \defgroup VarsGroup Global variables
/// @{

/// \defgroup StatusGroup Status flags
/// Status variables.
/// @{
static volatile bool        led_on                    ;  //!< Indicates if LED is turned on (true) or off (false).
static volatile bool        led_status                ;  //!< Indicates if LED is on or blinking (true) or off (false).
static volatile bool        in_debug                  ;  //!< Indicates if the SW is in DEBUG mode (true) or NORMAL mode (false).
static volatile bool        first_measure_done        ;  //!< Establishes if the first measurements used to feed the Median Filter have been done (true) or not (false).
static volatile byte        last_btn_status           ;  //!< Indicates if the button was pressed (HIGH) of not (LOW), to detect long pressure.
static volatile int32_t     err_code                  ;  //!< Contains the current error code (see \ref ErrCodesGroup).
/// @}

/// \defgroup TimestampsGroup Timestamps
/// The timestamps used to implement non-blocking timers.
/// @{
static volatile uint32_t    timestamp_lcd_on          ;  //!< Timestamp of the last LCD backlight activation in milliseconds.
static volatile uint32_t    timestamp_measurement     ;  //!< Timestamp of the last reading from Median Filter in milliseconds.
static volatile uint32_t    timestamp_last_led_ctrl   ;  //!< Timestamp of the last LED activation in milliseconds.
static volatile uint32_t    timestamp_last_read_dist  ;  //!< Timestamp of the last sensor reading in milliseconds.
static volatile uint32_t    timestamp_btn_press       ;  //!< Timestamp of the last button pressing in milliseconds.
/// @}

/// \defgroup TimingGroup Timings
/// The timing settings (for variable frequency timers).
/// @{
static uint32_t             measure_interval          ;  //!< Contains the current sensor reading period (based on current SW state) in milliseconds.
/// @}


/// \defgroup TankParamsCompGroup Computed tank(s) parameters
/// The computed paramaters based on the currently compiled tank data.
/// @{
static float64_t            tank_capacity             ; //!< The capacity of one tank in liters.
static float64_t            maximum_capacity          ; //!< The capacity of all tanks summed in liters (assume all tanks equals).
/// @}

/// \defgroup ReadingGroup Current reads
/// The last reading status.
/// @{
static uint16_t             number_measures_done      ; //!< The number of measures done during initialization phase.  
static float64_t            percentage                ; //!< The current water level in percentage.
static volatile float64_t   distance                  ; //!< The last distance read from sensor (in centimeters).
static volatile float64_t   previous_liters           ; //!< The previous capacity read in liters.
/// @}

/// @}

//!
//! \brief Helper class to write on LCD.
//! \details The purpose is to try the optimization of writings on LCD to prevent unwanted screen flickering.
//!          It uses a buffer to keep the current LCD content. When the user writes using the "print" or "write" methods,
//!          data are written in the buffer. The user has in charge the responsibility to update the LCD view by 
//!          explicitly invoking the "update_lcd" method. The latter verifies which characters have been changed from previous view and
//!          updates only the different ones.
//!
class LcdHelper {
public:

  //!
  //! \brief Class default constructor.
  //!
  LcdHelper ( void ) {

    // NULL instance
    this->lcd_instance = nullptr;

    // LCD backlight not controllable
    this->backlight_digital_pin = -1;
  }

  //!
  //! \brief Initializes this instance with current LCD instance.
  //!
  //! \param[in] lcd The pointer to the LiquidCrystal lcd instance.
  //! \param[in] backlight_dpin The digital pin number used to control LCD backlight (default: -1, if not controllable).
  //!
  void begin ( LiquidCrystal * const lcd, const int32_t backlight_dpin = -1 ) {

    // check null pointer
    if ( lcd != nullptr ) {

      // assign passed data to internal members
      this->lcd_instance = lcd;
      this->backlight_digital_pin = backlight_dpin;

      // initializes all members
      for ( uint16_t i = 0U; i < LCD_ROWS; i++ ) {
        for ( uint16_t j = 0U; j < LCD_COLS; j++ ) {
           this->lcd_matrix[i][j] = EMPTY_CHAR;
           this->last_lcd_matrix[i][j] = EMPTY_CHAR;
           this->special_char_map[i][j] = false;
           this->last_special_char_map[i][j] = false;
        }
      }

      // set cursor to initial position
      this->home();

      // clear LCD
      this->lcd_instance->clear();
    }
  }

  //!
  //! \brief Turns on the LCD backlight if configured.
  //! 
  void turn_on_backlight ( void ) {
    // check if configured
    if ( this->backlight_digital_pin != -1 ) {

      // Turn on light
      digitalWrite ( this->backlight_digital_pin, HIGH );
    }
  }

  //!
  //! \brief Turns off the LCD backlight if configured.
  //! 
  void turn_off_backlight ( void ) {
    // check if configured
    if ( this->backlight_digital_pin != -1 ) {

      // Turn off light
      digitalWrite ( this->backlight_digital_pin, LOW );
    }
  }

  //!
  //! \brief Clears the current LCD content (the internal buffer) and resets the cursor position.
  //! 
  void clear ( void ) {    
    for ( uint16_t i = 0U; i < LCD_ROWS; i++ ) {
      for ( uint16_t j = 0U; j < LCD_COLS; j++ ) {

        // reset the internal buffer 
        this->lcd_matrix[i][j] = EMPTY_CHAR;
        this->special_char_map[i][j] = false;
      }
    }

    // reset the cursor
    this->home();
  }

  //!
  //! \brief Sets the cursor position if valid (in LCD sizes).
  //!
  //! \param[in] c The column index.
  //! \param[in] r The row index.
  //!
  void setCursor ( const uint16_t c, const uint16_t r ) {

    // check the LCD sizes
    if ( ( c < LCD_COLS ) && ( r < LCD_ROWS ) ) {

      // set the cursor
      this->cursor_c = c;
      this->cursor_r = r;
    }
  }

  //!
  //! \brief Sets the cursor position to (0, 0).
  //!
  void home ( void ) {
    this->setCursor ( 0U, 0U );
  }

  //!
  //! \brief Prints a string to the internal buffer without exceeding the limits of the LCD.
  //!
  //! \param[in] str The string (char pointer).
  //! \param[in] str_length The string length.
  //!
  void print ( const char * str, const uint16_t str_length = LCD_COLS ) {
    for ( uint16_t str_i = 0U; str_i < str_length; str_i ++ ) {
      // if terminator has been reached than exit
      if ( str[str_i] == '\0' ) {
        break;
      }

      // check if we are in LCD boundaries
      if ( this->cursor_c < LCD_COLS ) {

        // write the current character
        this->lcd_matrix[this->cursor_r][this->cursor_c] = str[str_i];

        // shift the cursor to thr right by 1
        this->cursor_c ++;
      }
    }
        
  }

  //!
  //! \brief Prints a number to the LCD.
  //!
  //! \param[in] number The integer number to write.
  //!
  void print ( const int32_t number ) {
    // convert the number to string
    const String number_str = String ( number );

    // write the string
    this->print ( number_str.c_str() );
  }


  //!
  //! \brief Prints a String to the LCD.
  //!
  //! \param[in] str The string to write.
  //!
  void print ( const String& str ) {
    this->print ( str.c_str() );
  }

  //!
  //! \brief Prints a integer number to the LCD.
  //!
  //! \param[in] number The number to write.
  //!
  void print ( const int16_t number ) {
    // convert the number to string
    const String number_str = String ( number );

    // write the string
    this->print ( number_str.c_str() );
  }

  //!
  //! \brief Prints a integer number to the LCD.
  //!
  //! \param[in] number The number to write.
  //!
  void print ( const uint16_t number ) {
    // convert the number to string
    const String number_str = String ( number );

    // write the string
    this->print ( number_str.c_str() );
  }

  //!
  //! \brief Prints a floating point number to the LCD.
  //!
  //! \param[in] number The number to write.
  //!
  void print ( const float64_t number ) {
    // convert the number to string
    const String number_str = String ( number );

    // write the string
    this->print ( number_str.c_str() );
  }

  //!
  //! \brief Writes a special character to the LCD.
  //! 
  //! \details It writes the number on the buffer and set to true the #special_char_map matrix in the corresponding position on screen.
  //!
  //! \param[in] special_char_index The special character number.
  //!
  void write ( const int32_t special_char_index ) {
    // check if the special character is in 0-9 range
    if ( ( special_char_index >= 0 ) && ( special_char_index <= 9 ) ) {
      // flag position as "special character"
      this->special_char_map[this->cursor_r][this->cursor_c] = true;

      // print the number
      this->print ( special_char_index );
    }
  }

  //!
  //! \brief Updates the LCD HW with the current written data.
  //!
  void update_lcd ( void ) {
    // check if an update is needed and if the LCD instance is valid
    if ( ( this->need_update() == true ) && ( this->lcd_instance != nullptr ) ) {
      // write the different chars
      for ( uint16_t i = 0U; i < LCD_ROWS; i++ ) {
        for ( uint16_t j = 0U; j < LCD_COLS; j++ ) {
          if ( ( this->lcd_matrix[i][j] != this->last_lcd_matrix[i][j]             ) ||
               ( this->special_char_map[i][j] != this->last_special_char_map[i][j] )    ) {
            // if the content is changed with respect to displayed data

            // set the cursor to the correct position
            this->lcd_instance->setCursor(j, i);

            // if it is a special symbol
            if ( this->is_special_symbol ( i, j ) == true ) {

              // Write the special symbol (if configured)
              this->lcd_instance->write ( static_cast<byte> ( String ( this->lcd_matrix[i][j] ).toInt() ) );
            } else {
              // else if a normal character, write it
              this->lcd_instance->print ( String ( this->lcd_matrix[i][j] ) );
            }

            // update the current LCD HW content for next writing
            this->last_lcd_matrix[i][j] = this->lcd_matrix[i][j];
            this->last_special_char_map[i][j] = this->special_char_map[i][j];
          }
        }
      }      
    }
  }

private:

  //!
  //! \brief Checks if the symbol in the given position is special or not.
  //!
  //! \details Checks if the symbol is a number in 0-9 and it flagged true in #special_char_map.
  //!
  //! \param[in] r The row.
  //! \param[in] c The column.
  //!
  //! \return true if the symbol in (#r, #c) is special or false otherwise.
  //!
  bool is_special_symbol ( const uint16_t r, const uint16_t c ) {
    bool is_ss = false;

    // check if cursor position is valid
    if ( ( r < LCD_ROWS ) && ( c < LCD_COLS ) ) {

      // get the symbol
      char symbol = this->lcd_matrix[r][c];

      // check if is special
      if ( ( symbol >= '0' ) && ( symbol <= '9' ) && ( this->special_char_map[r][c] == true ) ) {
        is_ss = true;
      }
    }
    return is_ss;
  }

  //!
  //! \brief Checks if the LCD HW view needs an update.
  //!
  //! \details Checks if there is a symbol different from the currently displayed data
  //!
  //! \return true if there is a difference to display or false otherwise.
  //!
  bool need_update ( void ) {
    bool is_update_needed = false;
    
    for ( uint16_t i = 0U; i < LCD_ROWS; i++ ) {
      for ( uint16_t j = 0U; j < LCD_COLS; j++ ) {
        // check if it is different
        if ( this->last_lcd_matrix[i][j] != this->lcd_matrix[i][j] ) {
          is_update_needed = true;
          break;
        }
      }
    }

    return is_update_needed;
  }

  uint16_t cursor_r; //<! The current cursor position in row.
  
  uint16_t cursor_c; //<! The current cursor position in column.

  int32_t backlight_digital_pin; //<! The current digital pin for LCD backlight.

  static const char EMPTY_CHAR = ' '; //<! The empty character for LCD.
  
  char lcd_matrix[LCD_ROWS][LCD_COLS]; //<! The internal buffer containing data to display.

  char last_lcd_matrix[LCD_ROWS][LCD_COLS]; //<! The current LCD displayed content.

  bool special_char_map[LCD_ROWS][LCD_COLS]; //<! The special characters map.

  bool last_special_char_map[LCD_ROWS][LCD_COLS]; //<! The currently displayed special characters.

  LiquidCrystal *lcd_instance; //<! The configured LiquidCrystal instance.
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
  byte    medFilterWin;                //!< Samples number in the sliding window of the filter - usually an odd value.
  byte    medDataPointer;              //!< Central point of the filter sliding window.
  int16_t data        [FILTER_SIZE];   //!< Array of data ordered by age (Circular buffer).
  byte    sizeMap     [FILTER_SIZE];   //!< Array used to store data locations ordered by size.
  byte    locationMap [FILTER_SIZE];   //!< Array used to store data locations in the map.
  byte    oldestDataPoint;             //!< Position of the oldest value in the circular buffer.
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
  //! \param[in] current_liters The water volume in the tank(s).
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
  //! \param[in] back_time The hours passed.
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

  static const uint16_t STAT_SIZE         = (24U * 3U) + 1U; //!< The sample dimension (3 days).
  static const uint32_t LITERS_THRESHOLD  = 15U;             //!< Minimum consumption threshold to be inserted as sample.

  static const uint32_t ONE_SECOND_MS     = 1000U;           //!< 1 second constant.
  static const uint32_t ONE_HOUR_S        = (60U * 60U);     //!< 1 hour constant.
  
  uint32_t index                          ;        //!< Buffer current index.
  uint32_t consumption_samples[STAT_SIZE] ;        //!< The buffer with samples. 
  volatile uint32_t last_liters           ;        //!< The last known volume of water in the tank (in liters).
  volatile uint32_t consumption           ;        //!< The last hour water consumption.

  volatile uint32_t last_millis           ;        //!< Last timestamp (value from power-on).
  volatile uint32_t millis_passed         ;        //!< Passed milliseconds from last invocation of #updateTime.
  volatile uint32_t seconds_passed        ;        //!< Passed seconds from last invocation of #updateTime.
};

//!
//! @brief Analyzes the readings from distance sensor to determine if the water level is increasing, decreasing or stationary.
//!        Permits also to detect sensor failure if the readings appear to be random.
//!
class ReadingsAnalyzer 
{
public:
  //!
  //! \brief Default constructor.
  //!
  //! \details Creates and instance of this class.
  //! 
  //! \return An instance of this class to be initialized with #begin method.
  //!
  ReadingsAnalyzer ( void ) {
  }

  //!
  //! \brief Initializes this instance.
  //!
  void begin ( void ) {
    lastReading = 0.0;
    
    nDecreasing = 0U;
    nIncreasing = 0U;
    isNoisy     = false;
  }

  //!
  //! \brief Adds a measurement to the statistic.
  //! 
  //! \param[in] reading The distance measurement.
  //!
  void addReading ( const float64_t reading ) {
    if ( lastReading != 0.0 ) {

      isNoisy = false;

      const float64_t diffReading = reading - lastReading;
      if ( fabs(diffReading) > 5. ) {
        // noisy
        isNoisy = true;
      } else if ( diffReading > 0.2 ) {
        // decreasing
        nDecreasing = increase ( nDecreasing );
        nIncreasing = decrease ( nIncreasing );
      } else if ( diffReading < -0.2 ) {
        // increasing
        nDecreasing = decrease ( nDecreasing );
        nIncreasing = increase ( nIncreasing );
      } else {
        nDecreasing = decrease ( nDecreasing );
        nIncreasing = decrease ( nIncreasing );
      }
    }
    
    lastReading = reading;
  }

  //!
  //! \brief Checks the measurements and gives a response.
  //!
  //! \details Checks which events occurred more times.
  //!
  //! \return The analysis result which can be any of the following:
  //!         #UNKNOWN, #STATIONARY, #DECREASING, #INCREASING, #NOISY.
  //!
  uint8_t getAnalysis ( void ) {
    uint8_t result = UNKNOWN;

    if ( isNoisy == true )
    {
      result = NOISY;
    }
    else if ( nIncreasing < nDecreasing)
    {
      result = DECREASING;
    }
    else if ( nIncreasing > nDecreasing)
    {
      result = INCREASING;
    }
    else
    {
      result = STATIONARY;
    }

#if DEBUG
    // print in DEBUG mode
    Serial.print ( " N: " ); Serial.print ( isNoisy     );
    Serial.print ( " D: " ); Serial.print ( nDecreasing );
    Serial.print ( " I: " ); Serial.print ( nIncreasing );
    Serial.print ( " => " );
    switch ( result ) {
      case NOISY      : { Serial.println("noisy")     ; } break;
      case DECREASING : { Serial.println("decreasing"); } break;
      case INCREASING : { Serial.println("increasing"); } break;
      case STATIONARY : { Serial.println("stationary"); } break;
      default         : { Serial.println("unknown")   ; } break;
    }
#endif

    return result;
  }

  static const uint8_t UNKNOWN    = 0U; //!< Unknown analysis result (initialization value).
  static const uint8_t STATIONARY = 1U; //!< Water level is stationary.
  static const uint8_t DECREASING = 2U; //!< Water level is decreasing.
  static const uint8_t INCREASING = 3U; //!< Water level is increasing.
  static const uint8_t NOISY      = 4U; //!< Water level is not stable (possible sensor fault).

private:
  float64_t lastReading; //!< Last water measurement.

  uint16_t nDecreasing; //!< Number of "decreasing" readings.
  uint16_t nIncreasing; //!< Number of "increasing" readings.
  bool     isNoisy;

  static inline uint16_t increase ( const uint16_t val, const uint16_t by = 1U )
  {
    uint16_t retValue = 0U;

    // overflow detection
    if ( ( val + by ) < val )
    {
      retValue = UINT16_MAX;
    }
    else
    {
      retValue = val + by;
    }

    return retValue;
  }

  static inline uint16_t decrease ( const uint16_t val, const uint16_t by = 1U )
  {
    uint16_t retValue = 0U;

    if (by < val)
    {
      retValue = val - by;
    }

    return retValue;
  }
};

/// \defgroup ClassInstGroup Instances of the classes
/// @{

//Format (RS, E, DB4, DB5, DB6, DB7)
static LiquidCrystal lcd_if ( RS, E, DB4, DB5, DB6, DB7 ); //!< The instance of LCD HW.

static LcdHelper lcd; //!< The LCD helper instance.

static MedianFilter filter; //!< The Median Filter instance.

static ConsumptionData stats; //!< The Consumption Data instance.

static ReadingsAnalyzer analyzer; //!< Readings analyzer instance

/// @}

/// \defgroup SWFuncGroup SW Functions
/// @{

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
  first_measure_done       = false               ;
  err_code                 = ERR_OK              ;
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
  timestamp_last_read_dist = 0                   ; // ms 
  timestamp_btn_press      = 0                   ; // ms
  timestamp_last_led_ctrl  = 0                   ; // ms
}

//!
//! \brief Computes the volume of water in the tank.
//!
//! \details See below for details on computation.
//!           
//! <pre>
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
//! </pre>
//!
//! \param[in] read_distance The water distance from sensor.
//!
//! \return The volume in liters.
//!
inline float64_t compute_liters ( float64_t read_distance ) {

  if ( read_distance < 0.0 ) {
    
    // sanitize data below 0
    read_distance = 0.0;
    
  } else if ( read_distance > TANK_HEIGHT_CM ) {
    
    // sanitize data over tank height
    read_distance = WATER_MAX_HEIGHT_CM;
    
  } else if ( read_distance >= SENSOR_DISTANCE_CM ) {
    
    // remove sensor distance from water
    read_distance -= SENSOR_DISTANCE_CM;
    
  } else {
    
    // sanitize data less than sensor distance
    read_distance = 0.0;
    
  }

  // compute the volume
  const float64_t volume = TANK_NUMBER * ( ( TANK_RADIUS_CM * TANK_RADIUS_CM * PI * ( WATER_MAX_HEIGHT_CM - read_distance ) ) / CM3_PER_LITER );
  
  return volume;
}

//!
//! \brief Computes the percentage of filling of the tank.
//! 
//! \details It uses the most recent reading and the maximum capacity computed in #setup.
//!
//! \param[in] read_liters The volume of water read.
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
  lcd.turn_on_backlight();

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
  lcd.turn_off_backlight();
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
  }
}

//!
//! \brief Shows the current error code to LCD (only in debug mode).
//!
//! \details Implemented layout:
//! <pre>
//!          |----------------|
//!          |XXXXXXXXXXX e:XX|
//!          |XXXXXXXXXXXXXXXX|
//!          |----------------|
//! </pre>
//!
inline void show_err_code_debug ( void ) {
  if ( ( in_debug == true ) && ( last_btn_status == LOW ) ) {
    // if in debug mode and menu is not shown (the button in released)
    
    lcd.setCursor ( 12, 0 );
    lcd.print ( "e: " );
    lcd.setCursor ( 14, 0 );
    lcd.print ( err_code );

    lcd.update_lcd();
  }
}


//!
//! \brief Updates the LCD.
//!
//! \details Used only in debug mode.
//!  
//! \param[in] distance_to_print The distance from the water in centimeters.
//! \param[in] distance_comp The distance from water (calibrated value in centimeters).
//!
inline void update_lcd_debug ( const float64_t distance_to_print, const float64_t distance_comp ) {
  if ( in_debug == true ) {
    // if in debug mode and LCD needs an update

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

inline void monitorReadings ( void ) {
  const uint8_t result = analyzer.getAnalysis();

  if ( in_debug == false ) {
    lcd.setCursor ( 8, 0 );
    if ( result == ReadingsAnalyzer::NOISY ) {
      lcd.print ( "!" );
    } else if ( result == ReadingsAnalyzer::INCREASING ) {
      lcd.write ( ARROW_UP_CHAR );
    } else if ( result == ReadingsAnalyzer::DECREASING ) {
      lcd.write ( ARROW_DOWN_CHAR );
    } else {
      lcd.print ( "-" );
    }
  } else {
    if ( result == ReadingsAnalyzer::NOISY ) {
      // assign proper error code
      err_code = ERR_NOISY;
  
      // show error code if in debug
      show_err_code_debug();
    }
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

    analyzer.addReading ( distance_read );
    
    // apply calibration
    const float64_t dist_compensated = distance_read + SENSOR_CALIBRATION;
  
    if ( ( in_debug == true ) && ( last_btn_status == LOW ) ) {

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
//! <pre>
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
//! </pre>
//!  
//! \param[in] water_percentage The tank filling percentage.
//! \param[in] liters The water volume in liters.
//!
inline void update_lcd ( const float64_t water_percentage, const float64_t liters ) {
  bool must_update_lcd = true;
  if( ( water_percentage < 0.0 ) || ( water_percentage > 100.0 ) || ( liters < 0.0 ) || ( liters > 9999.9 ) ) {
    // if data are invalid, don't update the LCD
    must_update_lcd = false;
  }
  
  if ( must_update_lcd == true ) {
    // if the LCD must be updates

    // Clear the LCD
    lcd.clear();

    // Print unit symbols in specified locations
    lcd.setCursor ( 6, 0 );
    lcd.print ( "L" );
    lcd.setCursor ( 14, 0 );
    lcd.print ( "%" );

    // print the liters

    // determine the index of the first cipher of #liters to be written.
    uint16_t index = 4;
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
  lcd.update_lcd();

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
  lcd.update_lcd();

  // wait 3 second to let user to see result
  delay ( 3000 );

  // 4. Reset state
  digitalWrite ( LED_CAPACITY_DPIN, LOW );
  lcd.clear();
  lcd.update_lcd();

  // wait and back to normal
  delay ( 1000 );
}

//!
//! \brief Prints on LCD the consumption statistic passed.
//!
//! \details Follows the format:
//! <pre>
//!  |----------------| 
//!  |<      str     >|
//!  |Tot. <stat>   L |
//!  |----------------|
//! </pre>
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

  lcd.update_lcd();

  // wait to let user see the value
  delay ( SHOW_STAT_TIME );

  // clear all
  lcd.clear(); 
}

//!
//! \brief Prints the SW version on LCD.
//!
//! \details Follows the format:
//! <pre>
//!  |----------------| 
//!  | Versione:      |
//!  | <num_version>  |
//!  |----------------|
//! </pre>
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
  lcd.print ( BANNER );

  lcd.update_lcd();

  // wait to let the user see
  delay(SHOW_VERSION_TIME);

  // clear all
  lcd.clear();
}

//!
//! \brief Prints the tank parameters (diameter, number of tanks, max water heigh, max total capacity).
//!
//! \details Follows the format:
//! <pre>
//!  |----------------| 
//!  |D:xxxxx N:xx    |
//!  |H:xxxxx C:xxxxx |
//!  |----------------|
//! </pre>
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

  lcd.update_lcd();

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

  lcd.update_lcd();

  // wait to let user read
  delay(3000);

  // perform an autotest
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" 1. Autotest    ");

  lcd.update_lcd();

  // wait to let user read
  delay ( 1000 );

  // do autotest
  autotest();

  // perform real-time readings
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" 2. Lettura     ");

  lcd.update_lcd();

  // wait to let user read
  delay(1000);

  // change measure frequency to the highest value
  measure_interval = MEASURE_HF_INTERVAL; //ms

  // turn on LCD
  turn_on_lcd_light();
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

  lcd.update_lcd();

  // wait to let user read
  delay(3000);

  // change measure frequency to the lowest value
  measure_interval = MEASURE_LF_INTERVAL; //ms
}

//!
//! \brief Actions for sixth menu slot.
//!
inline void button_interval_6_handle ( void ) {
  // do nothing
}

//!
//! \brief Actions for fifth menu slot.
//!
inline void button_interval_5_handle ( void ) {
  show_params();
}

//!
//! \brief Actions for fourth menu slot.
//!
inline void button_interval_4_handle ( void ) {
  show_version();
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
    lcd.clear();
    lcd.print ( " -> Consumi " );
  } else if( ( btn_press_time >= BTN_INTERVAL_3_TIME ) && ( btn_press_time < BTN_INTERVAL_4_TIME ) ) {
    // slot 3
    lcd.clear();
    lcd.print ( " -> Modo" );
    lcd.setCursor ( 0, 1 );
    if ( in_debug == false ) {
      lcd.print ( "    Manutenzione" );
    } else {
      lcd.print ( "    Normale" );
    }
  } else if ( ( btn_press_time >= BTN_INTERVAL_4_TIME ) && (btn_press_time < BTN_INTERVAL_5_TIME) ) {
    // slot 4 
    lcd.clear();
    lcd.print ( " -> Versione" );
  } else if ( ( btn_press_time >= BTN_INTERVAL_5_TIME ) && (btn_press_time < BTN_INTERVAL_6_TIME) ) {
    // slot 5
    lcd.clear();
    lcd.print ( " -> Parametri" );
    lcd.setCursor ( 0, 1 );
    lcd.print ( "    Serbatoio" );
  } else if( ( btn_press_time >= BTN_INTERVAL_6_TIME ) ) {
    // slot 6
    lcd.clear();
    lcd.print ( " -> Esci Menu'" );
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
  const byte btn_status = static_cast < byte > ( digitalRead ( LCD_BUTTON_DPIN ) );

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

/// @}

/// \defgroup ArduinoFuncGroup Arduino functions
/// @{

//!
//! \brief Setup function (executed once).
//!
void setup ( void ) {
  // pin setup
  pinMode ( ECHO_DPIN,         INPUT  );
  pinMode ( TRIG_DPIN,         OUTPUT );
  pinMode ( LED_CAPACITY_DPIN, OUTPUT );
  pinMode ( LCD_LIGHT_DPIN,    OUTPUT );
  pinMode ( LCD_BUTTON_DPIN,   INPUT  );

  // initialize variables
  initialize();

  // initialize median filter
  filter.begin ( FILTER_SEED );

  // initialize consumption statistics
  stats.begin();

  analyzer.begin();

  // initialize the LCD
  
  // configure LCD size
  lcd_if.begin ( LCD_COLS, LCD_ROWS );

  // add special symbols
  
  lcd_if.createChar ( PROGRESS_CHAR  , const_cast<byte*> ( LCD_PROGRESS   ) );
  lcd_if.createChar ( ARROW_UP_CHAR  , const_cast<byte*> ( LCD_ARROW_UP   ) );
  lcd_if.createChar ( ARROW_DOWN_CHAR, const_cast<byte*> ( LCD_ARROW_DOWN ) );

  lcd.begin ( &lcd_if, LCD_LIGHT_DPIN );
  
  // compute tank parameters
  tank_capacity     = ( TANK_RADIUS_CM * TANK_RADIUS_CM * PI * WATER_MAX_HEIGHT_CM ) / CM3_PER_LITER;
  maximum_capacity  = TANK_NUMBER * tank_capacity;
  
  delay ( 1000 );

  // do autotest
  autotest();

  // turn on LCD backlight
  digitalWrite ( LCD_LIGHT_DPIN, HIGH );

  // print SW information
  lcd.print ( "Avvio..." );
  lcd.update_lcd();

  delay ( 1000 );
  
  show_version();
  lcd.update_lcd();

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
  lcd.update_lcd();
}

//!
//! \brief Loop function.
//!
void loop ( void ) {
  // consumption time management
  stats.updateTime();

  // button management
  manage_button();

  lcd.update_lcd();

  // read current milliseconds passed from boot.
  const uint32_t timestamp_now = millis();
  
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

      previous_liters = liters;

      // compute the percentage
      percentage = compute_percentage ( liters );

      // update statistics
      stats.updateConsumption ( static_cast< uint32_t > ( liters ) );

      // update LCD if not in debug
      if ( in_debug == false ) {       
        update_lcd ( percentage, liters );
      }

      // update indicator
      monitorReadings ( );
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

/// @}
