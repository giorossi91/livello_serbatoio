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

const int echo = 10;
const int trig = 8;
const int led_capienza = 2;
const int bottone_lcd = 3;
const int luce_lcd = 13;


const double RAGGIO_SERBATOIO_CM = 25;  //cm
const double ALTEZZA_SERBATOIO_CM = 254;  //cm


const double FATTORE_CONVERSIONE_LITRI = 1000.0; //1 l = 1000 cm^3
const double CAPIENZA_SERBATOIO = 500.0; //litri
const int NUMERO_SERBATOI = 2;

const int TIMER_ACCENSIONE_LCD = 5 * 1000; //ms

double capienza_massima = 0.0;
volatile unsigned long time_on;

//Formato (RS, E, DB4, DB5, DB6, DB7)
LiquidCrystal lcd(12, 11, 4, 5, 6, 7);

inline double calcola_litri(double misura_distanza)  {
  return NUMERO_SERBATOI * ((RAGGIO_SERBATOIO_CM * RAGGIO_SERBATOIO_CM * PI * (ALTEZZA_SERBATOIO_CM - misura_distanza)) / FATTORE_CONVERSIONE_LITRI);
}

inline double calcola_percentuale(double litri_letti)  {
  return (litri_letti / capienza_massima) * 100.0;
}


void accendi_lcd() {
  digitalWrite(luce_lcd, HIGH);
  time_on = millis();
}

void setup() {
  //16 caratteri e 2 linee
  lcd.begin(16, 2);

  capienza_massima = NUMERO_SERBATOI * CAPIENZA_SERBATOIO;

  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);
  pinMode(led_capienza, OUTPUT);

  pinMode(luce_lcd, OUTPUT);
  pinMode(bottone_lcd, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(bottone_lcd), accendi_lcd, RISING);

  time_on = millis();
  digitalWrite(luce_lcd, HIGH);
}

void loop() {

  
  if((millis() - time_on) > TIMER_ACCENSIONE_LCD) {
    digitalWrite(luce_lcd, LOW);
  }
 
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  int time = pulseIn(echo, HIGH);
  double distance = (double)time/(58.0);

  if(distance<2 || distance > ALTEZZA_SERBATOIO_CM) {
    //lcd.print("Fuori scala");
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("d: ");
    lcd.print((int)distance);
    lcd.print("cm");

    lcd.setCursor(0, 1);
    double litri = calcola_litri(distance);
    lcd.print("l: ");
    lcd.print((int)litri);
    lcd.print("L");

    lcd.setCursor(10,1);
    double percentuale = calcola_percentuale(litri);
    lcd.print(" ");
    lcd.print((int) percentuale);
    lcd.print("%"); 

    if(percentuale < 20) {
      lcd.setCursor(15,1);
      lcd.print("!");
      digitalWrite(led_capienza, HIGH);
    } else {
      digitalWrite(led_capienza, LOW);
      //lcd.setCursor(15,1);
      //lcd.print(" ");
    }
  }
  delay(100);
}



