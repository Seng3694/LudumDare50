#pragma once

#include <SFML/Graphics.hpp>
#include "TileMap.hpp"
#include "Score.hpp"
#include <memory>
#include "Player.hpp"
#include "ServiceLocator.hpp"
#include "Line.hpp"

class MapView : public sf::Drawable, public sf::Transformable
{
  public:
    MapView(
        gjt::ServiceLocator *services,
        std::shared_ptr<TileMap> tileMap, const Score &score);
    MapView(
        gjt::ServiceLocator *services,
        std::shared_ptr<TileMap> tileMap,
        const std::vector<PlayerMove> &moves, const Score &score);

    void updateTextScale();

    inline void setFrameColor(const sf::Color &color)
    {
        frame.setColor(color);
    }

    inline float getPixelWidth()
    {
        return tileMap->getPixelWidth() + 12;
    }

    inline float getPixelHeight()
    {
        return tileMap->getPixelHeight() + 12;
    }

  private:
    std::shared_ptr<TileMap> tileMap;
    std::shared_ptr<sf::Font> font;
    std::shared_ptr<gjt::Tileset> tileset;
    std::vector<Line> lines;
    sf::Text clearTimeText;
    sf::Sprite stars[5];
    sf::Sprite frame;
    Score score;
    float starSpacing;
    float starOffset;
    bool renderMoves;

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};