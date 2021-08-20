#pragma once

#include "ecs/core/ECSE.hpp"
#include "ecs/helper/Collision.hpp"

class RigidBodySystem : public System
{
public:
    void update(sf::Time dt) override;
    void getNotification(const char* eventType) override;
};

