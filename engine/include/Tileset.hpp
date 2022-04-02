#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

namespace gjt
{

class Tileset
{
  public:
    Tileset(std::shared_ptr<sf::Texture> texture, const uint32_t tileWidth, const uint32_t tileHeight);

    inline std::shared_ptr<sf::Texture> getTexture() const
    {
        return texture;
    }

    inline sf::IntRect getTextureRect(const uint32_t index) const
    {
        return textureRectangles[index];
    }
    inline uint32_t getTileWidth() const
    {
        return tileWidth;
    }
    inline uint32_t getTileHeight() const
    {
        return tileHeight;
    }

  private:
    std::shared_ptr<sf::Texture> texture;
    std::vector<sf::IntRect> textureRectangles;
    uint32_t tileWidth;
    uint32_t tileHeight;
};

}