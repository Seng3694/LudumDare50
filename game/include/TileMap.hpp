#pragma once

#include <vector>
#include <cstdint>
#include <Tileset.hpp>
#include <memory>
#include <SFML/Graphics.hpp>
#include <cassert>

typedef uint8_t tile_t;

enum class TileType : tile_t 
{
    HighGrass,
    LowGrass,
    Walkway,
    Boulder
};

class TileMap : public sf::Transformable, public sf::Drawable
{
  public:
    TileMap(
        std::shared_ptr<gjt::Tileset> tileset, const uint32_t width,
        const uint32_t height, const tile_t tiles[]);
    ~TileMap();

    inline tile_t getTile(const uint32_t index) const
    {
        assert(index < (width * height));
        return tiles[index];
    }
    inline tile_t getTile(const uint32_t x, const uint32_t y) const
    {
        return getTile(y * width + x);
    }
    inline uint32_t getWidth() const
    {
        return width;
    }
    inline uint32_t getHeight() const
    {
        return height;
    }
    inline uint32_t getPixelWidth() const
    {
        return width * tileset->getTileWidth();
    }
    inline uint32_t getPixelHeight() const
    {
        return height * tileset->getTileHeight();
    }

  private: 
    std::shared_ptr<gjt::Tileset> tileset;
    tile_t *tiles;
    uint32_t width;
    uint32_t height;

    void draw(
        sf::RenderTarget &target, sf::RenderStates states) const override;
};
