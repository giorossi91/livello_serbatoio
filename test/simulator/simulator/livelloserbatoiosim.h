#ifndef LIVELLOSERBATOIOSIM_H
#define LIVELLOSERBATOIOSIM_H

#include <QMainWindow>
#include <QThread>

#include <string>

Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(int32_t)


namespace Ui {
class LivelloSerbatoioSim;
}

class LivelloSerbatoioSim : public QMainWindow
{
    Q_OBJECT

public:
    explicit LivelloSerbatoioSim(QWidget *parent = nullptr);
    ~LivelloSerbatoioSim();

private:
    Ui::LivelloSerbatoioSim *ui;

    QThread *executor;

    static const int SCHEDULER_PERIOD_MS;
    static const int DISTANCE_LSB;

private slots:
    void updateSerialMonitor(std::string text);
    void updateLcdScreen    (std::string text);
    void updatePinStatus    (int32_t pin, int32_t value);

    void on_distance_vslider_sliderMoved(int position);
    void on_distance_spinbox_valueChanged(double position);
    void on_show_button_pressed();
    void on_show_button_released();
};

#endif // LIVELLOSERBATOIOSIM_H
