#include "ecs/system/RigidBodySystem.hpp"
#include<cmath>
#include<algorithm>

void RigidBodySystem::update(sf::Time dt)
{
    for(Entity entity : mEntities)
    {
        RigidBody& rigid = mECSE->getComponent<RigidBody>(entity);

        rigid.velocity += rigid.acceleration*dt.asSeconds();
        rigid.velocity.x *= std::pow(rigid.slowFriction, dt.asSeconds());
    }
}

void RigidBodySystem::getNotification(const char* eventType)
{
    assert(eventType == typeid(CollisionInfo).name()
           && "Invalid event type notification!");

    std::vector<CollisionInfo>& infos = mEventManager->getEventData<CollisionInfo>();
    std::sort(infos.begin(), infos.end(),
     [](CollisionInfo& info1, CollisionInfo& info2)
     {
            if(info1.entity < info2.entity) return true;
            if(info1.time < info2.time) return true;
            else return false;
     });
    for(auto itr = infos.begin(); itr != infos.end(); ++ itr)
    {
        RigidBody& rigidBody = mECSE->getComponent<RigidBody>(itr->entity);
        resolveAABB(rigidBody, itr->normal, itr->time);
    }
}
