QT += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simulator
TEMPLATE = app

LIBS += -L$$PWD/../deploy/arduino_environment/lib -larduino_environment
INCLUDEPATH += $$PWD/../deploy/arduino_environment/inc $$PWD/../..

QMAKE_CXXFLAGS += -Wno-deprecated-copy
QMAKE_CXXFLAGS += -Wno-template-id-cdtor

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
        chart.cpp \
        chartview.cpp \
        main.cpp \
        livelloserbatoiosim.cpp \
        showevents.cpp

HEADERS += \
        chart.h \
        chartview.h \
        $$PWD/../../livello_serbatoio.ino \
        livelloserbatoiosim.h \
        showevents.h

FORMS += \
        livelloserbatoiosim.ui \
        showevents.ui
