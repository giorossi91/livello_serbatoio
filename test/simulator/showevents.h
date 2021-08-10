#ifndef SHOWEVENTS_H
#define SHOWEVENTS_H

#include <QMainWindow>
#include <chart.h>

#include <chartview.h>
#include <QtCharts/QtCharts>
#include <QtCharts/QLineSeries>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QTimer>
#include <QMutex>

#include <map>

namespace Ui {
class ShowEvents;
}

class ShowEvents : public QMainWindow
{
    Q_OBJECT

public:
    explicit ShowEvents(QWidget *parent = nullptr);
    ~ShowEvents() override;

    void registerDigitalIO(int32_t id, QString seriesName);

    template<typename Functor>
    void addDigitalMonitor(int32_t id, int32_t periodMs, Functor && monitorFunction) {
        QTimer *t = new QTimer();
        t->setInterval(periodMs);
        t->setSingleShot(false);
        t->callOnTimeout([=]( void ) {
           addNewPoint(id, monitorFunction());
        });
        t->start();

        monitors << t;
    }

    void pauseMonitors ( void );
    void resumeMonitors( void );

private:

    void addSeries(int32_t id, QString seriesName);
    void handleMarkerClicked ( void );

    QMutex addDataMutex;

    Ui::ShowEvents *ui;

    std::map<int32_t, QLineSeries *> seriesMap;
    std::map<int32_t, int32_t>       idMap;
    std::atomic<int32_t>             uid;

    Chart          *dataChart;
    ChartView      *dataView;
    QDateTimeAxis  *x_axis;
    qreal          current_y_max;
    QList<QTimer*> monitors;

    static const int time_offset_sec;

public slots:
    void addNewPoint(int32_t id, int32_t val);
};

#endif // SHOWEVENTS_H
