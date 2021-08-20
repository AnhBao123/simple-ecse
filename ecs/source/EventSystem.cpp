#include "ecs/system/EventSystem.hpp"
#include "ecs/core/EventManager.hpp"

void EventSystem::update(sf::Time dt)
{
    bool willNotify = false;
    std::vector<Movement>& movements = mEventManager->getEventData<Movement>();

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        movements.push_back(Movement
        {
           .direction = sf::Vector2f(1.0f, 0.0f),
        });
        willNotify = true;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        movements.push_back(Movement
        {
            .direction = sf::Vector2f(-1.0f, 0.0f),
        });
        willNotify = true;
    }
    if(willNotify)
        mEventManager->notify<Movement>();

    movements.clear();
}

void EventSystem::handleEvent(sf::Event& event)
{
    bool willNotify = false;
    std::vector<Movement>& movements = mEventManager->getEventData<Movement>();
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z)
    {
        movements.push_back(Movement
        {
            .direction = sf::Vector2f(0.0f, -1.0f),
        });
        willNotify = true;
    }

    if(willNotify)
        mEventManager->notify<Movement>();

    movements.clear();
}
