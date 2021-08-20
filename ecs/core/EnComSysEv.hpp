#pragma once

#include<numeric>
#include<bitset>

#include<SFML/Graphics.hpp>

#include "world/ResourceID.hpp"

typedef std::uint_least32_t Entity;
constexpr Entity MAX_ENTITIES = 1000;

typedef std::uint_least16_t ComponentType;
constexpr ComponentType MAX_COMPONENTS = 32;

typedef std::bitset<MAX_COMPONENTS> Signature;

//component
struct Transform
{
    sf::FloatRect rect;
};

struct RigidBody
{
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    float slowFriction;
};

struct Jump
{
    float gravity;
    float jumpHeight;
    int maxJump;
    int currentJump;
};

struct Render
{
    Textures::ID texture;
};

struct Health
{
    float maxHP;
    float currentHP;
};

struct HealthBox
{
    bool hasHealth;
    sf::FloatRect hurtBox;
    bool hasHitbox;
    sf::FloatRect hitBox;
    float damage;
};

//movement is both component and event
struct Movement
{
    sf::Vector2f direction;
    float absAcceleration;
};

//events
struct CollisionInfo
{
    Entity entity;
    float time;
    sf::Vector2f normal;
};

struct TouchGround
{
    Entity entity;
};
