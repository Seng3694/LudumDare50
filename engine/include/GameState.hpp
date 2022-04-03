#pragma once

#include <SFML/Graphics.hpp>
#include "ServiceLocator.hpp"

namespace gjt
{

class GameState
{
    friend class Game;
  public:
    virtual ~GameState(){};

    virtual void load(){};
    virtual void ui(float dt){};
    virtual void update(float dt) = 0;
    virtual void draw(
        float dt, sf::RenderTarget &target,
        sf::RenderStates states = sf::RenderStates()) = 0;
    virtual void handleEvent(const sf::Event &e, float dt){};
    virtual void unload(){};

  //protected:
    Game *game;
    ServiceLocator *services;
};
} // namespace gjt
