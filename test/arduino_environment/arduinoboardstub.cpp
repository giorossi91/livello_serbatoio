#include "arduinoboardstub.h"

ArduinoBoardStub::ArduinoBoardStub ( void ) : ArduinoBoard() {
}

uint32_t ArduinoBoardStub::millis ( void ) {
    return this->timePassedFromBootMs;
}

void ArduinoBoardStub::harness_setTimePassedFromBoot(const uint32_t timePassedFromBoot) {
    this->timePassedFromBootMs = timePassedFromBoot;
}

uint32_t ArduinoBoardStub::harness_getTimePassedFromBoot( void ) {
    return this->timePassedFromBootMs.load();
}
