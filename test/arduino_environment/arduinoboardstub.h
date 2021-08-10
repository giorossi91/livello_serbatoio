#ifndef ARDUINOBOARDSTUB_H
#define ARDUINOBOARDSTUB_H

#include "arduinoboard.h"

#include <atomic>

extern std::atomic<uint32_t> timePassedFromBootMs;

class ArduinoBoardStub : public ArduinoBoard
{
public:
    ArduinoBoardStub ( void );

    virtual uint32_t millis( void ) override;
};

#endif // ARDUINOBOARDSTUB_H
