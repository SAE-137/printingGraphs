#ifndef APPSETUP_H
#define APPSETUP_H

#include "IOCContainer.h"

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

    IOCContainer& getContainer() { return container; }

private:
    IOCContainer container;
};

#endif // APPSETUP_H
