#ifndef DATACONTAINER_H
#define DATACONTAINER_H

#include <QVector>
#include <QPair>
#include <QDateTime>

class DataContainer
{
public:
    DataContainer() = default;
    bool isEmpty() const { return DataPoints.isEmpty(); }
    void clear() { DataPoints.clear(); }
    const QVector<QPair<QDateTime, double>>& points() const { return DataPoints; }

    QVector<QPair<QDateTime, double>> DataPoints;
};

#endif // DATACONTAINER_H
