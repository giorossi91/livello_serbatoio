/**
 * @brief Software del sensore di livello del serbatoio. 
 * @details Il software monitora il livello del serbatoio dell'acqua mediante sensore ad ultrasuoni
 *          e mostra i risultati su un display LCD e un LED in grado di segnalare la scarsità di
 *          acqua rimanente.
 * @author Giovanni Rossi
 */

/*
 * Collegamenti e pinout
 * LCD (1602A):
 *   Vss  : GND
 *   Vdd  : +5V
 *   Vo   : OUT Potenziometro
 *   RS   : DIGITAL 12
 *   RW   : GND (Write)
 *   E    : DIGITAL 11
 * 
 *   DB4, DB5, DB6, DB7
 *   4 ,  5 ,  6 ,  7  (DIGITAL)
 */

/* Include */
#include <LiquidCrystal.h>

/* Define */

#define VERSION "v0.2"

#define CONF_DEBUG   1
#define CONF_RELEASE 0

#define SENSOR_HCSR04 0
#define SENSOR_JSNSR04T 1

#define roundfvalue(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))

/* Configurazioni */
#define DEBUG  CONF_RELEASE
#define SENSOR SENSOR_HCSR04

/* Classi */
class MedianFilter {
public:
  MedianFilter(const byte size, const int16_t seed) {
    medFilterWin    = max(size, 3);                           // number of samples in sliding median filter window - usually odd #
    medDataPointer  = size >> 1;                              // mid point of window
    data            = (int16_t *) calloc(size, sizeof(int));  // array for data
    sizeMap         = (byte *)    calloc(size, sizeof(byte)); // array for locations of data in sorted list
    locationMap     = (byte *)    calloc(size, sizeof(byte)); // array for locations of history data in map list
    oldestDataPoint = medDataPointer;                         // oldest data point location in data array

    // initialize the arrays
    for (byte i = 0; i < medFilterWin; i++) {
      sizeMap[i]     = i;    // start map with straight run
      locationMap[i] = i;    // start map with straight run
      data[i]        = seed; // populate with seed value
    }
  } 
    
  int16_t in(int16_t value) {
    // sort sizeMap
    // small vaues on the left (-)
    // larger values on the right (+)
    
    boolean dataMoved    = false;
    const byte rightEdge = medFilterWin - 1;    // adjusted for zero indexed array
    
    data[oldestDataPoint] = value;  // store new data in location of oldest data in ring buffer
    
    // SORT LEFT (-) <======(n) (+)
    if (locationMap[oldestDataPoint] > 0) { // don't check left neighbours if at the extreme left
      for (byte i = locationMap[oldestDataPoint]; i > 0; i--) {   //index through left adjacent data
        byte n = i - 1; // neighbour location

        if (data[oldestDataPoint] < data[sizeMap[n]]) { // find insertion point, move old data into position
          sizeMap[i] = sizeMap[n];    // move existing data right so the new data can go left
          locationMap[sizeMap[n]]++;
  
          sizeMap[n] = oldestDataPoint;   // assign new data to neighbor position
          locationMap[oldestDataPoint]--;
  
          dataMoved = true;
        } else {
          break;  // stop checking once a smaller value is found on the left 
        }
      }
    }
    
    // SORT RIGHT (-) (n)======> (+)
    if (!dataMoved && locationMap[oldestDataPoint] < rightEdge) {   // don't check right if at right border, or the data has already moved
      for (byte i = locationMap[oldestDataPoint]; i < rightEdge; i++) {    //index through left adjacent data
        uint16_t n = i + 1;  // neighbour location
    
        if (data[oldestDataPoint] > data[sizeMap[n]]) { // find insertion point, move old data into position
          sizeMap[i] = sizeMap[n];    // move existing data left so the new data can go right
          locationMap[sizeMap[n]]--;
  
          sizeMap[n] = oldestDataPoint;   // assign new data to neighbor position
          locationMap[oldestDataPoint]++;
        } else {
          break;  // stop checking once a smaller value is found on the right 
        }
      }
    }
    
    oldestDataPoint++;  // increment and wrap
    if (oldestDataPoint == medFilterWin) {
      oldestDataPoint = 0;
    }
    
    return data[sizeMap[medDataPointer]];
  }
    
