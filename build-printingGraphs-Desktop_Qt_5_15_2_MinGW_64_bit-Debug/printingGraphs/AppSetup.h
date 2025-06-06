#ifndef APPSETUP_H
#define APPSETUP_H

#include "dependencycontainer.h"

class AppSetup {
public:
    AppSetup() = default;

    template<typename... ReaderTs>
    void configureReaders() {
        container.SetupReaders<ReaderTs...>();
    }

    template<typename... GraphTs>
    void configureGraphs() {
        container.SetupGraphs<GraphTs...>();
    }

    IocContainer& getContainer() { return container; }

private:
    IocContainer container;
};

#endif // APPSETUP_H
