#pragma once

#include<set>

#include "ecs/core/EnComSysEv.hpp"

class EventManager;
class ECSE;
class System
{
public:
    System();
    virtual ~System() = default;
    virtual void update(sf::Time dt) = 0;
    virtual void getNotification(const char* eventType);

    void setEventManager(EventManager* manager);
    void setECSE(ECSE* ecse);
public:
    std::set<Entity> mEntities;
    EventManager* mEventManager;
    ECSE* mECSE;
};
