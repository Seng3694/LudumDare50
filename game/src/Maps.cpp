#include "Maps.hpp"

sf::String map_1_name = "The Lonely Rock";
uint8_t map_1[] =  
   {2,2,2,2,2,2,2,2,2,2,2,2,
    2,1,1,1,1,1,1,1,1,1,1,2,
    2,1,1,1,1,1,1,1,1,1,1,2,
    2,1,1,1,1,1,1,1,1,1,1,2,
    2,1,1,1,1,1,1,1,1,1,1,2,
    2,1,1,1,1,3,1,1,1,1,1,2,
    2,1,1,1,1,1,1,1,1,1,1,2,
    2,1,1,1,1,1,1,1,1,1,1,2,
    2,1,1,1,1,1,1,1,1,1,1,2,
    2,2,2,2,2,2,2,2,2,2,2,2};
sf::Vector2u map_1_spawn = {0, 5};

sf::String map_2_name = "The Cross";
uint8_t map_2[] =  
   {1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,3,1,1,2,2,1,1,3,1,1,
    1,1,1,1,1,2,2,1,1,1,1,1,
    1,1,2,2,2,2,2,2,2,2,1,1,
    1,1,2,2,2,2,2,2,2,2,1,1,
    1,1,1,1,1,2,2,1,1,1,1,1,
    1,1,3,1,1,2,2,1,1,3,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1};
sf::Vector2u map_2_spawn = {5, 4};

sf::String map_3_name = "The Catwalk";
uint8_t map_3[] =  
   {2,1,1,1,1,1,1,1,1,1,1,1,
    2,1,3,1,1,1,3,1,1,1,1,1,
    2,1,1,1,3,1,1,1,3,1,1,1,
    2,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,1,1,
    2,2,2,2,2,2,2,2,2,2,1,1,
    2,1,1,1,1,1,1,1,1,1,1,1,
    2,1,1,1,3,1,1,1,3,1,1,1,
    2,1,3,1,1,1,3,1,1,1,1,1,
    2,1,1,1,1,1,1,1,1,1,1,1};
sf::Vector2u map_3_spawn = {5, 5};

sf::String map_4_name = "The Debug";
uint8_t map_4[] =  
   {2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,1,1,
    2,2,2,2,2,2,2,2,2,2,1,1};
sf::Vector2u map_4_spawn = {9, 7};

const sf::String &get_map_name(Maps maps)
{
    switch (maps)
    {
    case Maps::Map1:
        return map_1_name;
    case Maps::Map2:
        return map_2_name;
    case Maps::Map3:
        return map_3_name;
    case Maps::Map4:
        return map_4_name;
    default:
        return map_1_name;
    }
}
const uint8_t const *get_map_data(Maps maps)
{
    switch (maps)
    {
    case Maps::Map1:
        return map_1;
    case Maps::Map2:
        return map_2;
    case Maps::Map3:
        return map_3;
    case Maps::Map4:
        return map_4;
    default:
        return map_1;
    }
}

const sf::Vector2u &get_map_spawn(Maps maps)
{
    switch (maps)
    {
    case Maps::Map1:
        return map_1_spawn;
    case Maps::Map2:
        return map_2_spawn;
    case Maps::Map3:
        return map_3_spawn;
    case Maps::Map4:
        return map_4_spawn;
    default:
        return map_1_spawn;
    }
}
