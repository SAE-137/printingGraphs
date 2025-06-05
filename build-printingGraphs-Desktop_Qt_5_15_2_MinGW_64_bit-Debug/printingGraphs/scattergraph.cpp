#include "scattergraph.h"
#include <QtCharts/QChart>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>

void ScatterGraph::show(const DataContainer& data, QtCharts::QChartView* view) {
    if (data.isEmpty() || !view) {
        return;
    }

    // Создаём серию для точечного графика
    QtCharts::QScatterSeries* series = new QtCharts::QScatterSeries();
    series->setName("Data Points");
    series->setMarkerSize(8.0);

    // Заполняем серию данными
    for (const auto& point : data.points()) {
        series->append(point.first.toMSecsSinceEpoch(), point.second);
    }

    // Создаём график
    QtCharts::QChart* chart = new QtCharts::QChart();
    chart->addSeries(series);
    chart->setTitle("Scatter Graph");
    chart->legend()->setVisible(true);

    // Ось X (время)
    QtCharts::QDateTimeAxis* axisX = new QtCharts::QDateTimeAxis();
    axisX->setFormat("dd.MM.yyyy HH:mm");
    axisX->setTitleText("Time");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Ось Y (значения)
    QtCharts::QValueAxis* axisY = new QtCharts::QValueAxis();
    axisY->setTitleText("Value");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Устанавливаем график в представление
    view->setChart(chart);
}

GraphType ScatterGraph::getType() const {
    return GraphType::Scatter;
}

QString ScatterGraph::getName() const {
    return "Scatter";
}
