#pragma once

#include <SFML/Graphics.hpp>

namespace gjt
{
constexpr float PI = 3.1415927f;

inline float sign(const float number);

inline float lerp(const float start, const float end, const float value);
inline sf::Vector2f lerp2(
    const float startX, const float startY, const float endX, const float endY,
    const float value);
inline sf::Vector2f lerp2(
    const sf::Vector2f &start, const sf::Vector2f &end, const float value);

inline sf::Vector2f normalize(const sf::Vector2f &vector);
inline void normalize2(sf::Vector2f &vector);

inline float magnitude(const sf::Vector2f &vector);

inline float rad_to_deg(const float rad);
inline float deg_to_rad(const float deg);
inline float angle(const sf::Vector2f &a, const sf::Vector2f &b);

template <typename T>
inline float clamp(const T value, const T min, const T max);

template <typename T>
inline float wrap(const T value, const T min, const T max);

} // namespace gjt

#include "CustomMath.inl"
