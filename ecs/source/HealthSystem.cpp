#include "ecs/system/HealthSystem.hpp"

void HealthSystem::update(sf::Time dt)
{
    std::vector<Entity> mDie;
    for(Entity hitter : mEntities)
    {
        HealthBox& hitbox = mECSE->getComponent<HealthBox>(hitter);
        const Transform& transform = mECSE->getComponent<Transform>(hitter);

        //set hit/hurt box at center of transform rect
        hitbox.hitBox.left = transform.rect.left + (transform.rect.width - hitbox.hitBox.width)/2.0f;
        hitbox.hitBox.top = transform.rect.top + (transform.rect.height - hitbox.hitBox.height)/2.0f;
        hitbox.hurtBox.left = transform.rect.left + (transform.rect.width - hitbox.hurtBox.width)/2.0f;
        hitbox.hurtBox.top = transform.rect.top + (transform.rect.height - hitbox.hurtBox.height)/2.0f;

        if(hitbox.hasHitbox)
        {
            for(Entity getter : mEntities)
            {
                const HealthBox& hurtBox = mECSE->getComponent<HealthBox>(getter);
                if(hitter != getter && hurtBox.hasHealth)
                {
                    if(AABB(hitbox.hitBox, hurtBox.hurtBox))
                    {
                        Health& health = mECSE->getComponent<Health>(getter);
                        health.currentHP -= hitbox.damage*dt.asSeconds();
                        if(health.currentHP < 0)
                            mDie.push_back(getter);
                    }
                }
            }
        }
    }

    for(auto itr = mDie.begin(); itr != mDie.end(); ++ itr)
    {
        mECSE->destroyEntity((*itr));
    }
}

void HealthSystem::render()
{
    if(mEntities.empty()) return;
    for(Entity entity : mEntities)
    {
        const Health& health = mECSE->getComponent<Health>(entity);
        const Transform& transform = mECSE->getComponent<Transform>(entity);
        const HealthBox& box = mECSE->getComponent<HealthBox>(entity);

        if(box.hasHealth)
        {
            if(health.currentHP < health.maxHP/3.0f)
                mHealthBar.setFillColor(sf::Color::Red);
            mHealthBar.setSize(sf::Vector2f(150.0f*health.currentHP/health.maxHP, 10.0f));
            mHealthBar.setPosition(transform.rect.left - 30.0f, transform.rect.top - 100.0f);
            mBar.setPosition(transform.rect.left - 30.0f, transform.rect.top - 100.0f);
            mWindow->draw(mHealthBar);
            mWindow->draw(mBar);
        }

        //draw hit and hurt box
        sf::RectangleShape shape;
        shape.setFillColor(sf::Color(0xff000066));
        shape.setOutlineColor(sf::Color::Red);
        shape.setOutlineThickness(1.0f);
        shape.setPosition(box.hitBox.left, box.hitBox.top);
        shape.setSize(sf::Vector2f(box.hitBox.width, box.hitBox.height));
        if(box.hasHitbox)
            mWindow->draw(shape);
        shape.setFillColor(sf::Color(0x00ff0066));
        shape.setOutlineColor(sf::Color::Green);
        shape.setPosition(box.hurtBox.left, box.hurtBox.top);
        shape.setSize(sf::Vector2f(box.hurtBox.width, box.hurtBox.height));
        if(box.hasHealth)
            mWindow->draw(shape);
    }
}

bool HealthSystem::isEntityAlive(Entity entity)
{
    bool alive = false;
    for(Entity en : mEntities)
    {
        if(en == entity)
        {
            alive = true;
            break;
        }
    }
    return alive;
}

void HealthSystem::setWindow(sf::RenderWindow* window)
{
    mWindow = window;
    mHealthBar.setSize(sf::Vector2f(150.0f, 10.0f));
    mHealthBar.setFillColor(sf::Color::Green);
    mBar.setSize(sf::Vector2f(150.0f, 10.0f));
    mBar.setOutlineColor(sf::Color::White);
    mBar.setFillColor(sf::Color::Transparent);
    mBar.setOutlineThickness(1.0f);
}
