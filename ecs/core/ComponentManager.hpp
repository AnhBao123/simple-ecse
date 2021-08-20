#pragma once

#include<memory>
#include<typeinfo>

#include "ecs/core/ComponentArray.hpp"

class ComponentManager
{
public:
    ComponentManager();
    template<typename Cpn>
        void registerComponent();
    template<typename Cpn>
        void addComponent(Entity entity, Cpn component);
    template<typename Cpn>
        void removeComponent(Entity entity);
    template<typename Cpn>
        Cpn& getComponent(Entity entity);
    template<typename Cpn>
        ComponentType getComponentType();

    void entityDestroyed(Entity entity);
private:
    template<typename Cpn>
    std::shared_ptr<ComponentArray<Cpn>> getComponentArray();
private:
    std::unordered_map<const char*,
                       ComponentType> mComponentTypes;
    std::unordered_map<const char*,
                       std::shared_ptr<IComponenArray>> mComponentArrays;
    ComponentType mTypeCount;
};

template<typename Cpn>
void ComponentManager::registerComponent()
{
    assert(mTypeCount < MAX_COMPONENTS
           && "Too many components!");

    const char* typeName = typeid(Cpn).name();
    assert(mComponentTypes.find(typeName) == mComponentTypes.end()
           && "Same component type registered twice!");

    mComponentTypes.insert({typeName, mTypeCount});
    mComponentArrays.insert({typeName, std::make_shared<ComponentArray<Cpn>>()});
    mTypeCount ++;
}

template<typename Cpn>
void ComponentManager::addComponent(Entity entity, Cpn component)
{
    getComponentArray<Cpn>()->addData(entity, component);
}

template<typename Cpn>
void ComponentManager::removeComponent(Entity entity)
{
    getComponentArray<Cpn>()->removeData(entity);
}

template<typename Cpn>
Cpn& ComponentManager::getComponent(Entity entity)
{
    return getComponentArray<Cpn>()->getData(entity);
}

template<typename Cpn>
std::shared_ptr<ComponentArray<Cpn>> ComponentManager::getComponentArray()
{
    const char* typeName = typeid(Cpn).name();
    assert(mComponentTypes.find(typeName) != mComponentTypes.end()
           && "No existing component type!");

    return std::static_pointer_cast<ComponentArray<Cpn>>(mComponentArrays[typeName]);
}

template<typename Cpn>
ComponentType ComponentManager::getComponentType()
{
    const char* typeName = typeid(Cpn).name();
    assert(mComponentTypes.find(typeName) != mComponentTypes.end()
           && "No existing component type!");

    return mComponentTypes[typeName];
}
