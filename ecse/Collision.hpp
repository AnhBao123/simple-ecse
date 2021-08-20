#pragma once

#include<math.h>

#include "ecs/core/EnComSysEv.hpp"

bool collidePoint_Rect(sf::Vector2f point, const sf::FloatRect& rect);

bool AABB(const sf::FloatRect& rect1, const sf::FloatRect& rect2);

bool broadPhase(const sf::FloatRect& mover, sf::Vector2f velocity, const sf::FloatRect& target);

bool collideRay_rect(sf::Vector2f root, sf::Vector2f velocity, const sf::FloatRect& target,
              sf::Vector2f& normal, float& time);

bool collideEntity_entity(const sf::FloatRect& mover, const RigidBody& rigidBody, const sf::FloatRect& target,
                          sf::Time dt, sf::Vector2f& normal, float& time);

void resolveAABB(RigidBody& mover, sf::Vector2f normal, float time);
