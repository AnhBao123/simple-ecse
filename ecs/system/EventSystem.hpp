#pragma once

#include "ecs/core/System.hpp"

class EventSystem : public System
{
public:
    void update(sf::Time dt) override;
    void handleEvent(sf::Event& event);
};
