#ifndef SHOWEVENTS_H
#define SHOWEVENTS_H

#include <QMainWindow>
#include <chart.h>

#include <chartview.h>
#include <QtCharts/QtCharts>
#include <QtCharts/QLineSeries>
#include <QDateTimeAxis>
#include <QValueAxis>

#include <map>

namespace Ui {
class ShowEvents;
}

class ShowEvents : public QMainWindow
{
    Q_OBJECT

public:
    explicit ShowEvents(QWidget *parent = nullptr);
    ~ShowEvents();

private:

    void addSeries(int32_t pinId, QString seriesName);


    Ui::ShowEvents *ui;

    std::map<int32_t, QLineSeries *> pinSeries;

    Chart         *dataChart;
    ChartView     *dataView;
    QDateTimeAxis *x_axis;

public slots:
    void addNewPoint(int32_t pinId, int32_t val);
    void addNewSleep(int32_t usecSlept);
};

#endif // SHOWEVENTS_H
