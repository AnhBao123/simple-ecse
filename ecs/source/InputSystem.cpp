#include "ecs/system/InputSystem.hpp"
#include<iostream>
#include<cmath>

InputSystem::InputSystem()
: System()
{

}

void InputSystem::update(sf::Time dt)
{
    for(Entity entity : mEntities)
    {
        RigidBody& rigid = mECSE->getComponent<RigidBody>(entity);
        Movement& movement = mECSE->getComponent<Movement>(entity);
        Jump& jump = mECSE->getComponent<Jump>(entity);

        if(movement.direction.x == 1.0f)
            rigid.acceleration.x = movement.absAcceleration;
        else if(movement.direction.x == -1.0f)
            rigid.acceleration.x = -movement.absAcceleration;
        else
            rigid.acceleration.x = 0.0f;

        if(movement.direction.y == -1.0f && jump.currentJump < jump.maxJump)
        {
            rigid.velocity.y = -std::sqrt(2*jump.gravity*jump.jumpHeight);
            jump.currentJump ++;
        }
        movement.direction = sf::Vector2f(0.0f, 0.0f);
    }
}

void InputSystem::getNotification(const char* eventType)
{
    assert(eventType == typeid(Movement).name()
           && "Invalid event type to movement system!");

    std::vector<Movement>& movements = mEventManager->getEventData<Movement>();
    for(auto itr = movements.begin(); itr != movements.end(); ++ itr)
    {
        for(Entity entity : mEntities)
        {
            Movement& movement = mECSE->getComponent<Movement>(entity);

            movement.direction += itr->direction;
        }
    }
}

