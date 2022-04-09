#include "Maps.hpp"

sf::String map_1_name = "Cross";
uint8_t map_1[] =  
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
sf::Vector2u map_1_spawn = {5, 4};

sf::String map_2_name = "Catwalk";
uint8_t map_2[] =  
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
sf::Vector2u map_2_spawn = {5, 5};

sf::String map_3_name = "Scattered";
uint8_t map_3[] =  
   {3,1,1,3,1,1,1,1,3,1,1,3,
    1,1,1,1,1,3,3,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,
    3,1,1,3,1,1,1,1,3,1,1,3,
    1,1,1,1,1,2,2,1,1,1,1,1,
    1,1,1,1,1,2,2,1,1,1,1,1,
    3,1,1,3,1,1,1,1,3,1,1,3,
    1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,3,3,1,1,1,1,1,
    3,1,1,3,1,1,1,1,3,1,1,3};
sf::Vector2u map_3_spawn = {5, 5};

sf::String map_4_name = "Crab";
uint8_t map_4[] =  
   {1,1,1,1,1,1,1,1,1,1,1,1,
    1,3,1,1,1,1,1,1,1,1,3,1,
    1,1,1,3,3,1,1,3,3,1,1,1,
    1,1,3,2,2,2,2,2,2,3,1,1,
    1,1,3,1,1,1,1,1,1,3,1,1,
    1,1,3,1,1,1,1,1,1,3,1,1,
    1,1,3,3,1,1,1,1,3,3,1,1,
    1,1,1,3,3,1,1,3,3,1,1,1,
    1,3,1,1,1,1,1,1,1,1,3,1,
    1,1,1,1,1,1,1,1,1,1,1,1};
sf::Vector2u map_4_spawn = {5, 3};

sf::String map_5_name = "Lonely";
uint8_t map_5[] =  
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
sf::Vector2u map_5_spawn = {0, 5};

sf::String map_6_name = "Stairs";
uint8_t map_6[] =  
   {1,1,1,1,1,3,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,3,1,1,1,2,2,2,2,1,1,
    1,1,1,1,1,1,2,2,2,2,1,1,
    1,1,1,1,2,2,2,2,1,1,1,1,
    1,1,1,1,2,2,2,2,1,1,1,1,
    1,1,2,2,2,2,1,1,1,1,1,1,
    1,1,2,2,2,2,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,3,3,1,
    1,1,1,1,1,1,1,1,1,1,1,1};
sf::Vector2u map_6_spawn = {5, 7};

sf::String map_7_name = "Ledge";
uint8_t map_7[] =  
   {2,2,2,1,1,1,1,1,1,1,1,1,
    2,2,2,1,1,1,1,3,1,1,1,1,
    2,2,2,2,1,1,1,1,1,3,1,1,
    1,1,2,2,2,1,1,1,1,1,1,1,
    1,1,1,2,2,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,3,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1};
sf::Vector2u map_7_spawn = {1, 1};

sf::String map_8_name = "Neglected";
uint8_t map_8[] =  
   {3,1,1,3,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,3,1,
    1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,3,1,1,1,1,
    1,1,1,1,3,1,1,1,1,1,1,1,
    1,1,1,1,1,1,2,1,1,3,1,1,
    1,1,1,1,1,2,2,1,1,1,1,1,
    1,1,2,1,1,2,2,1,1,1,1,1,
    1,1,2,1,1,2,2,1,1,2,1,1,
    1,1,2,2,2,2,2,2,2,2,1,1};
sf::Vector2u map_8_spawn = {2, 7};

sf::String map_9_name = "Unkempt";
uint8_t map_9[] =  
   {2,2,2,2,3,3,3,3,2,2,2,2,
    2,1,1,1,1,1,1,1,1,1,1,2,
    2,1,1,1,1,1,1,3,1,1,1,2,
    2,1,1,1,3,1,1,1,1,1,1,2,
    2,1,1,1,1,1,1,1,1,1,1,2,
    2,1,1,1,1,1,1,1,1,1,1,2,
    2,1,3,1,1,1,1,1,1,1,1,2,
    2,1,1,1,1,1,1,3,1,1,1,2,
    2,1,1,1,1,1,1,1,1,1,1,2,
    2,2,2,2,2,2,2,2,2,2,2,2};
sf::Vector2u map_9_spawn = {0, 5};

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
    case Maps::Map5:
        return map_5_name;
    case Maps::Map6:
        return map_6_name;
    case Maps::Map7:
        return map_7_name;
    case Maps::Map8:
        return map_8_name;
    case Maps::Map9:
        return map_9_name;
    default:
        return map_1_name;
    }
}

const uint8_t *get_map_data(Maps maps)
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
    case Maps::Map5:
        return map_5;
    case Maps::Map6:
        return map_6;
    case Maps::Map7:
        return map_7;
    case Maps::Map8:
        return map_8;
    case Maps::Map9:
        return map_9;
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
    case Maps::Map5:
        return map_5_spawn;
    case Maps::Map6:
        return map_6_spawn;
    case Maps::Map7:
        return map_7_spawn;
    case Maps::Map8:
        return map_8_spawn;
    case Maps::Map9:
        return map_9_spawn;
    default:
        return map_1_spawn;
    }
}
