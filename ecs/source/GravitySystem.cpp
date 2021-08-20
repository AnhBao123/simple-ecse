#include "ecs/system/GravitySystem.hpp"

void GravitySystem::update(sf::Time dt)
{
    for(Entity entity : mEntities)
    {
        RigidBody& rigid = mECSE->getComponent<RigidBody>(entity);
        const Jump& jump = mECSE->getComponent<Jump>(entity);

        rigid.acceleration.y = jump.gravity;
    }
}

void GravitySystem::getNotification(const char* eventName)
{
    assert(eventName == typeid(TouchGround).name()
           && "Invalid event type sent to Gravity system!");

    std::vector<TouchGround>& infos = mEventManager->getEventData<TouchGround>();
    for(TouchGround& info : infos)
    {
        Jump& jump = mECSE->getComponent<Jump>(info.entity);

        jump.currentJump = 0.0f;
    }
}
