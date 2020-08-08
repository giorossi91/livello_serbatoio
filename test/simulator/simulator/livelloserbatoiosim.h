#ifndef LIVELLOSERBATOIOSIM_H
#define LIVELLOSERBATOIOSIM_H

#include <QMainWindow>
#include <QThread>

#include <showevents.h>

#include <string>
#include <atomic>

Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(int32_t)


namespace Ui {
class LivelloSerbatoioSim;
}

class LivelloSerbatoioSim : public QMainWindow {
    Q_OBJECT

public:
    explicit LivelloSerbatoioSim  ( QWidget *parent = nullptr );
             ~LivelloSerbatoioSim ( void );

private:
    Ui::LivelloSerbatoioSim *ui;

    QThread *executor;

    std::atomic<bool> simOn;

    static const int DISTANCE_LSB;

    ShowEvents *showEventsPanel;

private slots:
    void updateSerialMonitor(std::string text);
    void updateLcdScreen    (std::string text);
    void updatePinStatus    (int32_t pin, int32_t value);

    void on_distance_vslider_valueChanged         ( int value       );
    void on_distance_spinbox_valueChanged         ( double position );
    void on_show_button_pressed                   ( void            );
    void on_show_button_released                  ( void            );
    void on_timescale_combobox_currentIndexChanged( int index       );
    void on_actionPause_triggered            ( void );
    void on_actionResume_triggered           ( void );
    void on_actionShow_Digital_I_O_triggered ( void );
};

#endif // LIVELLOSERBATOIOSIM_H
