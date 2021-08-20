#include "ecs/helper/Collision.hpp"
#include<iostream>

bool collidePoint_Rect(sf::Vector2f point, const sf::FloatRect& rect)
{
    return point.x > rect.left && point.x < rect.left + rect.width
           && point.y > rect.top && point.y < rect.top + rect.height;
}

bool AABB(const sf::FloatRect& rect1, const sf::FloatRect& rect2)
{
    sf::Vector2f halfSize(rect1.width/2 + rect2.width/2, rect1.height/2 + rect2.height/2);
    sf::Vector2f distance(abs(rect1.left + rect1.width/2 - rect2.left - rect2.width/2),
                          abs(rect1.top + rect1.height/2- rect2.top - rect2.height/2));

    return (halfSize.x > distance.x && halfSize.y > distance.y);
}

bool broadPhase(const sf::FloatRect& mover, sf::Vector2f velocity, const sf::FloatRect& target)
{
    sf::FloatRect bounds(mover);
    if(velocity.x < 0) bounds.left += velocity.x;
    if(velocity.y < 0) bounds.top += velocity.y;
    bounds.width += abs(velocity.x);
    bounds.height += abs(velocity.y);

    return AABB(bounds, target);
}

bool collideRay_rect(sf::Vector2f root, sf::Vector2f velocity, const sf::FloatRect& target,
              sf::Vector2f& normal, float& time)
{
    //move vertical
    if(velocity.x == 0.0f)
    {
        if(root.x <= target.left || root.x >= target.left + target.width)
            return false;
        if(root.y <= target.top && root.y + velocity.y <= target.top)
            return false;
        if(root.y >= target.top + target.height
           && root.y + velocity.y >= target.top + target.height)
            return false;

        float entry, exit;
        entry = (target.top - root.y)/velocity.y;
        exit = (target.top + target.height - root.y)/velocity.y;

        if(entry > exit)
            std::swap(entry, exit);
        time = entry;

        if(velocity.y > 0.0f)
            normal = sf::Vector2f(0.0f, -1.0f);
        else
            normal = sf::Vector2f(0.0f, 1.0f);

        return true;
    }
    //move horizontal
    else if(velocity.y == 0.0f)
    {
        if(root.y <= target.top || root.y >= target.top + target.height)
            return false;
        if(root.x <= target.left && root.x + velocity.x <= target.left)
            return false;
        if(root.x >= target.left + target.width
           && root.x + velocity.x >= target.left + target.width)
            return false;

        float entry, exit;
        entry = (target.left - root.x)/velocity.x;
        exit = (target.left + target.width - root.x)/velocity.x;

        if(entry > exit)
            std::swap(entry, exit);

        if(velocity.x > 0.0f)
            normal = sf::Vector2f(-1.0f, 0.0f);
        else
            normal = sf::Vector2f(1.0f, 0.0f);

        return true;
    }
    //move diagonal
    else
    {
        sf::Vector2f near, far;
        near.x = (target.left - root.x)/velocity.x;
        near.y = (target.top - root.y)/velocity.y;

        far.x = (target.left + target.width - root.x)/velocity.x;
        far.y = (target.top + target.height - root.y)/velocity.y;

        if(near.x > far.x)
            std::swap(near.x, far.x);
        if(near.y > far.y)
            std::swap(near.y, far.y);

        if(near.x >= far.y || near.y >= far.x)
            return false;
        if(near.x >= 1 && near.y >= 1)
            return false;
        if(far.x <= 0 && far.y <= 0)
            return false;


        if(near.x >= near.y)
        {
            time = near.x;
            if(velocity.x > 0.0f)
                normal = sf::Vector2f(-1.0f, 0.0f);
            else
                normal = sf::Vector2f(1.0f, 0.0f);
        }
        else
        {
            time = near.y;
            if(velocity.y > 0.0f)
                normal = sf::Vector2f(0.0f, -1.0f);
            else
                normal = sf::Vector2f(0.0f, 1.0f);
        }

        return true;
    }
}

bool collideEntity_entity(const sf::FloatRect& mover, const RigidBody& rigidBody, const sf::FloatRect& target,
                          sf::Time dt, sf::Vector2f& normal, float& time)
{
    sf::FloatRect boundsM, boundsT;
    boundsM = mover;
    boundsT = target;

    if(broadPhase(boundsM, rigidBody.velocity*dt.asSeconds(), boundsT))
    {
        boundsT.left -= boundsM.width/2;
        boundsT.top -= boundsM.height/2;
        boundsT.width += boundsM.width;
        boundsT.height += boundsM.height;

        sf::Vector2f moverPos(mover.left + mover.width/2.0f, mover.top + mover.height/2.0f);

        if(collideRay_rect(moverPos, rigidBody.velocity*dt.asSeconds(),
                           boundsT, normal, time))
            return true;
        else
            return false;
    }
    else
    {
        return false;
    }
}

void resolveAABB(RigidBody& mover, sf::Vector2f normal, float time)
{
    mover.velocity += sf::Vector2f(abs(mover.velocity.x)*normal.x*(1-time)*1.3f,
                                   abs(mover.velocity.y)*normal.y*(1-time)*1.3f);
}

