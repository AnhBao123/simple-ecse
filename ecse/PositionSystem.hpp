#pragma once

#include "ecs/core/ECSE.hpp"

class PositionSystem : public System
{
public:
    void update(sf::Time dt) override;
};


