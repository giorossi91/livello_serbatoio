#include "showevents.h"
#include "ui_showevents.h"

#include "arduino_stubs.h"
#include "arduinoboard.h"
#include <QDateTime>
#include <QGridLayout>
#include <cmath>

QT_CHARTS_USE_NAMESPACE

const int ShowEvents::time_offset_sec = 30;

ShowEvents::ShowEvents(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ShowEvents)
{
    ui->setupUi(this);

    uid = 0;

    current_y_max = 2;

    dataChart = new Chart();

    QGridLayout *dataViewLayout = new QGridLayout(ui->d_io_chart_widget);

    dataView = new ChartView(dataChart);
    dataView->setRenderHint(QPainter::Antialiasing);
    dataViewLayout->addWidget(dataView);

    x_axis = new QDateTimeAxis();
    x_axis->setMin(QDateTime::currentDateTime().addSecs(-time_offset_sec * 1));
    x_axis->setMax(QDateTime::currentDateTime().addSecs(0));
    x_axis->setFormat("HH:mm:ss.zzz");

    dataChart->addAxis(x_axis, Qt::AlignBottom);
    dataChart->setTitle("Events");

    connect(board, SIGNAL(boardEvent(int32_t,int32_t)), this, SLOT(addNewPoint(int32_t,int32_t)));
}

ShowEvents::~ShowEvents ( void ) {
    for (auto t : qAsConst(monitors)) {
        t->stop();
        delete t;
    }

    delete ui;
}

void ShowEvents::registerDigitalIO ( int32_t id, QString seriesName ) {
    addSeries(id, seriesName);
}

void ShowEvents::pauseMonitors ( void )
{
    for (auto t : qAsConst(monitors)) {
        t->stop();
    }
}

void ShowEvents::resumeMonitors ( void )
{
    for (auto t : qAsConst(monitors)) {
        t->start();
    }
}

void ShowEvents::addSeries ( int32_t id, QString seriesName ) {
    QLineSeries *series = new QLineSeries();
    series->setName(seriesName);

    QValueAxis *y_axis = new QValueAxis();
    y_axis->setMin(0);
    y_axis->setMax(current_y_max);
    y_axis->setTickCount(1);
    y_axis->hide();

    dataChart->addSeries(series);
    dataChart->addAxis(y_axis, Qt::AlignLeft);
    series->attachAxis(x_axis);
    series->attachAxis(y_axis);

    seriesMap[id] = series;
    idMap    [id] = uid.load() << 1;
    uid++;

    current_y_max = idMap[id] + 1;
    for(auto axes : dataChart->axes(Qt::Vertical)) {
        axes->setMax(current_y_max);
    }

    // Connect all markers to handler
    const auto markers = dataChart->legend()->markers();
    for (QLegendMarker *marker : markers) {
        // Disconnect possible existing connection to avoid multiple connections
        QObject::disconnect(marker, &QLegendMarker::clicked, this, &ShowEvents::handleMarkerClicked);
        QObject::connect(marker, &QLegendMarker::clicked, this, &ShowEvents::handleMarkerClicked);
    }
}

static inline int32_t offset ( int32_t val, int32_t off ) {
    return (val + off);
}

void ShowEvents::addNewPoint ( int32_t id, int32_t val ) {
    addDataMutex.lock();
    if( ( seriesMap.count(id) > 0 ) && ( idMap.count(id) > 0 ) ) {
        QDateTime now    = QDateTime::currentDateTime();
        qint64    timeMs = now.toMSecsSinceEpoch();

        QLineSeries *series = seriesMap[id];
        int32_t     offval  = idMap[id];

        if(offset(val, offval) == offset(HIGH, offval)) {
            if(series->count() > 0 && std::fabs(series->at(series->count() - 1).y() - offset(val, offval)) > 0.1) {
                series->append(static_cast<qreal>(timeMs), static_cast<qreal>(offset(LOW, offval)));
            }
            series->append(static_cast<qreal>(timeMs), static_cast<qreal>(offset(HIGH, offval)));
        } else {
            if(series->count() > 0 && std::fabs(series->at(series->count() - 1).y() - offset(val, offval)) > 0.1) {
                series->append(static_cast<qreal>(timeMs), static_cast<qreal>(offset(HIGH, offval)));
            }
            series->append(static_cast<qreal>(timeMs), static_cast<qreal>(offset(LOW, offval)));
        }

        x_axis->setMin(now.addSecs(-time_offset_sec * 1));
        x_axis->setMax(now.addSecs(0));
    }

    addDataMutex.unlock();
}

void ShowEvents::handleMarkerClicked ( void ) {
    QLegendMarker* marker = qobject_cast<QLegendMarker*> (sender());

    switch (marker->type()) {
        case QLegendMarker::LegendMarkerTypeXY:
        {
            // Toggle visibility of series
            marker->series()->setVisible(!marker->series()->isVisible());

            // Turn legend marker back to visible, since hiding series also hides the marker
            // and we don't want it to happen now.
            marker->setVisible(true);

            // Dim the marker, if series is not visible
            qreal alpha = 1.0;

            if (!marker->series()->isVisible()) {
                alpha = 0.5;
            }

            QColor color;
            QBrush brush = marker->labelBrush();
            color = brush.color();
            color.setAlphaF(alpha);
            brush.setColor(color);
            marker->setLabelBrush(brush);

            brush = marker->brush();
            color = brush.color();
            color.setAlphaF(alpha);
            brush.setColor(color);
            marker->setBrush(brush);

            QPen pen = marker->pen();
            color = pen.color();
            color.setAlphaF(alpha);
            pen.setColor(color);
            marker->setPen(pen);

            break;
        }
        default:
        {
            break;
        }
    }
}
