QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

LIBS += -lcppunit -lgcov -lpthread
LIBS += -L$$PWD/../deploy/arduino_environment/lib -larduino_environment

INCLUDEPATH += $$PWD/../deploy/arduino_environment/inc

QMAKE_CXXFLAGS += -O0 -g2 -ftest-coverage -fprofile-arcs -Wno-keyword-macro

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        livello_serbatoio_test.cpp \
        livello_serbatoio_testapp.cpp

HEADERS += \
    livello_serbatoio_test.h \
    livello_serbatoio_uut.hpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    runUnitTest.sh
