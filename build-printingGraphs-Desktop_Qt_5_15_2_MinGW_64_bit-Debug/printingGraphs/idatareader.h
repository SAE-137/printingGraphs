#ifndef IDATAREADER_H
#define IDATAREADER_H

#include"datacontainer.h""

class IDataReader
{
public:
    virtual ~IDataReader() = default;

    virtual DataContainer loadFromFile(const QString &filePath) = 0;
    virtual QDateTime interpretDate(const QString &input) const;
private:
    static const QVector<QString> DATE_PATTERNS;
};

#endif // IDATAREADER_H
