#pragma once

#include <vector>
#include <cstdint>
#include <Tileset.hpp>
#include <memory>
#include <SFML/Graphics.hpp>
#include <cassert>
#include "Tiles.hpp"

class TileMap : public sf::Transformable, public sf::Drawable
{
  public:
    TileMap(
        std::shared_ptr<gjt::Tileset> tileset, const uint32_t width,
        const uint32_t height, const uint8_t tiles[]);
    ~TileMap();

    inline TileType getTile(const uint32_t index) const
    {
        assert(index < (width * height));
        return tiles[index];
    }
    inline TileType getTile(const uint32_t x, const uint32_t y) const
    {
        return getTile(y * width + x);
    }
    inline void setTile(const uint32_t index, TileType id)
    {
        assert(index < (width * height));
        tiles[index] = id;
    }
    inline void setTile(const uint32_t x, const uint32_t y, TileType id)
    {
        setTile(y * width + x, id);
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
    inline std::shared_ptr<gjt::Tileset> getTileset() const
    {
        return tileset;
    }
    inline uint32_t getMaxScore() const
    {
        return maxScore;
    }
    inline void setRenderGrid(const bool render)
    {
        renderGrid = render;
    }

  private: 
    std::shared_ptr<gjt::Tileset> tileset;
    TileType *tiles;
    uint32_t width;
    uint32_t height;
    uint32_t maxScore;
    bool renderGrid;

    void draw(
        sf::RenderTarget &target, sf::RenderStates states) const override;
};
