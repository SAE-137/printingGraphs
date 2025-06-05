#include <QDateTimeAxis>

#include "linegraph.h"

LineGraph::LineGraph()
{

}

void LineGraph::show(const DataContainer &dataset, QtCharts::QChartView* chartWidget)
{

    double yMin = std::numeric_limits<double>::max();
    double yMax = std::numeric_limits<double>::min();


    QtCharts::QLineSeries* dataSeries = new QtCharts::QLineSeries();
    QPen graphPen(QColor(0, 100, 0));
    graphPen.setWidth(1);
    dataSeries->setPen(graphPen);


    for (const auto& point : dataset.DataPoints) {
        yMin = std::min(yMin, point.second);
        yMax = std::max(yMax, point.second);
        dataSeries->append(point.first.toMSecsSinceEpoch(), point.second);
    }


    QtCharts::QChart* graph = new QtCharts::QChart();
    graph->addSeries(dataSeries);
    graph->legend()->setVisible(false);
    graph->setAnimationOptions(QtCharts::QChart::NoAnimation);

    // Ось времени
    QtCharts::QDateTimeAxis* timeAxis = new QtCharts::QDateTimeAxis();
    timeAxis->setFormat("dd.MM.yyyy HH:mm");
    timeAxis->setLabelsAngle(315);
    const int tickSpacing = 120;
    int tickCount = qBound(2, chartWidget->width() / tickSpacing, 10);
    timeAxis->setTickCount(tickCount);
    graph->addAxis(timeAxis, Qt::AlignmentFlag::AlignBottom);
    dataSeries->attachAxis(timeAxis);


    if (!dataset.DataPoints.isEmpty()) {
        timeAxis->setRange(
            dataset.DataPoints.front().first,
            dataset.DataPoints.back().first
            );
    }


    QtCharts::QValueAxis* valueAxis = new QtCharts::QValueAxis();
    graph->addAxis(valueAxis, Qt::AlignmentFlag::AlignLeft);
    dataSeries->attachAxis(valueAxis);


    const double padding = (yMax - yMin) * 0.05;
    valueAxis->setRange(yMin - padding, yMax + padding);


    chartWidget->setChart(graph);
}
