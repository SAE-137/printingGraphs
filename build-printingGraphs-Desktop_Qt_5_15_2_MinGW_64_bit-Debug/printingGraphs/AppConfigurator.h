#ifndef APPCONFIGURARTOR_H
#define APPCONFIGURARTOR_H

#include "IocContainer.h"

class AppConfigurator {
public:
    AppConfigurator() = default;

    template<typename... ReaderTs>
    void registerReaders() {
        injector.RegisterReaders<ReaderTs...>();
    }

    template<typename... ChartTs>
    void registerCharts() {
        injector.RegisterCharts<ChartTs...>();
    }

    IOCContainer& container() { return injector; }

private:
    IOCContainer injector;
};

#endif // APPCONFIGURARTOR_H