  int16_t out(void) {
    return data[sizeMap[medDataPointer]];
  }

private:
  byte    medFilterWin;      /// Number of samples in sliding median filter window - usually odd #
  byte    medDataPointer;    /// Mid point of window
  int16_t *data;             /// Array pointer for data sorted by age in ring buffer
  byte    *sizeMap;          /// Array pointer for locations data in sorted by size
  byte    *locationMap;      /// Array pointer for data locations in history map
  byte    oldestDataPoint;   /// Oldest data point location in ring buffer
};

/* Costanti */
const int16_t ECHO_DPIN         = 12;
const int16_t TRIG_DPIN         = 11;
const int16_t LED_CAPACITY_DPIN = 13;
const int16_t LCD_BUTTON_DPIN   = 2;
const int16_t LCD_LIGHT_DPIN    = 9;

const double   TANK_RADIUS_CM        = 35.0;    //cm
const double   TANK_HEIGHT_CM        = 156.0;   //cm
const double   SENSOR_DISTANCE       = 26.0;    //cm
const uint16_t TANK_NUMBER           = 2;
const double   CM3_PER_LITER         = 1000.0;  //1 l = 1000 cm^3
const double   LOW_LEVEL_THRESHOLD   = 30.0;    //%
const double   EMPTY_LEVEL_THRESHOLD = 10.0;    //%
const double   WATER_MAX_HEIGHT_CM   = TANK_HEIGHT_CM - SENSOR_DISTANCE; //cm


const uint32_t LCD_ON_TIMER         = 30 * 1000; //ms
const uint32_t MEASUREMENT_INTERVAL = 10 * 1000; //ms
const uint32_t SLEEP_TIME           = 1500;      //ms

#if SENSOR == SENSOR_JSNSR04T
const double SENSOR_CALIBRATION = 1.7;  //cm
#elif SENSOR == SENSOR_HCSR04
const double SENSOR_CALIBRATION = 0.0;  //cm
#else
const double SENSOR_CALIBRATION = 0.0;  //cm
#endif

const byte UP_ARROW_CHAR = 0;
const byte PROGRESS_CHAR = 1;

const int16_t RS  = 3;
const int16_t E   = 4;
const int16_t DB4 = 5;
const int16_t DB5 = 6;
const int16_t DB6 = 7;
const int16_t DB7 = 8;

byte LCD_UP_ARROW[] = { 4, 14, 21, 4, 4, 0, 0, 0 };
byte LCD_PROGRESS[] = { 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F };

const int16_t FILTER_SEED = 0;
const int16_t FILTER_SIZE = 5;

const uint32_t SENSOR_RESPONSE_TIMEOUT_US = 500000UL; //500 ms
const uint32_t SENSOR_NO_OBSTACLE_US      = 38000UL;  //38 ms


/* Variabili */
bool led_on          = false;
bool led_status      = false;
bool must_update_lcd = false;


uint16_t measure_interval       = 1000;  //ms
double   tank_capacity          = 0.0;   //L
uint16_t number_measures_done   = 0;   
bool     first_measure_done     = false;
double   maximum_capacity       = 0.0;   //L

volatile uint32_t timestamp_lcd_on       = 0U;    //ms
volatile uint32_t timestamp_measurement  = 0U;    //ms
volatile uint32_t timestamp_last_filling = 0U;    //ms
volatile double   distance               = 0.0;   //cm
volatile double   previous_liters        = 0.0;   //L
double            percentage             = 100.0; //%

//Formato (RS, E, DB4, DB5, DB6, DB7)
LiquidCrystal lcd(RS, E, DB4, DB5, DB6, DB7);

MedianFilter filter(FILTER_SIZE, FILTER_SEED);

/* Main SW */

