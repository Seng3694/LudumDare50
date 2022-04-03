#pragma once

#include <SFML/Graphics.hpp>

enum class Maps
{
    Map1,
    Map2,
    Map3,
    Map4,
    Map5,
    Map6,
    Map7,
    Map8,
    Map9,
    Count
};

extern sf::String map_1_name;
extern uint8_t map_1[];
extern sf::Vector2u map_1_spawn;

extern sf::String map_2_name;
extern uint8_t map_2[];
extern sf::Vector2u map_2_spawn;

extern sf::String map_3_name;
extern uint8_t map_3[];
extern sf::Vector2u map_3_spawn;

extern sf::String map_4_name;
extern uint8_t map_4[];
extern sf::Vector2u map_4_spawn;

extern sf::String map_5_name;
extern uint8_t map_5[];
extern sf::Vector2u map_5_spawn;

extern sf::String map_6_name;
extern uint8_t map_6[];
extern sf::Vector2u map_6_spawn;

extern sf::String map_7_name;
extern uint8_t map_7[];
extern sf::Vector2u map_7_spawn;

extern sf::String map_8_name;
extern uint8_t map_8[];
extern sf::Vector2u map_8_spawn;

extern sf::String map_9_name;
extern uint8_t map_9[];
extern sf::Vector2u map_9_spawn;

const sf::String &get_map_name(Maps maps);
const uint8_t const *get_map_data(Maps maps);
const sf::Vector2u &get_map_spawn(Maps maps);
