#include "linegraph.h"
#include <QDateTimeAxis>

GraphType LineGraph::getType() const {
    return GraphType::Line;
}

void LineGraph::show(const DataContainer& dataset, QtCharts::QChartView* chartWidget) {
    if (!chartWidget || dataset.isEmpty()) return;

    double yMin = std::numeric_limits<double>::max();
    double yMax = std::numeric_limits<double>::min();

    QtCharts::QLineSeries* dataSeries = new QtCharts::QLineSeries();
    QPen graphPen(QColor(Qt::darkGreen), 1);
    dataSeries->setPen(graphPen);

    for (const auto& point : dataset.points()) {
        yMin = std::min(yMin, point.second);
        yMax = std::max(yMax, point.second);
        dataSeries->append(point.first.toMSecsSinceEpoch(), point.second);
    }

    QtCharts::QChart* graph = new QtCharts::QChart();
    graph->addSeries(dataSeries);
    graph->legend()->setVisible(false);
    graph->setAnimationOptions(QtCharts::QChart::NoAnimation);

    QtCharts::QDateTimeAxis* timeAxis = new QtCharts::QDateTimeAxis();
    timeAxis->setFormat("dd.MM.yyyy HH:mm");
    timeAxis->setLabelsAngle(-45);
    int pixelsPerTick = 100;
    int tickCount = qMax(2, chartWidget->size().width() / pixelsPerTick);
    timeAxis->setTickCount(tickCount);
    graph->addAxis(timeAxis, Qt::AlignBottom);
    dataSeries->attachAxis(timeAxis);

    if (!dataset.isEmpty()) {
        timeAxis->setRange(dataset.points().first().first, dataset.points().last().first);
    }

    QtCharts::QValueAxis* valueAxis = new QtCharts::QValueAxis();
    graph->addAxis(valueAxis, Qt::AlignLeft);
    dataSeries->attachAxis(valueAxis);
    valueAxis->setRange(yMin, yMax);

    chartWidget->setChart(graph);
}

QString LineGraph::getName() const {
    return m_name;
}
