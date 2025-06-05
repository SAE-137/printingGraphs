

#include <QtCharts/QChartView>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QScatterSeries>
#include "scattergraph.h"
#include <limits>

void ScatterGraph::show(const DataContainer& data, QtCharts::QChartView* view)
{

    auto* series = new QtCharts::QScatterSeries(view);
    series->setMarkerSize(5.0);
    series->setPen(QPen(Qt::darkMagenta, 1));

    int total = data.DataPoints.size();
    int maxPixels = view->width() > 0 ? view->width() : 1000;
    int step = qMax(1, total / maxPixels);

    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::lowest();

    for (int i = 0; i < total; i += step) {
        double y = data.DataPoints[i].second;
        if (y < minY) minY = y;
        if (y > maxY) maxY = y;
        series->append(data.DataPoints[i].first.toMSecsSinceEpoch(), y);
    }


    auto* chart = new QtCharts::QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->setAnimationOptions(QtCharts::QChart::NoAnimation);

    auto* axisX = new QtCharts::QDateTimeAxis();
    axisX->setFormat("dd.MM.yyyy HH:mm");
    axisX->setLabelsAngle(-45);

    int pixelsPerTick = 100;
    int w = view->size().width();
    int count = qMax(2, w / pixelsPerTick);
    axisX->setTickCount(count);

    if (!data.DataPoints.isEmpty()) {
        axisX->setRange(data.DataPoints.first().first, data.DataPoints.last().first);
    }

    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    auto* axisY = new QtCharts::QValueAxis();
    axisY->setLabelFormat("%.2f");
    axisY->setRange(minY, maxY);
    axisY->setGridLineVisible(true);

    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    view->setRenderHint(QPainter::Antialiasing);
    view->setChart(chart);
}

GraphType ScatterGraph::getType() const
{
    return GraphType::Scatter;
}
