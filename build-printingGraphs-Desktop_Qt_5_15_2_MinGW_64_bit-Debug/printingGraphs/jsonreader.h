#ifndef JSONREADER_H
#define JSONREADER_H

#include"idatareader.h"
#include"datacontainer.h"

class JsonReader : public IDataReader
{
public:
     DataContainer loadFromFile(const QString &filePath) override;
     QString getName() const override;
};

#endif // JSONREADER_H
