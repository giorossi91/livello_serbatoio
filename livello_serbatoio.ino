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
#include<LiquidCrystal.h>

#define DEBUG 0

const int echo_dpin = 10;
const int trig_dpin = 8;
const int led_capacity_dpin = 2;
const int lcd_button_dpin = 3;
const int lcd_light_dpin = 13;

const double TANK_RADIUS_CM = 10;//34;  //cm
const double TANK_HEIGHT_CM = 30;//125;  //cm
const double SENSOR_DISTANCE = 10; //cm
const int TANK_NUMBER = 2;
const double CM3_PER_LITER = 1000.0; //1 l = 1000 cm^3
const double LOW_LEVEL_THRESHOLD = 20.0; //%
const double EMPTY_LEVEL_THRESHOLD = 5.0; //%

bool led_on;
bool led_status;

double tank_capacity; //L

const int LCD_ON_TIMER = 10 * 1000; //ms
const int MEASUREMENT_INTERVAL = 1 * 1000; //ms

double maximum_capacity = 0.0; //L
volatile unsigned long timestamp_lcd_on; //ms
volatile unsigned long timestamp_measurement; //ms
volatile double distance = 0; //cm

double percentage = 100.0;

const int RS = 12;
const int  E = 11;
const int DB4 = 4;
const int DB5 = 5;
const int DB6 = 6;
const int DB7 = 7;

//Formato (RS, E, DB4, DB5, DB6, DB7)
LiquidCrystal lcd(RS, E, DB4, DB5, DB6, DB7);

/*           
 *             sensor
 *        +----| W |----+ ---
 *        |      |      |     
 *        |  (distance) |      SENSOR_DISTANCE
 *        |      |      |  
 *     ----------------------> TANK_HEIGHT_CM
 *        |      |      |
 *        |      V      |   
 *     ----------------------> actual water level
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
  return TANK_NUMBER * ((TANK_RADIUS_CM * TANK_RADIUS_CM * PI * (TANK_HEIGHT_CM - read_distance)) / CM3_PER_LITER);
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

  //compute tank parameters
  tank_capacity = (TANK_RADIUS_CM * TANK_RADIUS_CM * PI * TANK_HEIGHT_CM) / CM3_PER_LITER;
  maximum_capacity = TANK_NUMBER * tank_capacity;

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
}

void loop() {

  //lcd on timer
  if((millis() - timestamp_lcd_on) > LCD_ON_TIMER) {
    digitalWrite(lcd_light_dpin, LOW);
  }

  //measument timer
  if((millis() - timestamp_measurement) > MEASUREMENT_INTERVAL) {
    //activate distance measurement
    digitalWrite(trig_dpin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_dpin, LOW);
  
    int time = pulseIn(echo_dpin, HIGH);

    distance = (double)time/(58.0); //convert to cm

    timestamp_measurement = millis();
  }
  
  if(distance<2 || distance > (TANK_HEIGHT_CM + SENSOR_DISTANCE)) {
    //do not update lcd
    //lcd.clear();
    //lcd.print("Fuori scala");
  } else {
    double liters = compute_liters(distance);
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
    lcd.print(" ");
    
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

    int index = 4;
    int l = (int) liters;
    if(l < 10) {
    } else if(l < 100) {
      index --;
    } else if(l < 1000) {
      index --;
    } else if(l < 10000) {
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
  delay(500);
}



