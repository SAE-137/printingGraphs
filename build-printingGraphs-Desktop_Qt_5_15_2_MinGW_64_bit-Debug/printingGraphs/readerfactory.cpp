#include "readerfactory.h"
#include "jsonreader.h"
#include "sqlreader.h"

ReaderFactory::ReaderFactory(const QVector<std::shared_ptr<IDataReader>>& readers) {

    for (const auto& it : readers) {
        if (it && !m_map.contains(it->getName())) {
            m_map[it->getName()] = it;
            m_expansions.append(it->getName());
        }
    }

    if (!m_map.contains("json")) {
        m_map["json"] = std::make_shared<JsonReader>();
        m_expansions.append("json");
    }
    if (!m_map.contains("sqlite")) {
        m_map["sqlite"] = std::make_shared<SqlReader>();
        m_expansions.append("sqlite");
    }
    m_expansions.removeDuplicates();
}

std::shared_ptr<IDataReader> ReaderFactory::getReader(const QString& ext) const {
    return m_map.value(ext.toLower(), nullptr);
}

QStringList ReaderFactory::getExpansions() const {
    return m_expansions;
}
