#ifndef GRAPHFACTORY_H
#define GRAPHFACTORY_H

#include"igraphs.h"


class GraphFactory
{
    GraphFactory(const QVector<std::shared_ptr<IGraphs>>& graphs);
    std::shared_ptr<IGraphs> getRender(GraphType type) const;
    QVector<std::shared_ptr<IGraphs>> show();
private:
    QHash<GraphType, std::shared_ptr<IGraphs>> m_map;
};

#endif // GRAPHFACTORY_H
