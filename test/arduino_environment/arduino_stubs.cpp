#include <arduino_stubs.h>
#include "arduinoboard.h"

static ArduinoBoard *board = nullptr;

int32_t max(int32_t a, int32_t b) {
    int32_t r = -1;
    if ( board != nullptr ) {
        r = board->max(a,b);
    } else {
        std::cerr << "Arduino Board is not initialized!" << std::endl;
        r = -1;
    }
    return r;
}

uint32_t millis ( void ) {
    uint32_t r = 0U;
    if ( board != nullptr ) {
        r = board->millis();
    } else {
        std::cerr << "Arduino Board is not initialized!" << std::endl;
    }
    return r;
}

void delay ( int32_t ms ) {
    if ( board != nullptr ) {
        board->delay(ms);
    } else {
        std::cerr << "Arduino Board is not initialized!" << std::endl;
    }
}

void digitalWrite ( int32_t pin, int32_t v ) {
    if ( board != nullptr ) {
        board->digitalWrite(pin, v);
    } else {
        std::cerr << "Arduino Board is not initialized!" << std::endl;
    }
}

int32_t digitalRead ( int32_t pin ) {
    int32_t r = -1;
    if ( board != nullptr ) {
        r = board->digitalRead(pin);
    } else {
        std::cerr << "Arduino Board is not initialized!" << std::endl;
    }
    return r;
}

void pinMode ( int32_t pin, int32_t mode ) {
    if ( board != nullptr ) {
        board->pinMode ( pin, mode );
    } else {
        std::cerr << "Arduino Board is not initialized!" << std::endl;
    }
}

uint32_t pulseIn ( int32_t pin, int32_t mode, int32_t timeout ) {
    uint32_t r = 0U;
    if ( board != nullptr ) {
        r = board->pulseIn ( pin, mode, timeout );
    } else {
        std::cerr << "Arduino Board is not initialized!" << std::endl;
    }
    return r;
}

void attachInterrupt (int32_t pin, void (*isr)(void), int32_t mode) {
    if ( board != nullptr ) {
        board->attachInterrupt ( pin, isr, mode );
    } else {
        std::cerr << "Arduino Board is not initialized!" << std::endl;
    }
}

int32_t digitalPinToInterrupt ( int32_t pin ) {
    int32_t r = -1;
    if ( board != nullptr ) {
        r = board->digitalPinToInterrupt ( pin );
    } else {
        std::cerr << "Arduino Board is not initialized!" << std::endl;
    }
    return r;
}

void delayMicroseconds ( int32_t usec ) {
    if ( board != nullptr ) {
        board->delayMicroseconds ( usec );
    } else {
        std::cerr << "Arduino Board is not initialized!" << std::endl;
    }
}

void harness_initBoard ( void ) {
    if ( board == nullptr ) {
        board = new ArduinoBoard();
    }
}

void harness_setBoard ( ArduinoBoard *newBoard ) {
    if ( newBoard != nullptr ) {
        board = newBoard;
    }
}

ArduinoBoard *harness_getBoard ( void ) {
    return board;
}
