#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "GameState.hpp"
#include "ServiceLocator.hpp"

namespace gjt
{
class Game
{
  public:
    Game(
        const std::string &title, const uint32_t width, const uint32_t height);

    template <typename T> void switchState();

    template <typename T> void run();

    inline uint32_t getWindowWidth() const
    {
        return windowWidth;
    }
    inline uint32_t getWindowHeight() const
    {
        return windowHeight;
    }
    inline void setClearColor(const sf::Color &color)
    {
        clearColor = color;
    }

  private:
    sf::RenderWindow window;
    ServiceLocator services;
    std::shared_ptr<GameState> currentState;
    sf::Color clearColor;
    const uint32_t windowWidth;
    const uint32_t windowHeight;

    void load();
    void update(float dt);
    void ui(float dt);
    void draw(
        float dt, sf::RenderTarget &target,
        sf::RenderStates states = sf::RenderStates::Default);
    void handleEvent(const sf::Event &e, float dt);
    void unload();
};

} // namespace gjt

#include "Game.inl"
