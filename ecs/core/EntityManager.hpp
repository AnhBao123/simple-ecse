#pragma once

#include<queue>
#include<cassert>

#include "ecs/core/EnComSysEv.hpp"

class EntityManager
{
public:
    EntityManager();
    Entity createEntity();
    void destroyEntity(Entity entity);
    void setSignature(Entity entity, Signature signature);
    Signature getSignature(Entity entity);
private:
    std::queue<Entity> mAvailableIDs;
    std::array<Signature, MAX_ENTITIES> mSignatures;
    std::size_t mLivingEntities;
};
