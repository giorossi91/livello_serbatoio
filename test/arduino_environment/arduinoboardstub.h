#ifndef ARDUINOBOARDSTUB_H
#define ARDUINOBOARDSTUB_H

#include "arduinoboard.h"

#include <atomic>

class ArduinoBoardStub : public ArduinoBoard
{
public:
    ArduinoBoardStub ( void );

    virtual uint32_t millis ( void ) override;

    virtual void harness_setTimePassedFromBoot ( const uint32_t timePassedFromBoot ) override;

    virtual uint32_t harness_getTimePassedFromBoot ( void ) override;

private:
    std::atomic<uint32_t> timePassedFromBootMs;

};

#endif // ARDUINOBOARDSTUB_H
