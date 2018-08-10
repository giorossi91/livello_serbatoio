/*
  PIN LCD (1602A):
 
 Vss  : GND
 Vdd  : +5V
 Vo   : OUT Potenziometro
 RS   : DIGITAL 12
 RW   : GND (Write)
 E    : DIGITAL 11
 
 DB4, DB5, DB6, DB7
 4 ,  5 ,  6 ,  7  (DIGITAL)
 */

/* ======== INCLUDE ======== */
#include <LiquidCrystal.h>
#include <MedianFilter.h>


/* ======== DEFINE  ======== */
#define DEBUG 0

#define roundf(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))

/* ======== CONSTANTS ====== */
/*
const int echo_dpin = 10;
const int trig_dpin = 8;
const int led_capacity_dpin = 2;
const int lcd_button_dpin = 3;
const int lcd_light_dpin = 13;
*/


const int echo_dpin = 12;
const int trig_dpin = 11;
const int led_capacity_dpin = 13;
const int lcd_button_dpin = 2;
const int lcd_light_dpin = 9;
const byte lcd_up_arrow[] = {4, 14, 21, 4, 4, 0, 0, 0};

const double TANK_RADIUS_CM = 35.0;  //cm
const double TANK_HEIGHT_CM = 156.0;//137.0;//125;  //cm
const double SENSOR_DISTANCE = 26.0; //cm
const double WATER_MAX_HEIGHT_CM = TANK_HEIGHT_CM - SENSOR_DISTANCE;
const int    TANK_NUMBER = 2;
const double CM3_PER_LITER = 1000.0; //1 l = 1000 cm^3
const double LOW_LEVEL_THRESHOLD = 25.0; //%
const double EMPTY_LEVEL_THRESHOLD = 5.0; //%

const int LCD_ON_TIMER = 30 * 1000; //ms
const int MEASUREMENT_INTERVAL = 10   * 1000; //ms
const int FILLING_TIMER = 5 * 60 * 1000; //ms (5 min)

const byte UP_ARROW_CHAR = 0;
const int RS = 3;
const int  E = 4;
const int DB4 = 5;
const int DB5 = 6;
const int DB6 = 7;
const int DB7 = 8;


/* ======== VARIABLES ====== */
bool led_on;
bool led_status;

double tank_capacity; //L


bool first_measure_done;
double maximum_capacity = 0.0; //L
volatile unsigned long timestamp_lcd_on; //ms
volatile unsigned long timestamp_measurement; //ms
volatile unsigned long timestamp_last_filling; //ms
volatile double distance = 0; //cm
volatile bool is_filling;
volatile double previous_distance;

MedianFilter filter(5, 0);

double percentage = 100.0;

//Formato (RS, E, DB4, DB5, DB6, DB7)
LiquidCrystal lcd(RS, E, DB4, DB5, DB6, DB7);

/*           
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
 * 
 * 
 */
inline double compute_liters(double read_distance)  {
  read_distance -= SENSOR_DISTANCE;
  return TANK_NUMBER * ((TANK_RADIUS_CM * TANK_RADIUS_CM * PI * (WATER_MAX_HEIGHT_CM - read_distance)) / CM3_PER_LITER);
}

inline double compute_percentage(double read_liters)  {
  return (read_liters / maximum_capacity) * 100.0;
}


void turn_on_lcd() {
  digitalWrite(lcd_light_dpin, HIGH);
  timestamp_lcd_on = millis();
}

void setup() {
  //16 characters e 2 lines
  lcd.begin(16, 2);
  
  lcd.print("    Avvio...");
  lcd.createChar(UP_ARROW_CHAR, lcd_up_arrow);
  first_measure_done = false;
  is_filling = false;

  //compute tank parameters
  tank_capacity = (TANK_RADIUS_CM * TANK_RADIUS_CM * PI * WATER_MAX_HEIGHT_CM) / CM3_PER_LITER;
  maximum_capacity = TANK_NUMBER * tank_capacity;
  previous_distance = distance;

  //pin setup
  pinMode(echo_dpin, INPUT);
  pinMode(trig_dpin, OUTPUT);
  pinMode(led_capacity_dpin, OUTPUT);

  pinMode(lcd_light_dpin, OUTPUT);
  pinMode(lcd_button_dpin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(lcd_button_dpin), turn_on_lcd, RISING);
  
  //lcd on
  timestamp_lcd_on = millis();  
  digitalWrite(lcd_light_dpin, HIGH);

  led_on = false;
  led_status = false;

  timestamp_measurement = millis();
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
}

