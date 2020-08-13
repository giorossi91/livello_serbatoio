#include "livelloserbatoiosim.h"
#include "ui_livelloserbatoiosim.h"

#include <string>
#include <cstdint>
#include <cmath>

#include <QScrollBar>
#include <QMutex>
#include <QWaitCondition>

// dependencies
#include "arduino_types.h"
#include "arduino_stubs.h"
#include "arduinoboard.h"
#include "Serial.h"
#include "LiquidCrystal.h"

// --> UUT
#define UNIT_TEST

#define DEBUG  CONF_DEBUG  //<-- release configuration
#define SENSOR SENSOR_HCSR04 //<-- sensor actually used

namespace uut {
#include "livello_serbatoio_uut.hpp"
}
// <--

const int LivelloSerbatoioSim::DISTANCE_LSB = 58;

LivelloSerbatoioSim::LivelloSerbatoioSim(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LivelloSerbatoioSim) {
    ui->setupUi(this);

    showEventsPanel = new ShowEvents(this);

    showEventsPanel->registerDigitalIO(uut::TRIG_DPIN           , "TRIG"     );
    showEventsPanel->registerDigitalIO(uut::LED_CAPACITY_DPIN   , "LED"      );
    showEventsPanel->registerDigitalIO(uut::LCD_BUTTON_DPIN     , "BUTTON"   );
    showEventsPanel->registerDigitalIO(uut::LCD_LIGHT_DPIN      , "LCD_LIGHT");
    showEventsPanel->registerDigitalIO(ArduinoBoard::SLEEP_EVENT, "Sleep"    );

    monitorSignal(uut::TRIG_DPIN         , 500);
    monitorSignal(uut::LED_CAPACITY_DPIN , 500);
    monitorSignal(uut::LCD_BUTTON_DPIN   , 500);
    monitorSignal(uut::LCD_LIGHT_DPIN    , 500);

    showEventsPanel->grabGesture(Qt::PanGesture);
    showEventsPanel->grabGesture(Qt::PinchGesture);
    showEventsPanel->show();

    qRegisterMetaType<std::string>();
    qRegisterMetaType<int32_t>("int32_t");

    ui->serial_textbrowser->setText("");
    ui->lcd_textedit->setText("");

    ui->distance_vslider->setMinimum(0);
    ui->distance_vslider->setMaximum(static_cast<uint32_t>(uut::TANK_HEIGHT_CM + 2));

    ui->distance_spinbox->setMinimum(0);
    ui->distance_spinbox->setMaximum(static_cast<uint32_t>(uut::TANK_HEIGHT_CM + 2));

    connect(&uut::lcd, SIGNAL(printTextOnLcd(std::string)) , this, SLOT(updateLcdScreen(std::string))     , Qt::QueuedConnection);
    connect(&Serial  , SIGNAL(printSerialText(std::string)), this, SLOT(updateSerialMonitor(std::string)) , Qt::QueuedConnection);
    connect(&arduino , SIGNAL(boardEvent(int32_t, int32_t)), this, SLOT(updatePinStatus(int32_t, int32_t)), Qt::QueuedConnection);

    QString status =  "Simulation of " VERSION " in "
#if DEBUG == CONF_DEBUG
        "DEBUG mode";
#else
        "RELEASE mode";
#endif

    ui->statusBar->showMessage(status);

    simOn = true;
    ui->actionPause ->setEnabled(true );
    ui->actionResume->setEnabled(false);

    executor = QThread::create([=](void) {
        arduino.setPulseTime(uut::ECHO_DPIN, static_cast<uint32_t>(ui->distance_spinbox->value() * DISTANCE_LSB));

        uut::setup();

        while(1) {
            simOnMutex.lock();
            while(simOn == false) {
                simOnCondition.wait(&simOnMutex);
            }
            simOnMutex.unlock();

            uut::loop();
        }
    });

    executor->start();
}

LivelloSerbatoioSim::~LivelloSerbatoioSim ( void ) {
    delete ui;

    delete showEventsPanel;
}

