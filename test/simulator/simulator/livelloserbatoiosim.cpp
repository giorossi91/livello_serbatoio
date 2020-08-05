#include "livelloserbatoiosim.h"
#include "ui_livelloserbatoiosim.h"

#include <string>
#include <cstdint>
#include <cmath>

#include <QScrollBar>

// dependencies
#include "arduino_types.h"
#include "arduino_stubs.h"
#include "arduinoboard.h"
#include "Serial.h"
#include "LiquidCrystal.h"

// --> UUT
#define UNIT_TEST

#define DEBUG  CONF_RELEASE  //<-- release configuration
#define SENSOR SENSOR_HCSR04 //<-- sensor actually used

namespace uut {
#include "livello_serbatoio_uut.hpp"
}
// <--

const int LivelloSerbatoioSim::SCHEDULER_PERIOD_MS = 500;
const int LivelloSerbatoioSim::DISTANCE_LSB        = 58;

LivelloSerbatoioSim::LivelloSerbatoioSim(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LivelloSerbatoioSim)
{
    ui->setupUi(this);

    qRegisterMetaType<std::string>();
    qRegisterMetaType<int32_t>("int32_t");


    ui->serial_textbrowser->setText("");
    ui->lcd_textedit->setText("");

    connect(&uut::lcd, SIGNAL(printTextOnLcd(std::string)) , this, SLOT(updateLcdScreen(std::string))     , Qt::QueuedConnection);
    connect(&Serial  , SIGNAL(printSerialText(std::string)), this, SLOT(updateSerialMonitor(std::string)) , Qt::QueuedConnection);
    connect(&arduino , SIGNAL(pinWritten(int32_t, int32_t)), this, SLOT(updatePinStatus(int32_t, int32_t)), Qt::QueuedConnection);


    executor = QThread::create([=](void) {
        ArduinoBoard::arduino_pins[uut::ECHO_DPIN].pulse_time = ui->distance_spinbox->value() * DISTANCE_LSB;
        uut::setup();

        while(1) {
            uut::loop();
        }
    });

    executor->start();
}

LivelloSerbatoioSim::~LivelloSerbatoioSim()
{
    delete ui;
}

void LivelloSerbatoioSim::updateSerialMonitor(std::string text)
{
    ui->serial_textbrowser->setPlainText(ui->serial_textbrowser->toPlainText() + QString::fromStdString(text));
    ui->serial_textbrowser->verticalScrollBar()->setValue(ui->serial_textbrowser->verticalScrollBar()->maximum());
}

void LivelloSerbatoioSim::updateLcdScreen(std::string text)
{
    ui->lcd_textedit->setText(QString::fromStdString(text));
}

void LivelloSerbatoioSim::updatePinStatus(int32_t pin, int32_t value)
{
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


void LivelloSerbatoioSim::on_distance_vslider_sliderMoved(int position)
{
    ui->distance_spinbox->setValue(position);
    ArduinoBoard::arduino_pins[uut::ECHO_DPIN].pulse_time = position * DISTANCE_LSB;
}

void LivelloSerbatoioSim::on_distance_spinbox_valueChanged(double position)
{
    ui->distance_vslider->setValue(static_cast<int>(position));
    ArduinoBoard::arduino_pins[uut::ECHO_DPIN].pulse_time = position * DISTANCE_LSB;
}

void LivelloSerbatoioSim::on_show_button_pressed()
{
    ArduinoBoard::arduino_pins[uut::LCD_BUTTON_DPIN].out_val = HIGH;
    if(ArduinoBoard::arduino_pins[uut::LCD_BUTTON_DPIN].pIsr != nullptr) {
        ArduinoBoard::arduino_pins[uut::LCD_BUTTON_DPIN].pIsr();
    }
}

void LivelloSerbatoioSim::on_show_button_released()
{
    ArduinoBoard::arduino_pins[uut::LCD_BUTTON_DPIN].out_val = LOW;
}
