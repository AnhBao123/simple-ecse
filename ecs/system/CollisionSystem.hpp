#pragma once

#include "ecs/core/ECSE.hpp"
#include "ecs/helper/Collision.hpp"

class CollisionSystem : public System
{
public:
    void update(sf::Time dt) override;
};
