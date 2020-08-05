#ifndef ARDUINO_TYPES_H_
#define ARDUINO_TYPES_H_

#include <unistd.h>
#include <string>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cmath>
#include <atomic>

typedef uint8_t     byte;
typedef bool        boolean;
typedef std::string String;
typedef void ( *isr_t ) ( void );

#define HARNESS_PREFIX "[Harness] "

const double  PI     = M_PI;

const int32_t LOW    = 0;
const int32_t HIGH   = 1;
const int32_t RISING = 1;

const int32_t INPUT  = 0;
const int32_t OUTPUT = 1;

struct Pin_t {
  int32_t  id;
  int32_t  mode;
  isr_t    pIsr;
  int32_t  in_val;
  int32_t  out_val;
  uint32_t pulse_time;
};

const int A0 = 14;
const int A1 = 15;
const int A2 = 16;
const int A3 = 17;
const int A4 = 18;
const int A5 = 19;

const int D0  =  0;
const int D1  =  1;
const int D2  =  2;
const int D3  =  3;
const int D4  =  4;
const int D5  =  5;
const int D6  =  6;
const int D7  =  7;
const int D8  =  8;
const int D9  =  9;
const int D10 = 10;
const int D11 = 11;
const int D12 = 12;
const int D13 = 13;

#endif // ARDUINO_TYPES_H_
