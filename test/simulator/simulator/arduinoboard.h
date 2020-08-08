#ifndef ARDUINOBOARD_H
#define ARDUINOBOARD_H

#ifdef QT_CORE_LIB
#include <QObject>
#endif

#include "arduino_types.h"
#include <mutex>

class ArduinoBoard
#ifdef QT_CORE_LIB
      : public QObject
{
    Q_OBJECT
#else
{
#endif
public:
    ArduinoBoard ( void );

    int32_t max ( int32_t a, int32_t b );

    int32_t millis ( void );

    void delay ( int32_t ms );

    void digitalWrite ( int32_t pin, int32_t v );

    int32_t digitalRead ( int32_t pin );

    void pinMode ( int32_t pin, int32_t mode );

    uint32_t pulseIn ( int32_t pin, int32_t mode, int32_t timeout );

    void setPulseTime ( int32_t pin, uint32_t time );

    void setPinValue ( int32_t pin, int32_t mode, int32_t value );

    void attachInterrupt ( int32_t pin, void (*isr)(void), int32_t mode );

    int32_t digitalPinToInterrupt ( int32_t pin );

    void delayMicroseconds ( int32_t usec );

    void setTimeScale ( double scale );

    static Pin_t arduino_pins[];

#ifdef QT_CORE_LIB
signals:
    void pinWritten ( int32_t pin, int32_t val );

    void sleepEvent ( int32_t usec );
    void pinEvent   ( int32_t pinId, int32_t val );

#endif

private:
    std::mutex pinLock;

    double timeScale;
};

extern ArduinoBoard arduino;

#endif // ARDUINOBOARD_H
