#include "Game.hpp"

#include <cassert>
//#include <imgui.h>
//#include <imgui-SFML.h>
#include "AudioManager.hpp"
#include "ContentManager.hpp"

namespace gjt
{
Game::Game(
    const std::string &title, const uint32_t width, const uint32_t height)
    : services(), windowWidth(width), windowHeight(height)
{
    window.create(
        sf::VideoMode(width, height), title,
        sf::Style::Close | sf::Style::Titlebar);
}

void Game::switchState(
    std::shared_ptr<GameState> instance, bool unloadPrevious, bool loadNext)
{
    if (currentState != nullptr && unloadPrevious)
    {
        currentState->unload();
    }

    currentState = instance;
    currentState->game = this;
    currentState->services = &services;

    if (loadNext)
        currentState->load();
}

void Game::run()
{
    load();
    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Time dt = clock.restart();
        float dtf = dt.asSeconds();

        sf::Event e;

        while (window.pollEvent(e))
        {
            //ImGui::SFML::ProcessEvent(window, e);
            if (e.type == sf::Event::Closed)
                window.close();
            else
                handleEvent(e, dtf);
        }
        //ImGui::SFML::Update(window, dt);

        window.clear(clearColor);

        ui(dtf);
        update(dtf);
        draw(dtf, window);
        //ImGui::SFML::Render(window);

        window.display();
    }

    unload();
}

void Game::load()
{
    //ImGui::SFML::Init(window);
    services.registerInstance<ContentManager>();
}

void Game::update(float dt)
{
    assert(currentState != nullptr);
    currentState->update(dt);
}

void Game::ui(float dt)
{
    assert(currentState != nullptr);
    currentState->ui(dt);
}

void Game::draw(float dt, sf::RenderTarget &target, sf::RenderStates states)
{
    assert(currentState != nullptr);
    currentState->draw(dt, target, states);
}

void Game::handleEvent(const sf::Event &e, float dt)
{
    assert(currentState != nullptr);
    currentState->handleEvent(e, dt);
}

void Game::unload()
{
    //ImGui::SFML::Shutdown(window);
}
} // namespace gjt
