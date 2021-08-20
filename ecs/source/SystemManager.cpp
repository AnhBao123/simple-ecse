#include "ecs/core/SystemManager.hpp"

SystemManager::SystemManager()
: mSignatures()
, mSystems()
{

}

void SystemManager::entityDestroyed(Entity entity)
{
    for(auto itr = mSystems.begin(); itr != mSystems.end(); ++ itr)
    {
        itr->second->mEntities.erase(entity);
    }
}

void SystemManager::entitySignatureChanged(Entity entity, Signature signature)
{
    for(auto itr = mSystems.begin(); itr != mSystems.end(); ++ itr)
    {
        const char* typeName = itr->first;
        std::shared_ptr<System> system = itr->second;
        if((mSignatures[typeName] & signature) == mSignatures[typeName])
        {
            system->mEntities.insert(entity);
        }
        else
        {
            system->mEntities.erase(entity);
        }
    }
}

