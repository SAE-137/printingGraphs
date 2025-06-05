#include "graphfactory.h"
#include "linegraph.h"
#include "scattergraph.h"

GraphFactory::GraphFactory(const QVector<std::shared_ptr<IGraphs>>& graphs) {
    // Добавляем переданные графики
    for (const auto& it : graphs) {
        if (it && !m_map.contains(it->getType())) {
            m_map[it->getType()] = it;
        }
    }
    // Добавляем встроенные графики, если их нет
    if (!m_map.contains(GraphType::Line)) {
        m_map[GraphType::Line] = std::make_shared<LineGraph>();
    }
    if (!m_map.contains(GraphType::Scatter)) {
        m_map[GraphType::Scatter] = std::make_shared<ScatterGraph>();
    }
}

std::shared_ptr<IGraphs> GraphFactory::getGraph(GraphType type) const {
    return m_map.value(type, nullptr);
}

QVector<std::shared_ptr<IGraphs>> GraphFactory::show() const {
    return m_map.values().toVector();
}
