#pragma once

#include <cstdint>
#include <SFML/Graphics.hpp>
#include <memory>
#include <Tileset.hpp>
#include "Tiles.hpp"
#include <cassert>


template <const TileType left, const TileType center>
class ResourceBar : public sf::Transformable, public sf::Drawable
{
  public:
    ResourceBar(
        std::shared_ptr<gjt::Tileset> ts, const int32_t const *v,
        const int32_t max)
        : tileset(ts), value(v), max(max)
    {
        assert(max > 2);
    }

    inline uint32_t getPixelWidth() const
    {
        return max * tileset->getTileWidth();
    }
    inline uint32_t getPixelHeight() const
    {
        return tileset->getTileHeight();
    }

  private:
    const int32_t const *value;
    const int32_t max;
    std::shared_ptr<gjt::Tileset> tileset;

    virtual void draw(
        sf::RenderTarget &target, sf::RenderStates states) const override
    {
        states.transform.combine(getTransform());
        sf::Sprite s(*tileset->getTexture());
        s.setPosition(0, 0);

        const uint32_t leftID = (uint32_t)left;
        const uint32_t centerID = (uint32_t)center;
        const uint32_t leftEmptyID = (uint32_t)TileType::ResourceBarLeftEmpty;
        const uint32_t centerEmptyID = (uint32_t)TileType::ResourceBarCenterEmpty;

        s.setTextureRect(tileset->getTextureRect(*value > 0 ? leftID : leftEmptyID));
        target.draw(s, states);

        for (uint32_t i = 1; i < max - 1; ++i)
        {
            s.setPosition(i * tileset->getTileWidth(), 0);
            s.setTextureRect(tileset->getTextureRect(*value > i ? centerID : centerEmptyID));
            target.draw(s, states);
        }

        s.setPosition(max * tileset->getTileWidth(), 0);
        s.setTextureRect(tileset->getTextureRect(*value == max ? leftID : leftEmptyID));
        s.setScale(-1, 1);
        target.draw(s, states);
    }
};
