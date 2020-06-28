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

#define A0 14
#define A1 15
#define A2 16
#define A3 17
#define A4 18
#define A5 19

#define D0   0
#define D1   1
#define D2   2
#define D3   3
#define D4   4
#define D5   5
#define D6   6
#define D7   7
#define D8   8
#define D9   9
#define D10 10
#define D11 11
#define D12 12
#define D13 13

#endif // ARDUINO_TYPES_H_
