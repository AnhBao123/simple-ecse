#pragma once

#include "ecs/core/EntityManager.hpp"
#include "ecs/core/ComponentManager.hpp"
#include "ecs/core/SystemManager.hpp"
#include "ecs/core/EventManager.hpp"

class ECSE
{
public:
    ECSE();
    //Entity
    Entity createEntity();
    void destroyEntity(Entity entity);
    //Component
    template<typename Cpn>
    void registerComponent();
    template<typename Cpn>
    Cpn& getComponent(Entity entity);
    template<typename Cpn>
    void addComponent(Entity entity, Cpn component);
    template<typename Cpn>
    void removeComponent(Entity entity);
    template<typename Cpn>
    ComponentType getComponentType();
    //System
    template<typename DrSys>
    std::shared_ptr<DrSys> registerSystem();
    template<typename DrSys>
    void setSystemSignature(Signature signature);
    //Event
    template<typename EventType>
    void registerEventType();
    template<typename EventType>
    void addEventSubcriber(System* system);

    EventManager* getEventManager();
private:
    std::unique_ptr<EntityManager> mEntity;
    std::unique_ptr<ComponentManager> mComponent;
    std::unique_ptr<SystemManager> mSystem;
    std::unique_ptr<EventManager> mEvent;
};

template<typename Cpn>
void ECSE::addComponent(Entity entity, Cpn component)
{
    mComponent->addComponent<Cpn>(entity, component);
    Signature signature = mEntity->getSignature(entity);
    signature.set(mComponent->getComponentType<Cpn>(), true);
    mEntity->setSignature(entity, signature);

    mSystem->entitySignatureChanged(entity, signature);
}

template<typename Cpn>
void ECSE::removeComponent(Entity entity)
{
    mComponent->removeComponent<Cpn>(entity);
    Signature signature = mEntity->getSignature(entity);
    signature.set(mComponent->getComponentType<Cpn>(), false);
    mEntity->setSignature(entity, signature);

    mSystem->entitySignatureChanged(entity, signature);
}

template<typename Cpn>
void ECSE::registerComponent()
{
    mComponent->registerComponent<Cpn>();
}

template<typename Cpn>
Cpn& ECSE::getComponent(Entity entity)
{
    return mComponent->getComponent<Cpn>(entity);
}

template<typename Cpn>
ComponentType ECSE::getComponentType()
{
    return mComponent->getComponentType<Cpn>();
}

template<typename DrSys>
std::shared_ptr<DrSys> ECSE::registerSystem()
{
    return mSystem->registerSystem<DrSys>();
}

template<typename DrSys>
void ECSE::setSystemSignature(Signature signature)
{
    mSystem->setSignature<DrSys>(signature);
}

template<typename EventType>
void ECSE::registerEventType()
{
    mEvent->registerEvent<EventType>();
}

template<typename EventType>
void ECSE::addEventSubcriber(System* system)
{
    mEvent->addSubcriber<EventType>(system);
}
