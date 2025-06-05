#ifndef READERFACTORY_H
#define READERFACTORY_H

#include"idatareader.h"
#include <memory>


class ReaderFactory {
public:
    ReaderFactory(const QVector<std::shared_ptr<IDataReader>>& readers);

    std::shared_ptr<IDataReader> getReader(const QString& ext) const;
    QStringList getExpansions() const;
private:
    QHash<QString, std::shared_ptr<IDataReader>> m_map;
    QStringList m_expansions;
};

#endif // READERFACTORY_H
