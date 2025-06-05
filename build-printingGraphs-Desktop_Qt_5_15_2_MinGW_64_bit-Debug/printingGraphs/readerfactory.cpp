#include "readerfactory.h"


ReaderFactory::ReaderFactory(const QVector<std::shared_ptr<IDataReader>>& readers) {

}

std::shared_ptr<IDataReader> ReaderFactory::getReader(const QString& ext) const {
    return m_map.value(ext.toLower(), nullptr);
}
QStringList ReaderFactory::getExpansions() const {
    return m_expansions;
}
