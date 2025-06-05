#ifndef SQLREADER_H
#define SQLREADER_H

#include "idatareader.h"

class SqlReader : public IDataReader {
public:
    DataContainer loadFromFile(const QString& filePath) const override;
    QString getName() const override;
};

#endif // SQLREADER_H
