#include "MainWindow.h"
#include "IocContainer.h"
#include "JsonReader.h"

#include "scattergraph.h"
#include "linegraph.h"
#include "graphfactory.h"
#include"AppConfigurator.h"

#include <QApplication>

int IOCContainer::s_nextTypeId = 115094801;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<GraphType>("GraphType");

    AppConfigurator cfg;
    cfg.registerReaders<JsonReader>();
    cfg.registerCharts<ScatterGraph, LineGraph>();

    auto readerFactory = cfg.container().GetObject<ReaderFactory>();
    auto chartFactory  = cfg.container().GetObject<GraphFactory>();

    MainWindow w(chartFactory, readerFactory);
    w.show();
    return a.exec();
}
