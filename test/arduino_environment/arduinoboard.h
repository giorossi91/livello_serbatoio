#ifndef ARDUINOBOARD_H
#define ARDUINOBOARD_H

#include <QObject>

#include "arduino_types.h"
#include <mutex>

class ArduinoBoard : public QObject
{
    Q_OBJECT

public:
    ArduinoBoard ( void );

    int32_t max ( int32_t a, int32_t b );

    virtual uint32_t millis( void );

    virtual void delay ( int32_t ms );

    virtual void digitalWrite ( int32_t pin, int32_t v );

    virtual int32_t digitalRead ( int32_t pin );

    virtual void pinMode ( int32_t pin, int32_t mode );

    virtual uint32_t pulseIn ( int32_t pin, int32_t mode, int32_t timeout );

    virtual void setPulseTime ( int32_t pin, uint32_t time );

    virtual void setPinValue ( int32_t pin, int32_t mode, int32_t value );

    virtual void attachInterrupt ( int32_t pin, void (*isr)(void), int32_t mode );

    virtual int32_t digitalPinToInterrupt ( int32_t pin );

    virtual void delayMicroseconds ( int32_t usec );

    virtual void setTimeScale ( double scale );

    static constexpr int32_t N_PINS = 20;

    static constexpr int32_t SLEEP_EVENT = 99;

    Pin_t arduino_pins[N_PINS];

    std::mutex pinLock;

signals:
    void boardEvent ( int32_t id, int32_t val );

protected:

    double timeScale;
};

#endif // ARDUINOBOARD_H
