#pragma once

#include "Game.hpp"
#include "PlayState.hpp"
#include "MapSelectionState.hpp"

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
        switchState(std::make_shared<MapSelectionState>());
    }
};
