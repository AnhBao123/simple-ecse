#pragma once

#include<unordered_map>
#include<map>
#include<vector>
#include<cassert>
#include<memory>
#include<iostream>

#include "ecs/core/EnComSysEv.hpp"
#include "ecs/core/System.hpp"

struct IEventArray
{
    virtual ~IEventArray() = default;
};

template<typename EventType>
struct EventArray : public IEventArray
{
    std::vector<EventType> mArray;
};

class EventManager
{
public:
    template<typename EventType>
    void registerEvent();
    template<typename EventType>
    void notify();
    template<typename EventType>
    void addSubcriber(System* system);
    template<typename EventType>
    std::vector<EventType>& getEventData();
private:
    template<typename EventType>
    std::shared_ptr<EventArray<EventType>> getEventArray();
private:
    std::unordered_map<const char*,
        std::shared_ptr<IEventArray>>   mEventArrays{};

    std::unordered_map<const char*,
        std::vector<System*>>           mSubcribers{};
};

template<typename EventType>
void EventManager::registerEvent()
{
    const char* typeName = typeid(EventType).name();
    assert(mEventArrays.find(typeName) == mEventArrays.end()
           && "Event type registered twice!");

    mEventArrays.emplace(typeName, std::make_shared<EventArray<EventType>>());
    mSubcribers.emplace(typeName, std::vector<System*>());
}

template<typename EventType>
void EventManager::notify()
{
    const char* typeName = typeid(EventType).name();
    assert(mEventArrays.find(typeName) != mEventArrays.end()
           && "Unregistered event type!");

    for(auto itr = mSubcribers[typeName].begin(); itr != mSubcribers[typeName].end(); ++ itr)
    {
        (*itr)->getNotification(typeName);
    }
}

template<typename EventType>
void EventManager::addSubcriber(System* system)
{
    const char* typeName = typeid(EventType).name();
    assert(mEventArrays.find(typeName) != mEventArrays.end()
           && "Unregistered event type!");

    mSubcribers[typeName].push_back(system);
}

template<typename EventType>
std::vector<EventType>& EventManager::getEventData()
{
    return getEventArray<EventType>()->mArray;
}

template<typename EventType>
std::shared_ptr<EventArray<EventType>> EventManager::getEventArray()
{
    const char* typeName = typeid(EventType).name();
    assert(mEventArrays.find(typeName) != mEventArrays.end()
           && "Unregistered use!");

    return std::static_pointer_cast<EventArray<EventType>>(mEventArrays[typeName]);
}

