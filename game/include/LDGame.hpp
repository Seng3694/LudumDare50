#pragma once

#include "Game.hpp"
#include "PlayState.hpp"
#include "MapSelectionState.hpp"
#include "SaveFileManager.hpp"

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
        services.registerInstance<SaveFileManager>(
            std::make_shared<SaveFileManager>("save.bin"));
        switchState(std::make_shared<MapSelectionState>());
    }
};
