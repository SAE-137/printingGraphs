#include "graphfactory.h"



GraphFactory::GraphFactory(const QVector<std::shared_ptr<IGraphs>>& graphs) {
    for(auto& it: graphs) {
        m_map[it->getType()] = it;
    }
}


std::shared_ptr<IGraphs> GraphFactory::getRender(GraphType type) const {
    return m_map.value(type, nullptr);
}


QVector<std::shared_ptr<IGraphs>> GraphFactory::show() {
    return m_map.values().toVector();
}
