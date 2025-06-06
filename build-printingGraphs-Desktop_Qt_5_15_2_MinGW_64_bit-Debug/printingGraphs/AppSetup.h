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

    DependencyContainer& getContainer() { return container; }

private:
    DependencyContainer container;
};

#endif // APPSETUP_H
