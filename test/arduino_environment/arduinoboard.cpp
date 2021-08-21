#include "arduinoboard.h"

#include <ctime>

ArduinoBoard::ArduinoBoard ( void ) : QObject() {
    timeScale = 1.0;

    uint32_t i = 0U;

    arduino_pins[i++] = { D0  , INPUT , nullptr, 0, 0, 0, 0 };
    arduino_pins[i++] = { D1  , INPUT , nullptr, 0, 0, 0, 0 };
    arduino_pins[i++] = { D2  , INPUT , nullptr, 0, 0, 0, 0 };
    arduino_pins[i++] = { D3  , INPUT , nullptr, 0, 0, 0, 0 };
    arduino_pins[i++] = { D4  , INPUT , nullptr, 0, 0, 0, 0 };
    arduino_pins[i++] = { D5  , INPUT , nullptr, 0, 0, 0, 0 };
    arduino_pins[i++] = { D6  , INPUT , nullptr, 0, 0, 0, 0 };
    arduino_pins[i++] = { D7  , INPUT , nullptr, 0, 0, 0, 0 };
    arduino_pins[i++] = { D8  , INPUT , nullptr, 0, 0, 0, 0 };
    arduino_pins[i++] = { D9  , INPUT , nullptr, 0, 0, 0, 0 };
    arduino_pins[i++] = { D10 , INPUT , nullptr, 0, 0, 0, 0 };
    arduino_pins[i++] = { D11 , INPUT , nullptr, 0, 0, 0, 0 };
    arduino_pins[i++] = { D12 , INPUT , nullptr, 0, 0, 0, 0 };
    arduino_pins[i++] = { D13 , INPUT , nullptr, 0, 0, 0, 0 };
    arduino_pins[i++] = { A0  , INPUT , nullptr, 0, 0, 0, 0 };
    arduino_pins[i++] = { A1  , INPUT , nullptr, 0, 0, 0, 0 };
    arduino_pins[i++] = { A2  , INPUT , nullptr, 0, 0, 0, 0 };
    arduino_pins[i++] = { A3  , INPUT , nullptr, 0, 0, 0, 0 };
    arduino_pins[i++] = { A4  , INPUT , nullptr, 0, 0, 0, 0 };
    arduino_pins[i++] = { A5  , INPUT , nullptr, 0, 0, 0, 0 };
}

int32_t ArduinoBoard::max ( int32_t a, int32_t b ) {
    return (a > b) ? a : b;
}

uint32_t ArduinoBoard::millis ( void ) {
    uint32_t ms = 0;

    struct timespec curr_time;
    int ret = clock_gettime(CLOCK_MONOTONIC_RAW, &curr_time);
    if(ret == 0) {
        ms = static_cast<uint32_t>(curr_time.tv_sec * 1000) + static_cast<uint32_t>(curr_time.tv_nsec / 1000000);
    } else {
        ms = 0;
    }

    timeScaleLock.lock();
    ms = static_cast<uint32_t> ( static_cast<double>( ms ) / timeScale );
    timeScaleLock.unlock();

    return ms;
}

void ArduinoBoard::delay ( int32_t ms ) {
    timeScaleLock.lock();
    useconds_t sleepTime = static_cast<useconds_t>(ms * 1000 * timeScale);
    timeScaleLock.unlock();

    emit boardEvent(SLEEP_EVENT, HIGH);

    usleep(sleepTime);

    emit boardEvent(SLEEP_EVENT, LOW);
}

void ArduinoBoard::digitalWrite ( int32_t pin, int32_t v ) {
    if ( harness_checkPinNumber( pin ) == true ) {
        pinLock.lock();
        arduino_pins[pin].internal_val = v;
        pinLock.unlock();

        // emit event for GUI
        emit boardEvent(pin, v);
    }
}

int32_t ArduinoBoard::digitalRead ( int32_t pin ) {
    int32_t v = -1;

    if ( harness_checkPinNumber( pin ) == true ) {
        pinLock.lock();
        v = arduino_pins[pin].external_val;
        pinLock.unlock();

        emit boardEvent(pin, v);
    }

    return v;
}

void ArduinoBoard::pinMode ( int32_t pin, int32_t mode ) {
    if ( harness_checkPinNumber( pin ) == true ) {
        pinLock.lock();
        arduino_pins[pin].mode = mode;
        pinLock.unlock();
    }
}

