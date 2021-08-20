#include "ecs/system/RenderSystem.hpp"

void RenderSystem::update(sf::Time dt)
{
    for(Entity entity : mEntities)
    {
        const Render& render = mECSE->getComponent<Render>(entity);
        const Transform& transform = mECSE->getComponent<Transform>(entity);

        mTArrays[render.texture].append(
            sf::Vertex(sf::Vector2f(transform.rect.left, transform.rect.top),
                       sf::Vector2f(0.0f, 0.0f)));

        mTArrays[render.texture].append(
            sf::Vertex(sf::Vector2f(transform.rect.left + transform.rect.width, transform.rect.top),
                       sf::Vector2f(mTextureSize[render.texture].x, 0.0f)));

        mTArrays[render.texture].append(
            sf::Vertex(sf::Vector2f(transform.rect.left + transform.rect.width, transform.rect.top + transform.rect.height),
                       mTextureSize[render.texture]));

        mTArrays[render.texture].append(
            sf::Vertex(sf::Vector2f(transform.rect.left, transform.rect.top + transform.rect.height),
                       sf::Vector2f(0.0f, mTextureSize[render.texture].y)));
    }

    sf::RenderStates state;
    for(int i = 0; i < Textures::TextureCount; ++ i)
    {
        Textures::ID id = static_cast<Textures::ID>(i);
        state.texture = &mTextures->get(id);
        mWindow->draw(mTArrays[id], state);
        mTArrays[id].clear();
    }

}

void RenderSystem::loadTexture(TextureHolder* textures)
{
    mTextures = textures;
    for(int i = 0; i < Textures::TextureCount; ++ i)
    {
        Textures::ID id = static_cast<Textures::ID>(i);
        mTextureSize[i] = sf::Vector2f(textures->get(id).getSize());
        mTArrays[i].setPrimitiveType(sf::Quads);
    }
}

void RenderSystem::setWindow(sf::RenderWindow* window)
{
    mWindow = window;
}
