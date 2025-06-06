#ifndef LINEGRAPH_H
#define LINEGRAPH_H

#include <QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include "igraphs.h"

class LineGraph : public IGraphs {
public:
    GraphType getType() const override;
    void show(const DataContainer& data, QtCharts::QChartView* view) override;
    QString getName() const override;

private:
    QString m_name = "Line Graph";
};

#endif // LINEGRAPH_H
