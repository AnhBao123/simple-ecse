#pragma once

#include "ecs/core/ECSE.hpp"
#include "ecs/helper/Collision.hpp"

class HealthSystem : public System
{
public:
    void update(sf::Time dt) override;
    void render();
    bool isEntityAlive(Entity entity);
    void setWindow(sf::RenderWindow* window);
private:
    sf::RenderWindow* mWindow;
    sf::RectangleShape mHealthBar;
    sf::RectangleShape mBar;
};
