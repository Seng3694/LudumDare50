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
    virtual ~Game(){}

    void switchState(
        std::shared_ptr<GameState> instance, bool unloadPrevious = true,
        bool loadNext = true);

    void run();

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
    inline const sf::View &getView()
    {
        return window.getView();
    }
    inline void setView(sf::View& view)
    {
        window.setView(view);
    }
    inline void resetView()
    {
        window.setView(window.getDefaultView());
    }
    inline std::shared_ptr<GameState> getCurrentState()
    {
        return currentState;
    }
    inline void shouldClose()
    {
        window.close();
    }
    inline void setWindowIcon(std::shared_ptr<sf::Image> image)
    {
        window.setIcon(
            image->getSize().x, image->getSize().y, image->getPixelsPtr());
    }

  protected:
    sf::RenderWindow window;
    ServiceLocator services;
    std::shared_ptr<GameState> currentState;
    sf::Color clearColor;
    const uint32_t windowWidth;
    const uint32_t windowHeight;

    virtual void load();
    virtual void update(float dt);
    virtual void ui(float dt);
    virtual void draw(
        float dt, sf::RenderTarget &target,
        sf::RenderStates states = sf::RenderStates::Default);
    virtual void handleEvent(const sf::Event &e, float dt);
    virtual void unload();
};

} // namespace gjt
