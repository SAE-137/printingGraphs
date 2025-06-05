#ifndef GRAPHFACTORY_H
#define GRAPHFACTORY_H

#include "igraphs.h"
#include <QHash>
#include <QVector>
#include <memory>

class GraphFactory {
public:
    GraphFactory(const QVector<std::shared_ptr<IGraphs>>& graphs = {});
    std::shared_ptr<IGraphs> getGraph(GraphType type) const;
    QVector<std::shared_ptr<IGraphs>> show() const;

private:
    QHash<GraphType, std::shared_ptr<IGraphs>> m_map;
};

#endif // GRAPHFACTORY_H
