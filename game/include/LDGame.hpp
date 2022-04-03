#pragma once

#include "Game.hpp"
#include "PlayState.hpp"
#include "MapSelectionState.hpp"
#include "SaveFileManager.hpp"
#include "TitleState.hpp"

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

        auto content = services.resolve<gjt::ContentManager>();
        content->loadFromFile<sf::Font>("content/monogram-extended.ttf");
        content->loadFromFile<sf::Texture>("content/player.png");
        content->loadFromFile<sf::Texture>("content/map_frame.png");
        content->loadFromFile<sf::Texture>("content/scrolling_rectangle.png");
        content->loadFromFile<sf::Texture>("content/tiles.png");
        content->loadFromFile<sf::Texture>(
            "content/transition_banner_left.png");
        content->loadFromFile<sf::Texture>(
            "content/transition_banner_right.png");
        content->loadFromFile<sf::Texture>(
            "content/splash.png");


        /*switchState(
            std::static_pointer_cast<gjt::GameState, MapSelectionState>(
                std::make_shared<MapSelectionState>()));*/
        switchState(
            std::static_pointer_cast<gjt::GameState, TitleState>(
            std::make_shared<TitleState>()));
    }
};
