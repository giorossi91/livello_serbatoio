#include <arduino_stubs.h>
#include "arduinoboard.h"


int32_t max(int32_t a, int32_t b) {
  return arduino.max(a,b);
}


uint32_t millis ( void ) {
  return arduino.millis();
}

void delay ( int32_t ms ) {
    arduino.delay(ms);
}

void digitalWrite ( int32_t pin, int32_t v ) {
    arduino.digitalWrite(pin, v);
}

int32_t digitalRead ( int32_t pin ) {
    return arduino.digitalRead(pin);
}

void pinMode ( int32_t pin, int32_t mode ) {
  arduino.pinMode(pin, mode);
}

uint32_t pulseIn ( int32_t pin, int32_t mode, int32_t timeout ) {
    return arduino.pulseIn(pin, mode, timeout);
}

void attachInterrupt (int32_t pin, void (*isr)(void), int32_t mode) {
    return arduino.attachInterrupt(pin, isr, mode);
}

int32_t digitalPinToInterrupt ( int32_t pin ) {
  return arduino.digitalPinToInterrupt(pin);
}

void delayMicroseconds ( int32_t usec ) {
    arduino.delayMicroseconds(usec);
}
