#ifndef SCATTERGRAPH_H
#define SCATTERGRAPH_H




#include <QtCharts/QScatterSeries>
#include <QtCharts/QChart>
#include <QValueAxis>
#include"datacontainer.h"
#include"igraphs.h"


class ScatterGraph : public IGraphs
{
public:
    ScatterGraph();
    ~ScatterGraph();
    void show(const DataContainer& data, QtCharts::QChartView* view) override;
    GraphType getType() const override;

private:

};

#endif // SCATTERGRAPH_H
