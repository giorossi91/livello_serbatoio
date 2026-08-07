QT -= gui

TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++11

SOURCES += \
    LiquidCrystal.cpp \
    Serial.cpp \
    String.cpp \
    arduino_stubs.cpp \
    arduinoboard.cpp \
    arduinoboardstub.cpp

HEADERS += \
    LiquidCrystal.h \
    Serial.h \
    String.h \
    arduino_stubs.h \
    arduino_types.h \
    arduinoboard.h \
    arduinoboardstub.h

# Deployment rules for unix env
unix {
    DEPLOY_BASE = $$absolute_path($$PWD/../deploy/arduino_environment)

    # 1. static lib
    static_lib_deploy.files = $$OUT_PWD/lib$${TARGET}.a
    static_lib_deploy.path = $$DEPLOY_BASE/lib

    # 2. headers
    headers_deploy.files = $$HEADERS
    headers_deploy.path = $$DEPLOY_BASE/inc

    INSTALLS += static_lib_deploy headers_deploy

    # install
    QMAKE_POST_LINK += $(MAKE) -f $(MAKEFILE) install
}
