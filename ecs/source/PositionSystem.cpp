#include "ecs/system/PositionSystem.hpp"

void PositionSystem::update(sf::Time dt)
{
    for(Entity entity : mEntities)
    {
        const RigidBody& rigid = mECSE->getComponent<RigidBody>(entity);
        Transform& transform = mECSE->getComponent<Transform>(entity);

        transform.rect.left += rigid.velocity.x*dt.asSeconds();
        transform.rect.top += rigid.velocity.y*dt.asSeconds();
    }
}
