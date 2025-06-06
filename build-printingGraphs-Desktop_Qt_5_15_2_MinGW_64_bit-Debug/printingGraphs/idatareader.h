#ifndef IDATAREADER_H
#define IDATAREADER_H

#include"datacontainer.h"

class IDataReader
{
public:
    virtual ~IDataReader() = default;

    virtual DataContainer loadFromFile(const QString &filePath) const = 0;
    virtual QDateTime interpretDate(const QString &input) const;
    virtual QString getName() const = 0;
private:
    static const QVector<QString> DATE_PATTERNS;
};

#endif // IDATAREADER_H
