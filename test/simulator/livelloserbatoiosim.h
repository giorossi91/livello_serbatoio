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
             ~LivelloSerbatoioSim ( void ) override;

private:
    Ui::LivelloSerbatoioSim *ui;

    QThread *executor;

    bool               simOn;
    QMutex             simOnMutex;
    QWaitCondition     simOnCondition;
    std::atomic <bool> threadExit;
    ShowEvents *showEventsPanel;

    QTimer             *randomTimer;
    QTimer             *linearChargeTimer;
    QTimer             *linearDischargeTimer;


    void monitorSignal(int32_t id, int32_t periodMs);

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
    void on_tankMax_button_clicked();
    void on_sensMax_button_clicked();
    void on_manual_button_clicked();
    void on_random_button_clicked();
    void on_linearcharge_button_clicked();
    void on_lineardischarge_button_clicked();
};

#endif // LIVELLOSERBATOIOSIM_H
