#include "ecs/core/ComponentManager.hpp"

ComponentManager::ComponentManager()
: mComponentTypes()
, mComponentArrays()
, mTypeCount(0)
{
}

void ComponentManager::entityDestroyed(Entity entity)
{
    for(auto itr = mComponentArrays.begin(); itr != mComponentArrays.end(); ++ itr)
    {
        itr->second->entityDestroyed(entity);
    }
}