uint32_t ArduinoBoard::pulseIn ( int32_t pin, int32_t mode, int32_t timeout ) {
    ( void ) mode;
    ( void ) timeout;

    uint32_t v = 0U;

    if ( harness_checkPinNumber( pin ) == true ) {
        pinLock.lock();
        v = arduino_pins[pin].pulse_time;
        pinLock.unlock();
    }

    return v;
}

void ArduinoBoard::harness_setPulseTime(int32_t pin, uint32_t time) {
    if ( harness_checkPinNumber( pin ) == true ) {
        pinLock.lock();
        arduino_pins[pin].pulse_time = time;
        pinLock.unlock();
    }
}

void ArduinoBoard::harness_setPinValue(int32_t pin, int32_t mode, int32_t value) {
    if ( harness_checkPinNumber( pin ) == true ) {
        pinLock.lock();
        if(mode == OUTPUT) {
            arduino_pins[pin].internal_val = value;
        } else if (mode == INPUT) {
            arduino_pins[pin].external_val = value;
        }
        pinLock.unlock();
    }
}

int32_t ArduinoBoard::harness_getPinMode(int32_t pin) {
    int32_t mode = -1;
    if ( harness_checkPinNumber( pin ) == true ) {
        pinLock.lock();
        mode = arduino_pins[pin].mode;
        pinLock.unlock();
    }
    return mode;
}

int32_t ArduinoBoard::harness_getIsrMode(int32_t pin) {
    int32_t mode = -1;
    if ( harness_checkPinNumber( pin ) == true ) {
        pinLock.lock();
        mode = arduino_pins[pin].isrMode;
        pinLock.unlock();
    }
    return mode;
}

isr_t ArduinoBoard::harness_getIsrFunc(int32_t pin) {
    isr_t isrFunc = nullptr;
    if ( harness_checkPinNumber( pin ) == true ) {
        pinLock.lock();
        isrFunc = arduino_pins[pin].pIsr;
        pinLock.unlock();
    }
    return isrFunc;
}

int32_t ArduinoBoard::harness_getPinExternalValue(int32_t pin) {
    int32_t val = -1;
    if ( harness_checkPinNumber( pin ) == true ) {
        pinLock.lock();
        val = arduino_pins[pin].external_val;
        pinLock.unlock();
    }
    return val;
}

int32_t ArduinoBoard::harness_getPinInternalValue(int32_t pin) {
    int32_t val = -1;
    if ( harness_checkPinNumber( pin ) == true ) {
        pinLock.lock();
        val = arduino_pins[pin].internal_val;
        pinLock.unlock();
    }
    return val;
}

void ArduinoBoard::attachInterrupt (int32_t pin, isr_t isr, int32_t mode) {
    if ( harness_checkPinNumber( pin ) == true ) {
        pinLock.lock();
        arduino_pins[pin].pIsr    = isr;
        arduino_pins[pin].isrMode = mode;
        pinLock.unlock();
    }
}

int32_t ArduinoBoard::digitalPinToInterrupt ( int32_t pin ) {
    int32_t pinIrq = -1;
    if ( harness_checkPinNumber( pin ) == true ) {
        pinIrq = pin;
    }
    return pinIrq;
}

void ArduinoBoard::delayMicroseconds ( int32_t usec ) {
    //std::cout << HARNESS_PREFIX"Sleeping " << usec << " us..." << std::endl;

    pinLock.lock();
    useconds_t sleepTime = static_cast<useconds_t>(usec * timeScale);
    pinLock.unlock();

    emit boardEvent(SLEEP_EVENT, HIGH);

    usleep(static_cast<useconds_t>(sleepTime));

    emit boardEvent(SLEEP_EVENT, LOW);
}

void ArduinoBoard::harness_setTimeScale ( double scale ) {
    pinLock.lock();
    timeScale = scale;
    pinLock.unlock();
}

uint32_t ArduinoBoard::harness_getTimePassedFromBoot( void ) {
    return millis();
}

void ArduinoBoard::harness_setTimePassedFromBoot(const uint32_t timePassedFromBoot) {
    ( void ) timePassedFromBoot;
    // unused function
}

bool ArduinoBoard::harness_checkPinNumber ( const int32_t pin ) const {
    return ( ( pin >= 0 ) && ( pin < N_PINS ) );
}
