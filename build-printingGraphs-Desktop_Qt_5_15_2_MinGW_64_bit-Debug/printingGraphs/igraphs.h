#ifndef IGRAPHS_H
#define IGRAPHS_H


class IGraphs
{
public:
    IGraphs();
    virtual ~IGraphs() = default;
    virtual void show() = 0;
    virtual void setData() = 0;
};

#endif // IGRAPHS_H
