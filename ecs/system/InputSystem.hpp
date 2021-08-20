#pragma once

#include "ecs/core/ECSE.hpp"

class InputSystem : public System
{
public:
    InputSystem();
    void update(sf::Time dt) override;
    virtual void getNotification(const char* eventType);
};

