#pragma once

#include "ecs/core/ECSE.hpp"
#include "world/ResourceID.hpp"
#include "world/ResourceManager.hpp"

class RenderSystem : public System
{
public:
    void update(sf::Time dt) override;
    //void handleNotification(Event::ID type, const std::vector<Event::Info>& actions) override;
    void loadTexture(TextureHolder* textures);
    void setWindow(sf::RenderWindow* window);
private:
    TextureHolder* mTextures;
    sf::RenderWindow* mWindow;
    std::array<sf::VertexArray, Textures::TextureCount> mTArrays;
    std::array<sf::Vector2f, Textures::TextureCount> mTextureSize;
};
