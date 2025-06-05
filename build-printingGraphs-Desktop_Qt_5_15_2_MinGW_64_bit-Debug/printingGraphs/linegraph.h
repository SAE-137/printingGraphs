#ifndef LINEGRAPH_H
#define LINEGRAPH_H

#include <QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>

#include"igraphs.h"

class LineGraph : public IGraphs
{
public:
    LineGraph();
    ~LineGraph() = default;
    void show(const DataContainer &data, QtCharts::QChartView* view) override;
};

#endif // LINEGRAPH_H
