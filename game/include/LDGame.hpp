#pragma once

#include "Game.hpp"
#include "PlayState.hpp"
#include "Map1.hpp"
#include "Map2.hpp"

class LDGame : public gjt::Game
{
  public:
    LDGame(
        const std::string &title, const uint32_t width, const uint32_t height)
        : gjt::Game(title, width, height)
    {
    }

  protected:
    virtual void load() override
    {
        gjt::Game::load();
        switchState(std::make_shared<PlayState>(
            map_2, map_2_name, map_2_spawn));
    }
};