void loop() {
  //lcd on timer
#if !DEBUG
  if((millis() - timestamp_lcd_on) > LCD_ON_TIMER) {
    digitalWrite(lcd_light_dpin, LOW);
  }
#endif

  //measument timer
  if((millis() - timestamp_measurement) > MEASUREMENT_INTERVAL) {
    //activate distance measurement
    digitalWrite(trig_dpin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_dpin, LOW);
  
    int time = pulseIn(echo_dpin, HIGH);

    distance = (double)time/(58.0); //convert to cm
#if DEBUG
    Serial.print("dist: ");
    Serial.print(distance);
    Serial.print(" round: ");
    Serial.println(roundf(distance));
#endif    
    filter.in(roundf(distance));

    timestamp_measurement = millis();
    first_measure_done = true;
  }

  distance = filter.out();
  
  if(distance < SENSOR_DISTANCE) {
    distance = SENSOR_DISTANCE;
  } else if(distance > (WATER_MAX_HEIGHT_CM + SENSOR_DISTANCE)) {
    distance = WATER_MAX_HEIGHT_CM + SENSOR_DISTANCE;
  } 


  if((millis() - timestamp_last_filling) > FILLING_TIMER) {
    timestamp_last_filling = millis();
    // if the new distance is lower than the previous one, then the water level is greater, so the tank is filling.
    if(distance < previous_distance) {
      is_filling = true;
    } else {
      is_filling = false;
    }
    previous_distance = distance;
  }

  if(first_measure_done) {
    double liters = compute_liters(distance);
    if(liters < 0.5) {
      liters = 0.0;
    }
    if(liters > maximum_capacity) {
      liters = maximum_capacity;
    }
    percentage = compute_percentage(liters);
#if DEBUG
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("d: ");
    lcd.print((int)distance);
    lcd.print("cm");

    lcd.setCursor(0, 1);
    lcd.print("l: ");
    lcd.print((int)liters);
    lcd.print("L");

    lcd.setCursor(10,1);
    lcd.write(UP_ARROW_CHAR);
    //lcd.print(" ");
    
    if(percentage >= 99.5) {
      percentage = 100.0;
    } else if (percentage <= 0.5) {
      percentage = 0.0;
    }
    
    lcd.print((int) percentage);
    lcd.print("%");
#else
/*
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
 */
    lcd.clear();
    lcd.setCursor(6,0);
    lcd.print("L");
    lcd.setCursor(14,0);
    lcd.print("%");

    if(is_filling) {
      lcd.setCursor(8,0);
      lcd.write(UP_ARROW_CHAR);
    }

    int index = 4;
    int l = (int) liters;
    if(l >= 10) {
      index --;
    } 
    if(l >= 100) {
      index --;
    }
    if(l >= 1000) {
      index --;
    }
    if(l >= 10000) {
      index --;
    }
    lcd.setCursor(index, 0);    
    lcd.print(l);

    if(percentage >= 99.5) {
      percentage = 100.0;
    } else if (percentage <= 0.5) {
      percentage = 0.0;
    }
    int p = (int) percentage;
    index = 12;
    if(p < 10) {
    } else if(p < 100) {
      index --;
    } else if(p < 1000) {
      index --;
    }
    lcd.setCursor(index, 0);
    lcd.print(p);

    lcd.setCursor(1, 1);
    int howmany = (14 * p) / 100;
    for(int i = 0; i < howmany; i++) {
      lcd.print("#");   
    }

    if(howmany == 0) {
      lcd.setCursor(5, 1);
      lcd.print("Vuoto");
    } else if(howmany <= 2) {
      lcd.setCursor(5, 1);
      lcd.print("Riserva");
    }
#endif
  }
  if(percentage <= EMPTY_LEVEL_THRESHOLD) {
    led_status = true;
    led_on = !led_on;
  } else if(percentage < LOW_LEVEL_THRESHOLD)  {
    led_status = true;
    led_on = true;
  } else {
    led_status = false;
    led_on = false;
  }
  digitalWrite(led_capacity_dpin, led_on);
  delay(MEASUREMENT_INTERVAL);
}



