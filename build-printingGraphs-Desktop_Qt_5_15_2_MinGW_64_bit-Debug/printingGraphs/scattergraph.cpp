#include "scattergraph.h"
#include <QtCharts/QChart>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>

void ScatterGraph::show(const DataContainer& data, QtCharts::QChartView* view) {
    QtCharts::QScatterSeries* series = new QtCharts::QScatterSeries();

    int total = data.DataPoints.size();
    int maxPixels = view->width() > 0 ? view->width() : 1000;
    int step = qMax(1, total/maxPixels);

    double minY = data.DataPoints.first().second;
    double maxY = minY;

    if(!data.isEmpty()) {
        for(int i = 0; i < total; i += step) {
            minY = qMin(minY, data.DataPoints[i].second);
            maxY = qMax(maxY, data.DataPoints[i].second);
            series->append(data.DataPoints[i].first.toMSecsSinceEpoch(), data.DataPoints[i].second);
        }
    }

    series->setMarkerSize(5.0);
    series->setPen(QPen(Qt::green, 1));
    auto *chart = new QtCharts::QChart();
    chart->addSeries(series);
    chart->legend()->hide();


    auto *axisX = new QtCharts::QDateTimeAxis();
    axisX->setFormat("dd.MM.yyyy HH:mm");

    int pixelsPerTick = 100;
    int w = view->size().width();
    int count = qMax(2, w / pixelsPerTick);
    axisX->setTickCount(count);
    axisX->setLabelsAngle(-45);
    axisX->setRange(data.DataPoints.first().first,
                    data.DataPoints.last().first);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    auto *axisY = new QtCharts::QValueAxis();
    axisY->setLabelFormat("%.2f");
    axisY->setRange(minY, maxY);
    axisY->setGridLineVisible(true);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    view->setRenderHint(QPainter::Antialiasing);
    view->setChart(chart);
}

GraphType ScatterGraph::getType() const {
    return GraphType::Scatter;
}

QString ScatterGraph::getName() const {
    return "Scatter";
}
