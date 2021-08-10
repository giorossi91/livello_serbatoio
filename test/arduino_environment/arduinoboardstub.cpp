#include "arduinoboardstub.h"

std::atomic<uint32_t> timePassedFromBootMs;

ArduinoBoardStub::ArduinoBoardStub ( void ) : ArduinoBoard() {
}

uint32_t ArduinoBoardStub::millis ( void ) {
    return timePassedFromBootMs;
}
