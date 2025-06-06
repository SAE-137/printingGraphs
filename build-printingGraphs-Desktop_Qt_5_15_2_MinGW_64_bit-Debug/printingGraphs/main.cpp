#include <QApplication>
#include "mainwindow.h"
#include "appsetup.h"
#include "jsonreader.h"
#include "sqlreader.h"
#include "linegraph.h"
#include "scattergraph.h"
#include "graphfactory.h"
#include "readerfactory.h"
#include "dependencycontainer.h"

int DependencyContainer::typeCounter = 200;

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    qRegisterMetaType<GraphType>("GraphType");

    AppSetup setup;
    setup.configureReaders<JsonReader, SqlReader>();
    setup.configureGraphs<LineGraph, ScatterGraph>();

    auto readerFactory = setup.getContainer().Resolve<ReaderFactory>();
    auto graphFactory = setup.getContainer().Resolve<GraphFactory>();

    MainWindow window(graphFactory, readerFactory);
    window.show();
    return app.exec();
}
