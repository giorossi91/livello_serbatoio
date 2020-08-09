#include "arduinoboard.h"
#include <ctime>

ArduinoBoard arduino;

const int32_t ArduinoBoard::SLEEP_EVENT = 99;

Pin_t ArduinoBoard::arduino_pins[] = {
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

ArduinoBoard::ArduinoBoard( void ) {
    timeScale = 1.0;
}

int32_t ArduinoBoard::max(int32_t a, int32_t b) {
    return (a > b) ? a : b;
}

int32_t ArduinoBoard::millis ( void ) {
    int32_t ms = 0;

    struct timespec curr_time;
    int ret = clock_gettime(CLOCK_MONOTONIC_RAW, &curr_time);
    if(ret == 0) {
        ms = static_cast<int32_t>(curr_time.tv_sec * 1000) + static_cast<int32_t>(curr_time.tv_nsec / 1000000);
    } else {
        ms = 0;
    }

    pinLock.lock();
    ms *= (1.0 / timeScale);
    pinLock.unlock();

    return ms;
}

void ArduinoBoard::delay ( int32_t ms ) {
    std::cout << HARNESS_PREFIX"Sleeping " << ms << " ms..." << std::endl;
    pinLock.lock();
    useconds_t sleepTime = static_cast<useconds_t>(ms * 1000 * timeScale);
    pinLock.unlock();

#ifdef QT_CORE_LIB
    emit boardEvent(SLEEP_EVENT, HIGH);
#endif

    usleep(sleepTime);

#ifdef QT_CORE_LIB
    emit boardEvent(SLEEP_EVENT, LOW);
#endif
}

void ArduinoBoard::digitalWrite ( int32_t pin, int32_t v ) {
    std::cout << HARNESS_PREFIX"DIG-O[" << std::setw(4) << pin << "] v = " << v << std::endl;

    pinLock.lock();
    arduino_pins[pin].in_val = v;
    pinLock.unlock();

#ifdef QT_CORE_LIB
    emit boardEvent(pin, v);
#endif

}

int32_t ArduinoBoard::digitalRead ( int32_t pin ) {
    pinLock.lock();
    int32_t v = arduino_pins[pin].out_val;
    pinLock.unlock();

    std::cout << HARNESS_PREFIX"DIG-I[" << std::setw(4) << pin << "] v = " << v << std::endl;

#ifdef QT_CORE_LIB
    emit boardEvent(pin, v);
#endif

    return v;
}

void ArduinoBoard::pinMode ( int32_t pin, int32_t mode ) {
    pinLock.lock();
    arduino_pins[pin].mode = mode;
    pinLock.unlock();
}

uint32_t ArduinoBoard::pulseIn ( int32_t pin, int32_t mode, int32_t timeout ) {
    ( void ) mode;
    ( void ) timeout;

    pinLock.lock();
    uint32_t v = arduino_pins[pin].pulse_time;
    pinLock.unlock();

    std::cout << HARNESS_PREFIX"INPUT[" << std::setw(4) << pin << "] t = " << v << " us" << std::endl;

    return v;
}

void ArduinoBoard::setPulseTime(int32_t pin, uint32_t time) {
    pinLock.lock();
    arduino_pins[pin].pulse_time = time;
    pinLock.unlock();
}

void ArduinoBoard::setPinValue(int32_t pin, int32_t mode, int32_t value)
{
    pinLock.lock();
    if(mode == OUTPUT) {
        arduino_pins[pin].in_val = value;
    } else if (mode == INPUT) {
        arduino_pins[pin].out_val = value;
    }
    pinLock.unlock();
}

void ArduinoBoard::attachInterrupt (int32_t pin, void (*isr)(void), int32_t mode) {
    ( void ) mode;
    pinLock.lock();
    arduino_pins[pin].pIsr = isr;
    pinLock.unlock();
}

int32_t ArduinoBoard::digitalPinToInterrupt ( int32_t pin ) {
    return pin;
}

void ArduinoBoard::delayMicroseconds ( int32_t usec ) {
    std::cout << HARNESS_PREFIX"Sleeping " << usec << " us..." << std::endl;

    pinLock.lock();
    useconds_t sleepTime = static_cast<useconds_t>(usec * timeScale);
    pinLock.unlock();

#ifdef QT_CORE_LIB
    emit boardEvent(SLEEP_EVENT, HIGH);
#endif

    usleep(static_cast<useconds_t>(sleepTime));

#ifdef QT_CORE_LIB
    emit boardEvent(SLEEP_EVENT, LOW);
#endif

}

void ArduinoBoard::setTimeScale ( double scale ) {
    pinLock.lock();
    timeScale = scale;
    pinLock.unlock();
}