void LivelloSerbatoioSim::monitorSignal(int32_t id, int32_t periodMs) {
    showEventsPanel->addDigitalMonitor(id, periodMs, [=]( void ) {
        int32_t v = 0;
        arduino.pinLock.lock();
        if(ArduinoBoard::arduino_pins[id].mode == OUTPUT) {
            v = ArduinoBoard::arduino_pins[id].in_val;
        } else {
            v = ArduinoBoard::arduino_pins[id].out_val;
        }
        arduino.pinLock.unlock();
        return v;
    });
}

void LivelloSerbatoioSim::updateSerialMonitor ( std::string text ) {
    ui->serial_textbrowser->setPlainText(ui->serial_textbrowser->toPlainText() + QString::fromStdString(text));
    ui->serial_textbrowser->verticalScrollBar()->setValue(ui->serial_textbrowser->verticalScrollBar()->maximum());
}

void LivelloSerbatoioSim::updateLcdScreen ( std::string text ) {
    ui->lcd_textedit->setText(QString::fromStdString(text));
}

void LivelloSerbatoioSim::updatePinStatus ( int32_t pin, int32_t value ) {
    // update ui
    if(pin == uut::LCD_LIGHT_DPIN && value == HIGH) {
        ui->lcd_textedit->setStyleSheet("border: 1px solid rgb(0,255,0);");
    } else if (pin == uut::LCD_LIGHT_DPIN && value == LOW) {
        ui->lcd_textedit->setStyleSheet("border: 1px solid black;");
    }

    if(pin == uut::LED_CAPACITY_DPIN && value == HIGH) {
        ui->led_label->setStyleSheet("border-radius:20px; background-color:red;");
    } else if(pin == uut::LED_CAPACITY_DPIN && value == LOW) {
        ui->led_label->setStyleSheet("border-radius:20px; background-color:black;");
    }
}

void LivelloSerbatoioSim::on_distance_vslider_valueChanged ( int value ) {
    ui->distance_spinbox->setValue(value);
    arduino.setPulseTime(uut::ECHO_DPIN, static_cast<uint32_t>(value * DISTANCE_LSB));
}


void LivelloSerbatoioSim::on_distance_spinbox_valueChanged ( double position ) {
    ui->distance_vslider->setValue(static_cast<int>(position));
    arduino.setPulseTime(uut::ECHO_DPIN, static_cast<uint32_t>(position * DISTANCE_LSB));
}

void LivelloSerbatoioSim::on_show_button_pressed ( void ) {
    arduino.setPinValue(uut::LCD_BUTTON_DPIN, INPUT, HIGH);
    if(ArduinoBoard::arduino_pins[uut::LCD_BUTTON_DPIN].pIsr != nullptr) {
        ArduinoBoard::arduino_pins[uut::LCD_BUTTON_DPIN].pIsr();
    }
}

void LivelloSerbatoioSim::on_show_button_released ( void ) {
    arduino.setPinValue(uut::LCD_BUTTON_DPIN, INPUT, LOW);
}

void LivelloSerbatoioSim::on_timescale_combobox_currentIndexChanged ( int index ) {
    switch ( index ) {
    case 0:
        arduino.setTimeScale(0.01);
        break;
    case 1:
        arduino.setTimeScale(0.1);
        break;
    case 2:
        arduino.setTimeScale(1.0);
        break;
    case 3:
        arduino.setTimeScale(10.0);
        break;
    case 4:
        arduino.setTimeScale(100.0);
        break;
    }
}

void LivelloSerbatoioSim::on_actionPause_triggered ( void ) {
    simOnMutex.lock();
    simOn = false;
    simOnMutex.unlock();

    showEventsPanel->pauseMonitors();

    ui->actionPause ->setEnabled(false);
    ui->actionResume->setEnabled(true);
}

void LivelloSerbatoioSim::on_actionResume_triggered ( void ) {
    simOnMutex.lock();
    simOn = true;
    simOnMutex.unlock();
    simOnCondition.notify_all();

    showEventsPanel->resumeMonitors();

    ui->actionPause ->setEnabled(true );
    ui->actionResume->setEnabled(false);
}

void LivelloSerbatoioSim::on_actionShow_Digital_I_O_triggered ( void )
{
    showEventsPanel->show();
}
