#ifndef SCATTERGRAPH_H
#define SCATTERGRAPH_H

#include "igraphs.h"

class ScatterGraph : public IGraphs {
public:
    void show(const DataContainer& data, QtCharts::QChartView* view) override;
    GraphType getType() const override;
    QString getName() const override;
};

#endif // SCATTERGRAPH_H
