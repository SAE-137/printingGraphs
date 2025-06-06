#ifndef DEPENDENCYCONTAINER_H
#define DEPENDENCYCONTAINER_H

#include "graphfactory.h"
#include "readerfactory.h"
#include <functional>
#include <memory>
#include <unordered_map>

namespace {
class FactoryBase {
public:
    virtual ~FactoryBase() = default;
};

template<typename T>
class ObjectFactory : public FactoryBase {
    std::function<std::shared_ptr<T>()> creator;
public:
    ObjectFactory(std::function<std::shared_ptr<T>()> fn) : creator(fn) {}
    std::shared_ptr<T> GetObject() { return creator(); }
};
}

class IocContainer {
    static int typeCounter;
    template<typename T>
    static int GetTypeId() {
        static int typeId = typeCounter++;
        return typeId;
    }

    std::unordered_map<int, std::shared_ptr<FactoryBase>> factories;

public:
    template<typename T>
    std::shared_ptr<T> Resolve() {
        auto typeId = GetTypeId<T>();
        auto factoryBase = factories.at(typeId);
        auto factory = std::static_pointer_cast<ObjectFactory<T>>(factoryBase);
        return factory->GetObject();
    }

    template<typename TInterface, typename... TS>
    void BindFunctor(std::function<std::shared_ptr<TInterface>(std::shared_ptr<TS>...)> fn) {
        factories[GetTypeId<TInterface>()] = std::make_shared<ObjectFactory<TInterface>>(
            [=] { return fn(Resolve<TS>()...); });
    }

    template<typename TInterface>
    void BindInstance(std::shared_ptr<TInterface> instance) {
        factories[GetTypeId<TInterface>()] = std::make_shared<ObjectFactory<TInterface>>(
            [=] { return instance; });
    }

    template<typename TInterface, typename... TS>
    void BindFunctor(std::shared_ptr<TInterface> (*fn)(std::shared_ptr<TS>...)) {
        BindFunctor(std::function<std::shared_ptr<TInterface>(std::shared_ptr<TS>... ts)>(fn));
    }

    template<typename TInterface, typename TConcrete, typename... TArgs>
    void BindFactory() {
        BindFunctor(std::function<std::shared_ptr<TInterface>(std::shared_ptr<TArgs>...)>(
            [](std::shared_ptr<TArgs>... args) -> std::shared_ptr<TInterface> {
                return std::make_shared<TConcrete>(std::forward<std::shared_ptr<TArgs>>(args)...);
            }));
    }

    template<typename TInterface, typename TConcrete, typename... TArgs>
    void BindInstance() {
        BindInstance<TInterface>(std::make_shared<TConcrete>(Resolve<TArgs>()...));
    }

    template<typename... ReaderTs>
    void SetupReaders() {
        (BindFactory<ReaderTs, ReaderTs>(), ...);
        BindFunctor<ReaderFactory, ReaderTs...>(&IocContainer::MakeReaderFactory<ReaderTs...>);
    }

    template<typename... GraphTs>
    void SetupGraphs() {
        (BindFactory<GraphTs, GraphTs>(), ...);
        BindFunctor<GraphFactory, GraphTs...>(&IocContainer::MakeGraphFactory<GraphTs...>);
    }

private:
    template<typename... ReaderTs>
    static std::shared_ptr<ReaderFactory> MakeReaderFactory(std::shared_ptr<ReaderTs>... readers) {
        return std::make_shared<ReaderFactory>(QVector<std::shared_ptr<IDataReader>>{readers...});
    }

    template<typename... GraphTs>
    static std::shared_ptr<GraphFactory> MakeGraphFactory(std::shared_ptr<GraphTs>... graphs) {
        return std::make_shared<GraphFactory>(QVector<std::shared_ptr<IGraphs>>{graphs...});
    }
};

#endif // DEPENDENCYCONTAINER_H