/**
 * @brief Calcola la quantità di acqua rimasta nel serbatoio.
 * @details Il calcolo viene eseguito come schematizzato di seguito.
 *            
 *             sensor
 *        +----| W |----+ ---  TANK_HEIGHT_CM
 *        |      |      |    } 
 *        |  (distance) |    > SENSOR_DISTANCE
 *        |      |      |    }
 *     ----------------------> WATER_MAX_HEIGHT_CM
 *        |      |      |
 *        |      V      |   
 *     ----------------------> current water level
 *        |             |      
 *        |             |
 *     ----------------------> level 0
 *        
 *    real distance = distance - SENSOR_DISTANCE
 * 
 * @param read_distance La distanza del sensore dall'acqua in cm.
 * 
 * @return Il numero di litri rimanenti nel serbatoio.
 * 
 */
inline double compute_liters(double read_distance) {
  read_distance -= SENSOR_DISTANCE;
  return TANK_NUMBER * ((TANK_RADIUS_CM * TANK_RADIUS_CM * PI * (WATER_MAX_HEIGHT_CM - read_distance)) / CM3_PER_LITER);
}

/**
 * @brief Calcola la percentuale di acqua rimanente nel serbatoio.
 * @details Utilizza la lettura attuale e la capacità massima calcolata nella funzione setup().
 * 
 * @param read_liters La quantità di litri letti.
 * 
 * @return La percentuale di riempimento nel range [0, 100].
 */
inline double compute_percentage(double read_liters) {
  return (read_liters / maximum_capacity) * 100.0;
}

/**
 * @brief Accende la retroilluminazione dell'LCD.
 * @details Alza la linea di alimentazione della retroilluminazione ed attiva il timer di spegnimento.
 *          Utilizzata come ISR alla pressione del bottone.
 */
inline void turn_on_lcd_light(void) {
  digitalWrite(LCD_LIGHT_DPIN, HIGH);
  timestamp_lcd_on = millis();
#if DEBUG
  Serial.print("Button pressed. Timestamp: ");
  Serial.println(timestamp_lcd_on);
#endif
}

/**
 * @brief Spegne la retroilluminazione dell'LCD.
 * @details Abbassa la linea di alimentazione della retroilluminazione.
 */
inline void turn_off_lcd_light(void) {
  digitalWrite(LCD_LIGHT_DPIN, LOW);
#if DEBUG
  Serial.println("LCD off");
#endif
}

/**
 * @brief Esegue una misurazione di livello dell'acqua.
 * @details Attiva il sensore ad ultrasuoni ed applica eventuali compensazioni necessarie.
 * 
 * @return La misura in cm oppure -1.0 in caso di errore di misura.
 */
inline double measure_level(void) {
  //activate distance measurement
  digitalWrite(TRIG_DPIN, HIGH);
  delayMicroseconds(15); //>= 10 usec da datasheet
  digitalWrite(TRIG_DPIN, LOW);
  
  uint32_t time = pulseIn(ECHO_DPIN, HIGH, SENSOR_RESPONSE_TIMEOUT_US);
  if ((time == 0U) || (time >= SENSOR_NO_OBSTACLE_US)) {
    lcd.clear();
    lcd.setCursor(5, 1);
    lcd.print("Errore");
    return -1.0; //Errore
  }
  
  double distance_read = (double) time / (58.0);  //convert to cm
  
  //compensate the constant error introduced by new sensor
  double dist_compensated = distance_read + SENSOR_CALIBRATION;
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
    measure_interval   = MEASUREMENT_INTERVAL;
  }

  return dist_compensated;
}

/**
 * @brief Controlla lo stato del LED in base al livello dell'acqua.
 * @details Se il livello dell'acqua è al di sotto di LOW_LEVEL_THRESHOLD ma al di sopra di EMPTY_LEVEL_THRESHOLD
 *          lo fa accendere, se è al di sotto di EMPTY_LEVEL_THRESHOLD lo fa lampeggiare, altrimenti lo lascia spento.
 *          
 * @param percentage La percentuale di riempimento del serbatoio (in [0, 100]).
 */
