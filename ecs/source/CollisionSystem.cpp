#include "ecs/system/CollisionSystem.hpp"

void CollisionSystem::update(sf::Time dt)
{
    bool willNotifyCollisions = false;
    bool willNotifyGround = false;
    std::vector<CollisionInfo>& infos = mEventManager->getEventData<CollisionInfo>();
    std::vector<TouchGround>& touchGround = mEventManager->getEventData<TouchGround>();

    for(Entity mover : mEntities)
    for(Entity target : mEntities)
    if(mover != target)
    {
        const Transform& transFirst = mECSE->getComponent<Transform>(mover);
        const RigidBody& rigidFirst = mECSE->getComponent<RigidBody>(mover);

        const Transform& transSecond = mECSE->getComponent<Transform>(target);
        const RigidBody& rigidSecond = mECSE->getComponent<RigidBody>(target);
        if(rigidSecond.velocity == sf::Vector2f() && rigidFirst.velocity != sf::Vector2f())
        {
            float time = 1.0f;
            sf::Vector2f normal = sf::Vector2f();
            if(collideEntity_entity(transFirst.rect, rigidFirst, transSecond.rect, dt, normal, time))
            {
                infos.push_back(
                    CollisionInfo
                    {
                        .entity = mover,
                        .time = time,
                        .normal = normal,
                    });
                if(normal.y == -1.0f)
                {
                    touchGround.push_back(
                        TouchGround
                        {
                            .entity = mover,
                        });
                    willNotifyGround = true;
                }
                willNotifyCollisions = true;
            }
        }
    }

    if(willNotifyCollisions)
        mEventManager->notify<CollisionInfo>();

    if(willNotifyGround)
        mEventManager->notify<TouchGround>();

    touchGround.clear();
    infos.clear();
}
