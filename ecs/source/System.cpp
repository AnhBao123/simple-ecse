#include "ecs/core/System.hpp"
#include "ecs/core/EventManager.hpp"

System::System()
: mEntities()
, mEventManager(nullptr)
, mECSE(nullptr)
{
}

void System::getNotification(const char* eventType)
{
    //do sth
    std::cout << "Base system handling...........\n";
}

void System::setEventManager(EventManager* manager)
{
    mEventManager = manager;
}

void System::setECSE(ECSE* ecse)
{
    mECSE = ecse;
}
