#include "ecs/core/ECSE.hpp"

ECSE::ECSE()
: mEntity(new EntityManager)
, mComponent(new ComponentManager)
, mSystem(new SystemManager)
, mEvent(new EventManager)
{
}

Entity ECSE::createEntity()
{
    return mEntity->createEntity();
}

void ECSE::destroyEntity(Entity entity)
{
    mEntity->destroyEntity(entity);
    mComponent->entityDestroyed(entity);
    mSystem->entityDestroyed(entity);
}

EventManager* ECSE::getEventManager()
{
    return mEvent.get();
}