inline void control_led(double percentage) {
  if (percentage <= EMPTY_LEVEL_THRESHOLD) {
    led_status = true;
    led_on     = !led_on;
  } else if (percentage < LOW_LEVEL_THRESHOLD) {
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
inline double sanitize_data(double data, double min_val, double max_val, double min_threshold, double max_threshold) {
  double sanitized_data = min_val;
  
  if (data < min_threshold) {
    sanitized_data = min_val;
  } else if (data > max_threshold) {
    sanitized_data = max_val;
  } else {
    sanitized_data = data;
  }
  
  return sanitized_data;
}



#if !DEBUG

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
inline void update_lcd(double percentage, double liters) {
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
    if (l >= 10000) {
      index--;
    }
    lcd.setCursor(index, 0);
    lcd.print(l);
  
    percentage = sanitize_data(percentage, 0.0, 100.0, 0.5, 99.5);
    
    int16_t p = (int16_t) percentage;
    index = 12;
    if (p < 10) {
    } else if (p < 100) {
      index--;
    } else if (p < 1000) {
      index--;
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

#else

/**
 * @brief Aggiorna l'LCD con i parametri attuali.
 * @details Mostra informazioni di debug sull'LCD.
 *  
 * @param distance_to_print La distanza dall'acqua in cm.
 * @param percentage La percentuale di riempimento del serbatoio.
 * @param liters I litri di acqua rimanenti.
 */
inline void update_lcd_debug(double distance_to_print, double percentage, double liters) {
  //It's a debug function, update always!
  
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("d: ");
  lcd.print((int) distance_to_print);
  lcd.print("cm");
  
  lcd.setCursor(0, 1);
  lcd.print("l: ");
  lcd.print((int) liters);
  lcd.print("L");
  
  lcd.setCursor(10, 1);
  //lcd.write(UP_ARROW_CHAR);
  lcd.print(" ");
  
  if (percentage >= 99.5) {
    percentage = 100.0;
  } else if (percentage <= 0.5) {
    percentage = 0.0;
  }
  
  lcd.print((int) percentage);
  lcd.print("%");
}

#endif

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

void setup(void) {
  //16 characters e 2 lines
  lcd.begin(16, 2);
  
  lcd.createChar(UP_ARROW_CHAR, LCD_UP_ARROW);
  lcd.createChar(PROGRESS_CHAR, LCD_PROGRESS);
  
  first_measure_done = false;
    
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

  attachInterrupt(digitalPinToInterrupt(LCD_BUTTON_DPIN), turn_on_lcd_light, RISING);
   
  led_on     = false;
  led_status = false;

  digitalWrite(LCD_LIGHT_DPIN, HIGH);

  timestamp_lcd_on       = millis();
  timestamp_measurement  = millis();
  timestamp_last_filling = millis();

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
  
  //lcd on timer
  if ((millis() - timestamp_lcd_on) > LCD_ON_TIMER) {
    turn_off_lcd_light();
  }

  //measument timer
  if ((millis() - timestamp_measurement) > measure_interval) {
    distance = measure_level();
    if(abs((-1.0) - distance) < 0.1) {
      return; //ERRORE! Ferma il loop!
    }
    filter.in(roundfvalue(distance));
  }

  distance = filter.out();
  distance = sanitize_data(distance, SENSOR_DISTANCE, (WATER_MAX_HEIGHT_CM + SENSOR_DISTANCE), SENSOR_DISTANCE, (WATER_MAX_HEIGHT_CM + SENSOR_DISTANCE));

  if (first_measure_done) {
    double liters = compute_liters(distance);
    liters = sanitize_data(liters, 0.0, maximum_capacity, 0.5, maximum_capacity);

    if(previous_liters != liters) {
      must_update_lcd = true;
    }
    previous_liters = liters;
    
    percentage = compute_percentage(liters);
      
#if DEBUG
    update_lcd_debug(distance, percentage, liters);
#else
    update_lcd(percentage, liters);
#endif
  }
  control_led(percentage);
  
  delay(SLEEP_TIME);
}
