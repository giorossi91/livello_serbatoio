QT -= gui

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Serial.cpp \
    arduino_stubs.cpp \
    arduinoboard.cpp \
    arduinoboardstub.cpp

HEADERS += \
    LiquidCrystal.h \
    Serial.h \
    arduino_stubs.h \
    arduino_types.h \
    arduinoboard.h \
    arduinoboardstub.h

# Default rules for deployment.
unix {
    target.path = $$PWD/../deploy/arduino_environment
}
!isEmpty(target.path): INSTALLS += target
