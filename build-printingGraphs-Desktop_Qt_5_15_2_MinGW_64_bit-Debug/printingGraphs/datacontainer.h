#ifndef DATACONTAINER_H
#define DATACONTAINER_H

#include <QVector>
#include <QPair>
#include <QDateTime>



class DataContainer
{
public:
    DataContainer();
    QVector<QPair<QDateTime, double>> DataPoints;
};

#endif // DATACONTAINER_H
