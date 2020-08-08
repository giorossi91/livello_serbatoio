#include "showevents.h"
#include "ui_showevents.h"

#include "arduinoboard.h"
#include <QDateTime>
#include <QGridLayout>
#include <cmath>

QT_CHARTS_USE_NAMESPACE

const int SLEEP_TIME_ID = 14;

ShowEvents::ShowEvents(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ShowEvents)
{
    ui->setupUi(this);

    dataChart = new Chart();

    QGridLayout *dataViewLayout = new QGridLayout(ui->d_io_chart_widget);

    dataView = new ChartView(dataChart);
    dataView->setRenderHint(QPainter::Antialiasing);
    dataViewLayout->addWidget(dataView);

    x_axis = new QDateTimeAxis();
    x_axis->setMin(QDateTime::currentDateTime().addSecs(-30 * 1));
    x_axis->setMax(QDateTime::currentDateTime().addSecs(0));
    x_axis->setFormat("HH:mm:ss.zzz");

    dataChart->addAxis(x_axis, Qt::AlignBottom);

    addSeries(0 , "D0");
    addSeries(1 , "D1");
    addSeries(2 , "D2");
    addSeries(3 , "D3");
    addSeries(4 , "D4");
    addSeries(5 , "D5");
    addSeries(6 , "D6");
    addSeries(7 , "D7");
    addSeries(8 , "D8");
    addSeries(9 , "D9");
    addSeries(10, "D10");
    addSeries(11, "D11");
    addSeries(12, "D12");
    addSeries(13, "D13");

    addSeries(SLEEP_TIME_ID, "Sleep");

    dataChart->setTitle("D/IO");

    connect(&arduino, SIGNAL(pinEvent(int32_t, int32_t)), this, SLOT(addNewPoint(int32_t, int32_t)));
    connect(&arduino, SIGNAL(sleepEvent(int32_t))       , this, SLOT(addNewSleep(int32_t))         );
}

ShowEvents::~ShowEvents()
{
    delete ui;
}

void ShowEvents::addSeries(int32_t pinId, QString seriesName)
{
    QLineSeries *series = new QLineSeries();
    series->setName(seriesName);

    QValueAxis *y_axis = new QValueAxis();
    y_axis->setMin(0);
    y_axis->setMax(20);
    y_axis->setTickCount(1);
    y_axis->hide();

    dataChart->addSeries(series);
    dataChart->addAxis(y_axis, Qt::AlignLeft);
    series->attachAxis(x_axis);
    series->attachAxis(y_axis);

    pinSeries[pinId] = series;
}

static inline int32_t offset(int32_t val, int32_t off) {
    return val + off + 1;
}

void ShowEvents::addNewPoint(int32_t pinId, int32_t val) {
    QDateTime bjtime = QDateTime::currentDateTime();

    QLineSeries *pinSerie = pinSeries[pinId];

    if(offset(val, pinId) == offset(HIGH, pinId)) {
        if(pinSerie->count() > 0 && std::fabs(pinSerie->at(pinSerie->count() - 1).y() - offset(val, pinId)) > 0.1) {
            pinSerie->append(bjtime.toMSecsSinceEpoch(), static_cast<qreal>(offset(LOW, pinId)));
        }
        pinSerie->append(bjtime.toMSecsSinceEpoch(), static_cast<qreal>(offset(HIGH, pinId)));
    } else {
        if(pinSerie->count() > 0 && std::fabs(pinSerie->at(pinSerie->count() - 1).y() - offset(val, pinId)) > 0.1) {
            pinSerie->append(bjtime.toMSecsSinceEpoch(), static_cast<qreal>(offset(HIGH, pinId)));
        }
        pinSerie->append(bjtime.toMSecsSinceEpoch(), static_cast<qreal>(offset(LOW, pinId)));
    }

    x_axis->setMin(QDateTime::currentDateTime().addSecs(-30 * 1));
    x_axis->setMax(QDateTime::currentDateTime().addSecs(0));
}

void ShowEvents::addNewSleep(int32_t usecSlept)
{
    QDateTime bjtime = QDateTime::currentDateTime();

    pinSeries[SLEEP_TIME_ID]->append(bjtime.toMSecsSinceEpoch(), static_cast<qreal>(offset(0, SLEEP_TIME_ID)));
    pinSeries[SLEEP_TIME_ID]->append(bjtime.toMSecsSinceEpoch(), static_cast<qreal>(offset(1, SLEEP_TIME_ID)));
    pinSeries[SLEEP_TIME_ID]->append(bjtime.toMSecsSinceEpoch() + (usecSlept / 1000), static_cast<qreal>(offset(1, SLEEP_TIME_ID)));
    pinSeries[SLEEP_TIME_ID]->append(bjtime.toMSecsSinceEpoch() + (usecSlept / 1000), static_cast<qreal>(offset(0, SLEEP_TIME_ID)));

    x_axis->setMin(QDateTime::currentDateTime().addSecs(-30 * 1));
    x_axis->setMax(QDateTime::currentDateTime().addSecs(0));
}
