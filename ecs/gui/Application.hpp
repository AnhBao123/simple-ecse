#pragma once

#include<SFML/Graphics.hpp>

#include "ecs/system/RenderSystem.hpp"
#include "ecs/system/RigidBodySystem.hpp"
#include "ecs/system/PositionSystem.hpp"
#include "ecs/system/GravitySystem.hpp"
#include "ecs/system/InputSystem.hpp"
#include "ecs/system/EventSystem.hpp"
#include "ecs/system/CollisionSystem.hpp"
#include "ecs/system/HealthSystem.hpp"

class Application
{
public:
    Application();
    ~Application();
    void run();
private:
    void process();
    void update(sf::Time dt);
    void render();

    void setup();
private:
    sf::RenderWindow mWindow;
    sf::View mView;
    const sf::Time TIME_PER_FRAME;

    TextureHolder mTextures;

    ECSE mECSE;
    std::shared_ptr<RenderSystem> mRenderer;
    std::shared_ptr<RigidBodySystem> mRigidSystem;
    std::shared_ptr<PositionSystem> mPositioner;
    std::shared_ptr<GravitySystem> mGravity;
    std::shared_ptr<InputSystem> mMover;
    std::shared_ptr<EventSystem> mListener;
    std::shared_ptr<CollisionSystem> mCollider;
    std::shared_ptr<HealthSystem> mHealth;
    std::vector<Entity> mEntities;
};

