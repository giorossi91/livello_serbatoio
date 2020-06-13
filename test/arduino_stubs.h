#ifndef ARDUINO_STUBS_H_
#define ARDUINO_STUBS_H_

#include <string>
#include <cstdint>

typedef uint8_t byte;
typedef bool boolean;
typedef std::string String;

#define PI 3.14

const int LOW = 0;
const int HIGH = 1;
const int RISING = 1;

const int INPUT = 0;
const int OUTPUT = 1;

int max(int a, int b) {
  return (a > b) ? a : b;
}

// TODO da fare

int millis(void) {

}

void delay(int ms) {

}

void digitalWrite(int pin, int v) {

}

int digitalRead(int pin) {

}

void pinMode (int pin, int mode) {

}

int pulseIn(int pin, int mode, int timeout) {

}

void attachInterrupt (int pin, void (*isr)(void), int mode) {

}

int digitalPinToInterrupt(int pin) { 

} 

void delayMicroseconds(int usec) {

}


#endif // ARDUINO_STUBS_H_
