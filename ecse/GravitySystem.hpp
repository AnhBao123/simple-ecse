#pragma once

#include "ecs/core/ECSE.hpp"

class GravitySystem : public System
{
public:
    void update(sf::Time dt) override;
    void getNotification(const char* eventName) override;
};
