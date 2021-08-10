#include <arduino_stubs.h>
#include "arduinoboard.h"

ArduinoBoard *board = nullptr;

int32_t max(int32_t a, int32_t b) {
  return board->max(a,b);
}

uint32_t millis ( void ) {
  return board->millis();
}

void delay ( int32_t ms ) {
    board->delay(ms);
}

void digitalWrite ( int32_t pin, int32_t v ) {
    board->digitalWrite(pin, v);
}

int32_t digitalRead ( int32_t pin ) {
    return board->digitalRead(pin);
}

void pinMode ( int32_t pin, int32_t mode ) {
  board->pinMode(pin, mode);
}

uint32_t pulseIn ( int32_t pin, int32_t mode, int32_t timeout ) {
    return board->pulseIn(pin, mode, timeout);
}

void attachInterrupt (int32_t pin, void (*isr)(void), int32_t mode) {
    return board->attachInterrupt(pin, isr, mode);
}

int32_t digitalPinToInterrupt ( int32_t pin ) {
  return board->digitalPinToInterrupt(pin);
}

void delayMicroseconds ( int32_t usec ) {
    board->delayMicroseconds(usec);
}
