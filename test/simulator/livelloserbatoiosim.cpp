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
#include "String.h"

// --> UUT
#define UNIT_TEST

#define DEBUG  CONF_DEBUG  //<-- release configuration
#define SENSOR SENSOR_HCSR04 //<-- sensor actually used

namespace uut {
#include "livello_serbatoio_uut.hpp"
}
// <--

LivelloSerbatoioSim::LivelloSerbatoioSim(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LivelloSerbatoioSim), randomTimer{nullptr}, linearChargeTimer{nullptr}, linearDischargeTimer{nullptr} {
    ui->setupUi(this);

    // setup arduino board to use for simulator
    harness_initBoard();

    uut::lcd_if.harness_mapSymbol(uut::PROGRESS_CHAR  , "#");
    uut::lcd_if.harness_mapSymbol(uut::ARROW_DOWN_CHAR, "V");
    uut::lcd_if.harness_mapSymbol(uut::ARROW_UP_CHAR  , "^");

    showEventsPanel = new ShowEvents(this);

    showEventsPanel->registerDigitalIO(uut::TRIG_DPIN           , "TRIG"     );
    showEventsPanel->registerDigitalIO(uut::LED_CAPACITY_DPIN   , "LED"      );
    showEventsPanel->registerDigitalIO(uut::LCD_BUTTON_DPIN     , "BUTTON"   );
    showEventsPanel->registerDigitalIO(uut::LCD_LIGHT_DPIN      , "LCD_LIGHT");
    //showEventsPanel->registerDigitalIO(Arduinoharness_getBoard()::SLEEP_EVENT, "Sleep"    );

    monitorSignal(uut::TRIG_DPIN         , 500);
    monitorSignal(uut::LED_CAPACITY_DPIN , 500);
    monitorSignal(uut::LCD_BUTTON_DPIN   , 500);
    monitorSignal(uut::LCD_LIGHT_DPIN    , 500);

    showEventsPanel->grabGesture(Qt::PanGesture);
    showEventsPanel->grabGesture(Qt::PinchGesture);
    showEventsPanel->show();

    qRegisterMetaType<std::string>();
    qRegisterMetaType<int32_t>("int32_t");

    ui->manual_button->setEnabled(false);

    ui->serial_textbrowser->setText("");
    ui->lcd_textedit->setText("");

    ui->distance_spinbox->setDecimals(2);
    ui->distance_vslider->setMinimum(-2);
    ui->distance_vslider->setMaximum(static_cast<uint32_t>(uut::TANK_HEIGHT_CM + 2));

    ui->distance_spinbox->setDecimals(2);
    ui->distance_spinbox->setSingleStep(0.2);
    ui->distance_spinbox->setMinimum(-2);
    ui->distance_spinbox->setMaximum(static_cast<uint32_t>(uut::TANK_HEIGHT_CM + 2));
    ui->distance_vslider->setValue( static_cast<uint32_t>(uut::SENSOR_DISTANCE_CM - uut::SENSOR_CALIBRATION) );

    connect(&uut::lcd_if      , SIGNAL(printTextOnLcd(std::string)) , this, SLOT(updateLcdScreen(std::string))     , Qt::QueuedConnection);
    connect(&Serial           , SIGNAL(printSerialText(std::string)), this, SLOT(updateSerialMonitor(std::string)) , Qt::QueuedConnection);
    connect(harness_getBoard(), SIGNAL(boardEvent(int32_t,int32_t)) , this, SLOT(updatePinStatus(int32_t,int32_t)) , Qt::QueuedConnection);

    QString status =  "Simulation of " VERSION " in "
#if DEBUG == CONF_DEBUG
        "DEBUG mode";
#else
        "RELEASE mode";
#endif

    ui->statusBar->showMessage(status);

    simOn = true;
    threadExit = false;
    ui->actionPause ->setEnabled(true );
    ui->actionResume->setEnabled(false);

    executor = QThread::create([=](void) {
        harness_getBoard()->harness_setPulseTime(uut::ECHO_DPIN, static_cast<uint32_t>(ui->distance_spinbox->value() * uut::SENSOR_LSB));

        uut::setup();

        while(!threadExit) {
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
    threadExit = true;

    simOnMutex.lock();
    simOn = true;
    simOnCondition.notify_all();
    simOnMutex.unlock();

    delete ui;

    delete showEventsPanel;

    delete harness_getBoard();
}

void LivelloSerbatoioSim::monitorSignal(int32_t id, int32_t periodMs) {
    showEventsPanel->addDigitalMonitor(id, periodMs, [=]( void ) {
        int32_t v = 0;

        if(harness_getBoard()->harness_getPinMode(id) == OUTPUT) {
            v = harness_getBoard()->harness_getPinInternalValue(id);
        } else {
            v = harness_getBoard()->harness_getPinExternalValue(id);
        }
        return v;
    });
}

void LivelloSerbatoioSim::updateSerialMonitor ( std::string text ) {
    ui->serial_textbrowser->moveCursor(QTextCursor::End);
    ui->serial_textbrowser->insertPlainText(QString::fromStdString(text));
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
    harness_getBoard()->harness_setPulseTime(uut::ECHO_DPIN, static_cast<uint32_t>(value * uut::SENSOR_LSB));
}


void LivelloSerbatoioSim::on_distance_spinbox_valueChanged ( double position ) {
    ui->distance_vslider->setValue(static_cast<int>(position));
    harness_getBoard()->harness_setPulseTime(uut::ECHO_DPIN, static_cast<uint32_t>(position * uut::SENSOR_LSB));
}

void LivelloSerbatoioSim::on_tankMax_button_clicked()
{
    ui->distance_spinbox->setValue(static_cast<int>(uut::TANK_HEIGHT_CM - uut::SENSOR_CALIBRATION) );
}

void LivelloSerbatoioSim::on_sensMax_button_clicked()
{
    ui->distance_spinbox->setValue(static_cast<int>(uut::SENSOR_DISTANCE_CM - uut::SENSOR_CALIBRATION) );
}

void LivelloSerbatoioSim::on_show_button_pressed ( void ) {
    harness_getBoard()->harness_setPinValue(uut::LCD_BUTTON_DPIN, INPUT, HIGH);
    const int32_t isrMode = harness_getBoard()->harness_getIsrMode(uut::LCD_BUTTON_DPIN);
    const isr_t isrFunc = harness_getBoard()->harness_getIsrFunc(uut::LCD_BUTTON_DPIN);
    if( ( ( isrMode == RISING ) ||
          ( isrMode == CHANGE ) ) &&
          ( isrFunc != nullptr ) ) {
        isrFunc();
    }
}

void LivelloSerbatoioSim::on_show_button_released ( void ) {
    harness_getBoard()->harness_setPinValue(uut::LCD_BUTTON_DPIN, INPUT, LOW);
    const int32_t isrMode = harness_getBoard()->harness_getIsrMode(uut::LCD_BUTTON_DPIN);
    const isr_t isrFunc = harness_getBoard()->harness_getIsrFunc(uut::LCD_BUTTON_DPIN);
    if( ( ( isrMode == FALLING ) ||
          ( isrMode == CHANGE ) ) &&
          ( isrFunc != nullptr ) ) {
        isrFunc();
    }
}

void LivelloSerbatoioSim::on_timescale_combobox_currentIndexChanged ( int index ) {
    switch ( index ) {
    case 0:
        harness_getBoard()->harness_setTimeScale(0.01);
        break;
    case 1:
        harness_getBoard()->harness_setTimeScale(0.1);
        break;
    case 2:
        harness_getBoard()->harness_setTimeScale(1.0);
        break;
    case 3:
        harness_getBoard()->harness_setTimeScale(10.0);
        break;
    case 4:
        harness_getBoard()->harness_setTimeScale(100.0);
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


void LivelloSerbatoioSim::on_manual_button_clicked()
{
    ui->manual_button->setEnabled( false );
    ui->distance_spinbox->setEnabled( true );
    ui->distance_vslider->setEnabled( true );

    ui->linearcharge_button->setEnabled( true );
    ui->lineardischarge_button->setEnabled( true );
    ui->random_button->setEnabled( true );

    if ( randomTimer != nullptr ) {
        randomTimer->stop();
        delete randomTimer;
        randomTimer = nullptr;
    }

    if ( linearChargeTimer != nullptr ) {
        linearChargeTimer->stop();
        delete linearChargeTimer;
        linearChargeTimer = nullptr;
    }

    if ( linearDischargeTimer != nullptr ) {
        linearDischargeTimer->stop();
        delete linearDischargeTimer;
        linearDischargeTimer = nullptr;
    }
}

void LivelloSerbatoioSim::on_random_button_clicked()
{
    ui->manual_button->setEnabled( true );
    ui->distance_spinbox->setEnabled( false );
    ui->distance_vslider->setEnabled( false );

    ui->linearcharge_button->setEnabled( true );
    ui->lineardischarge_button->setEnabled( true );
    ui->random_button->setEnabled( true );

    if ( randomTimer != nullptr ) {
        randomTimer->stop();
        delete randomTimer;
        randomTimer = nullptr;
    }

    if ( linearChargeTimer != nullptr ) {
        linearChargeTimer->stop();
        delete linearChargeTimer;
        linearChargeTimer = nullptr;
    }

    if ( linearDischargeTimer != nullptr ) {
        linearDischargeTimer->stop();
        delete linearDischargeTimer;
        linearDischargeTimer = nullptr;
    }

    randomTimer = new QTimer();
    randomTimer->setInterval( 1000 );
    randomTimer->setSingleShot( false );
    randomTimer->callOnTimeout ( [&]() {
        const double value = static_cast<double>( QRandomGenerator::system()->bounded( 0,  static_cast<int>(uut::TANK_HEIGHT_CM)));
        ui->distance_spinbox->setValue( value );
    });
    randomTimer->start( );
}

void LivelloSerbatoioSim::on_linearcharge_button_clicked()
{
    ui->manual_button->setEnabled( true );
    ui->distance_spinbox->setEnabled( false );
    ui->distance_vslider->setEnabled( false );

    ui->linearcharge_button->setEnabled( true );
    ui->lineardischarge_button->setEnabled( true );
    ui->random_button->setEnabled( true );

    if ( randomTimer != nullptr ) {
        randomTimer->stop();
        delete randomTimer;
        randomTimer = nullptr;
    }

    if ( linearChargeTimer != nullptr ) {
        linearChargeTimer->stop();
        delete linearChargeTimer;
        linearChargeTimer = nullptr;
    }

    if ( linearDischargeTimer != nullptr ) {
        linearDischargeTimer->stop();
        delete linearDischargeTimer;
        linearDischargeTimer = nullptr;
    }

    linearChargeTimer = new QTimer();
    linearChargeTimer->setInterval( 30 * 1000 );
    linearChargeTimer->setSingleShot( false );
    linearChargeTimer->callOnTimeout ( [&]() {
        double current = ui->distance_spinbox->value();
        if ( current > uut::SENSOR_DISTANCE_CM )
        {
            current -= 0.2;
        }

        ui->distance_spinbox->setValue( current );
    });
    linearChargeTimer->start( );
}

void LivelloSerbatoioSim::on_lineardischarge_button_clicked()
{
    ui->manual_button->setEnabled( true );
    ui->distance_spinbox->setEnabled( false );
    ui->distance_vslider->setEnabled( false );

    ui->linearcharge_button->setEnabled( true );
    ui->lineardischarge_button->setEnabled( true );
    ui->random_button->setEnabled( true );

    if ( randomTimer != nullptr ) {
        randomTimer->stop();
        delete randomTimer;
        randomTimer = nullptr;
    }

    if ( linearChargeTimer != nullptr ) {
        linearChargeTimer->stop();
        delete linearChargeTimer;
        linearChargeTimer = nullptr;
    }

    if ( linearDischargeTimer != nullptr ) {
        linearDischargeTimer->stop();
        delete linearDischargeTimer;
        linearDischargeTimer = nullptr;
    }

    linearDischargeTimer = new QTimer();
    linearDischargeTimer->setInterval( 30 * 1000 );
    linearDischargeTimer->setSingleShot( false );
    linearDischargeTimer->callOnTimeout ( [&]() {
        double current = ui->distance_spinbox->value();
        if ( current < uut::TANK_HEIGHT_CM )
        {
            current += 0.2;
        }

        ui->distance_spinbox->setValue( current );
    });
    linearDischargeTimer->start( );
}
