#pragma once

#include "CustomMath.hpp"

namespace gjt
{

inline float sign(const float number)
{
    return number >= 0 ? 1 : -1;
}

inline float lerp(const float start, const float end, const float value)
{
    return start + value * (end - start);
}

inline sf::Vector2f lerp2(
    const float startX, const float startY, const float endX, const float endY,
    const float value)
{
    return sf::Vector2f(lerp(startX, endX, value), lerp(startY, endY, value));
}

inline sf::Vector2f lerp2(
    const sf::Vector2f &start, const sf::Vector2f &end, const float value)
{
    return lerp2(start.x, start.y, end.x, end.y, value);
}

inline sf::Vector2f normalize(const sf::Vector2f &vector)
{
    const float mag = magnitude(vector);
    return mag == 0 ? vector : (vector / mag);
}

inline void normalize2(sf::Vector2f &vector)
{
    const float mag = magnitude(vector);
    if(mag == 0)
        vector /= mag;
}

inline float magnitude(const sf::Vector2f &vector)
{
    return sqrtf((vector.x * vector.x) + (vector.y * vector.y));
}

inline float rad_to_deg(const float rad)
{
    return rad * 180.0f / PI;
}

inline float deg_to_rad(const float deg)
{
    return deg * PI / 180.0f;
}

inline float angle(const sf::Vector2f& a, const sf::Vector2f& b)
{
    return (atan2f(b.y - a.y, b.x - a.x) * 180 / PI);
}

template <typename T>
inline float clamp(const T value, const T min, const T max)
{
    return value > max ? max : value < min ? min : value;
}

template <typename T>
inline float wrap(T value, const T min, const T max)
{
    while (value > max)
        value = min + (value - max);
    while (value < min)
        value = max - (min - value);
    return value;
}

}
