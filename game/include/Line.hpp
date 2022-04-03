#pragma once

#include <cstdint>
#include <SFML/Graphics.hpp>
#include <CustomMath.hpp>

class Line : public sf::Drawable
{
  public:
    Line(){}
    Line(const sf::Vector2f& from, const sf::Vector2f &to, const sf::Color &color, const float thickness)
    {
        sf::Vector2f direction = gjt::normalize(to - from);
        sf::Vector2f perpendicular(-direction.y, direction.x);

        sf::Vector2f offset = (thickness / 2.0f) * perpendicular;

        vertices[0].position = from + offset;
        vertices[1].position = to + offset;
        vertices[2].position = from - offset;
        vertices[3].position = to - offset;

        for (uint32_t i = 0; i < 4; ++i)
            vertices[i].color = color;
    }

  protected:
    sf::Vertex vertices[4];

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        target.draw(vertices, 4, sf::TriangleStrip, states);
    }
};
