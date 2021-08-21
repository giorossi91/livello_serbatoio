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

    virtual uint32_t millis ( void );

    virtual void delay ( int32_t ms );

    virtual void digitalWrite ( int32_t pin, int32_t v );

    virtual int32_t digitalRead ( int32_t pin );

    virtual void pinMode ( int32_t pin, int32_t mode );

    virtual uint32_t pulseIn ( int32_t pin, int32_t mode, int32_t timeout );

    virtual void attachInterrupt ( int32_t pin, isr_t isr, int32_t mode );

    virtual int32_t digitalPinToInterrupt ( int32_t pin );

    virtual void delayMicroseconds ( int32_t usec );


    virtual void harness_setPulseTime ( int32_t pin, uint32_t time );

    virtual void harness_setPinValue ( int32_t pin, int32_t mode, int32_t value );

    virtual int32_t harness_getPinMode ( int32_t pin );

    virtual int32_t harness_getIsrMode ( int32_t pin );

    virtual isr_t harness_getIsrFunc ( int32_t pin );

    virtual int32_t harness_getPinExternalValue ( int32_t pin );

    virtual int32_t harness_getPinInternalValue ( int32_t pin );

    virtual void harness_setTimeScale ( double scale );

    virtual uint32_t harness_getTimePassedFromBoot ( void );

    virtual void harness_setTimePassedFromBoot ( const uint32_t timePassedFromBoot );

signals:
    void boardEvent ( int32_t id, int32_t val );

protected:
    bool harness_checkPinNumber ( const int32_t pin ) const;

    static constexpr int32_t N_PINS = 20;

    static constexpr int32_t SLEEP_EVENT = 99;

    Pin_t arduino_pins[N_PINS];

    std::mutex pinLock;

    std::mutex timeScaleLock;

    double timeScale;
};

#endif // ARDUINOBOARD_H
