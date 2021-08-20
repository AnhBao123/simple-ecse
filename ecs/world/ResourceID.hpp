#pragma once

#include<SFML/Graphics.hpp>

template<typename Resource, typename ID>
class ResourceHolder;

namespace Textures
{
    enum ID
    {
        Concrete,
        Earth,
        Grass,
        TextureCount,
    };
}

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;


