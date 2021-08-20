#pragma once

#include<array>
#include<unordered_map>
#include<cassert>

#include "ecs/core/EnComSysEv.hpp"

class IComponenArray
{
public:
    virtual ~IComponenArray() = default;
    virtual void entityDestroyed(Entity entity) = 0;
};

template<typename Cpn>
class ComponentArray : public IComponenArray
{
public:
    ComponentArray();
    void addData(Entity entity, Cpn data);
    void removeData(Entity entity);
    Cpn& getData(Entity entity);
    void entityDestroyed(Entity entity) override;
private:
    std::size_t mRealSize;
    std::array<Cpn, MAX_ENTITIES> mComponentData;
    std::unordered_map<Entity, std::size_t> mEntityIndex;
    std::unordered_map<std::size_t, Entity> mIndexEntity;
};

template<typename Cpn>
ComponentArray<Cpn>::ComponentArray()
: mRealSize(0)
, mComponentData()
, mEntityIndex()
, mIndexEntity()
{

}

template<typename Cpn>
void ComponentArray<Cpn>::addData(Entity entity, Cpn data)
{
    assert(mEntityIndex.find(entity) == mEntityIndex.end()
           && "Same component type added more than once!");

    mComponentData[mRealSize] = data;
    mEntityIndex[entity] = mRealSize;
    mIndexEntity[mRealSize] = entity;
    mRealSize ++;
}

template<typename Cpn>
void ComponentArray<Cpn>::removeData(Entity removedEntity)
{
    assert(mEntityIndex.find(removedEntity) != mEntityIndex.end()
           && "No component to remove!");

    std::size_t removedIndex = mEntityIndex[removedEntity];
    mComponentData[removedIndex] = mComponentData[mRealSize - 1];
    mEntityIndex[mIndexEntity[mRealSize - 1]] = removedIndex;
    mIndexEntity[removedIndex] = mIndexEntity[mRealSize - 1];
    mIndexEntity.erase(mRealSize - 1);
    mEntityIndex.erase(removedEntity);
    mRealSize --;
}

template<typename Cpn>
Cpn& ComponentArray<Cpn>::getData(Entity entity)
{
    assert(mEntityIndex.find(entity) != mEntityIndex.end()
           && "No component to get!");

    return mComponentData[mEntityIndex[entity]];
}

template<typename Cpn>
void ComponentArray<Cpn>::entityDestroyed(Entity entity)
{
    if(mEntityIndex.find(entity) != mEntityIndex.end())
    {
        removeData(entity);
    }
}
