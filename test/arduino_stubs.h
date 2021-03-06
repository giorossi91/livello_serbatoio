#ifndef ARDUINO_STUBS_H_
#define ARDUINO_STUBS_H_

#include "arduino_types.h"

std::atomic<uint32_t> timePassedFromBootMs;

Pin_t arduino_pins[] = {
  { D0  , INPUT , nullptr, 0, 0, 0 },
  { D1  , INPUT , nullptr, 0, 0, 0 },
  { D2  , INPUT , nullptr, 0, 0, 0 },
  { D3  , INPUT , nullptr, 0, 0, 0 },
  { D4  , INPUT , nullptr, 0, 0, 0 },
  { D5  , INPUT , nullptr, 0, 0, 0 },
  { D6  , INPUT , nullptr, 0, 0, 0 },
  { D7  , INPUT , nullptr, 0, 0, 0 },
  { D8  , INPUT , nullptr, 0, 0, 0 },
  { D9  , INPUT , nullptr, 0, 0, 0 },
  { D10 , INPUT , nullptr, 0, 0, 0 },
  { D11 , INPUT , nullptr, 0, 0, 0 },
  { D12 , INPUT , nullptr, 0, 0, 0 },
  { D13 , INPUT , nullptr, 0, 0, 0 },
  { A0  , INPUT , nullptr, 0, 0, 0 },
  { A1  , INPUT , nullptr, 0, 0, 0 },
  { A2  , INPUT , nullptr, 0, 0, 0 },
  { A3  , INPUT , nullptr, 0, 0, 0 },
  { A4  , INPUT , nullptr, 0, 0, 0 },
  { A5  , INPUT , nullptr, 0, 0, 0 }
};

int32_t max(int32_t a, int32_t b) {
  return (a > b) ? a : b;
}


int32_t millis ( void ) {
  return timePassedFromBootMs;
}

void delay ( int32_t ms ) {
  std::cout << HARNESS_PREFIX"Sleeping " << ms << " ms..." << std::endl;
  usleep(ms * 1000);
}

void digitalWrite ( int32_t pin, int32_t v ) {
  std::cout << HARNESS_PREFIX"DIG-O[" << std::setw(4) << pin << "] v = " << v << std::endl;
  arduino_pins[pin].in_val = v;
}

int32_t digitalRead ( int32_t pin ) {
  int32_t v = arduino_pins[pin].out_val; 
  std::cout << HARNESS_PREFIX"DIG-I[" << std::setw(4) << pin << "] v = " << v << std::endl;
  return v;
}

void pinMode ( int32_t pin, int32_t mode ) {
  arduino_pins[pin].mode = mode;
}

uint32_t pulseIn ( int32_t pin, int32_t mode, int32_t timeout ) {
  ( void ) mode;
  ( void ) timeout;

  uint32_t v = arduino_pins[pin].pulse_time;

  std::cout << HARNESS_PREFIX"INPUT[" << std::setw(4) << pin << "] t = " << v << " us" << std::endl;
  
  return arduino_pins[pin].pulse_time;
}

void attachInterrupt (int32_t pin, void (*isr)(void), int32_t mode) {
  ( void ) mode;
  arduino_pins[pin].pIsr = isr;
}

int32_t digitalPinToInterrupt ( int32_t pin ) {
  return pin;
} 

void delayMicroseconds ( int32_t usec ) {
  std::cout << HARNESS_PREFIX"Sleeping " << usec << " us..." << std::endl;
  usleep(usec);
}


#endif // ARDUINO_STUBS_H_
