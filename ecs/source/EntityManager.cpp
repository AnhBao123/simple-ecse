#include "ecs/core/EntityManager.hpp"

EntityManager::EntityManager()
: mAvailableIDs()
, mSignatures()
, mLivingEntities(0)
{
    for(Entity en = 0; en < MAX_ENTITIES; ++ en)
        mAvailableIDs.push(en);
}

Entity EntityManager::createEntity()
{
    assert(mLivingEntities < MAX_ENTITIES
           && "Too many entities!");

    Entity entity = mAvailableIDs.front();
    mAvailableIDs.pop();
    mLivingEntities ++;
    return entity;
}

void EntityManager::destroyEntity(Entity entity)
{
    assert(entity < MAX_ENTITIES
           && "Entity out of range!");

    mAvailableIDs.push(entity);
    mLivingEntities --;
}

void EntityManager::setSignature(Entity entity, Signature signature)
{
    assert(entity < MAX_ENTITIES
           && "Entity out of range!");

    mSignatures[entity] = signature;
}

Signature EntityManager::getSignature(Entity entity)
{
    assert(entity < MAX_ENTITIES
           && "Entity out of range!");

    return mSignatures[entity];
}

