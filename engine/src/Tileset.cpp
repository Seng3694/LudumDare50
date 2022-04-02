#include "Tileset.hpp"

namespace gjt
{
Tileset::Tileset(
    std::shared_ptr<sf::Texture> tex, const uint32_t tw,
    const uint32_t th)
    : texture(tex), tileWidth(tw), tileHeight(th)
{
    const sf::Vector2u textureSize = texture->getSize();
    const uint32_t width = textureSize.x / tileWidth;
    const uint32_t height = textureSize.y / tileHeight;
    textureRectangles.reserve(width * height);

    for (uint32_t y = 0; y < height; ++y)
    {
        for (uint32_t x = 0; x < width; ++x)
        {
            textureRectangles.push_back(sf::IntRect(x * tileWidth, y * tileHeight, tileWidth, tileHeight));
        }
    }
}

}
    