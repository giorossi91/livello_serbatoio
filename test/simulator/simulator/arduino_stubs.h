#ifndef ARDUINO_STUBS_H_
#define ARDUINO_STUBS_H_

#include "arduino_types.h"

extern int32_t max(int32_t a, int32_t b);

extern int32_t millis ( void );

extern void delay ( int32_t ms );

extern void digitalWrite ( int32_t pin, int32_t v );

extern int32_t digitalRead ( int32_t pin );

extern void pinMode ( int32_t pin, int32_t mode );

extern uint32_t pulseIn ( int32_t pin, int32_t mode, int32_t timeout );

extern void attachInterrupt (int32_t pin, void (*isr)(void), int32_t mode);

extern int32_t digitalPinToInterrupt ( int32_t pin );

extern void delayMicroseconds ( int32_t usec );


#endif // ARDUINO_STUBS_H_
