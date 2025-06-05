#ifndef IGRAPHS_H
#define IGRAPHS_H

#include <QtCharts/QChartView>
#include "datacontainer.h"

enum class GraphType {
    Scatter,
    Line  // Изменено на Line вместо Liner для совместимости с QComboBox
};

Q_DECLARE_METATYPE(GraphType)

class IGraphs {
public:
    virtual ~IGraphs() = default;
    virtual void show(const DataContainer& data, QtCharts::QChartView* view) = 0;
    virtual GraphType getType() const = 0;
    virtual QString getName() const = 0;  // Аналог getNameChart из чужого кода
};

#endif // IGRAPHS_H
