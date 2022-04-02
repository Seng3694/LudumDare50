#include "TileMap.hpp"
#include <memory>

TileMap::TileMap(
    std::shared_ptr<gjt::Tileset> ts, const uint32_t w, const uint32_t h,
    const uint8_t tiles[])
    : tileset(ts), maxScore(0), width(w), height(h)
{
    const uint32_t size = w * h;
    this->tiles = new TileType[size];
    for (uint32_t i = 0; i< size; ++i)
    {
        this->tiles[i] = (TileType)tiles[i];
        if (this->tiles[i] == TileType::HighGrass)
            maxScore++;
    }
}

TileMap::~TileMap()
{
    delete[] tiles;
}

void TileMap::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform.combine(getTransform());
    sf::Sprite sprite;
    sprite.setTexture(*tileset->getTexture());
    sf::RectangleShape gridRect;

    gridRect.setFillColor(sf::Color::Transparent);
    const float gridOutlineThickness = 0.1f;
    gridRect.setSize(sf::Vector2f(
        tileset->getTileWidth() - gridOutlineThickness * 2,
        tileset->getTileWidth() - gridOutlineThickness * 2));
    gridRect.setOutlineThickness(gridOutlineThickness);
    gridRect.setOutlineColor(sf::Color(0x5f574fff));

    for (uint32_t y = 0; y < height; ++y)
    {
        for (uint32_t x = 0; x < width; ++x)
        {
            TileType tile = getTile(x, y);
            sprite.setPosition(
                x * tileset->getTileWidth(), y * tileset->getTileHeight());

            sprite.setTextureRect(tileset->getTextureRect(0));
            target.draw(sprite, states);

            if (tile != TileType::LowGrass)
            {
                sprite.setTextureRect(tileset->getTextureRect((uint32_t)tile));
                target.draw(sprite, states);
            }
            
            gridRect.setPosition(
                x * tileset->getTileWidth() + gridOutlineThickness,
                y * tileset->getTileHeight() + gridOutlineThickness);
            target.draw(gridRect, states);
        }
    }
}
