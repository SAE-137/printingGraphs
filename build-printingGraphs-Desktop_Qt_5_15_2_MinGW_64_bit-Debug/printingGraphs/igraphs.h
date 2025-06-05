#ifndef IGRAPHS_H
#define IGRAPHS_H


#include <QtCharts/QChartView>
#include"datacontainer.h"

class IGraphs
{
public:
    IGraphs();
    virtual ~IGraphs() = default;
    virtual void show(const DataContainer &, QtCharts::QChartView*) = 0;
    virtual void setData() = 0;
};

#endif // IGRAPHS_H
