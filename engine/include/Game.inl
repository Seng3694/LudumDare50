#pragma once

#include "Game.hpp"
#include <imgui.h>
#include <imgui-SFML.h>

namespace gjt
{
template <typename T> void Game::switchState()
{
    if (currentState != nullptr)
    {
        currentState->unload();
    }

    currentState = std::make_shared<T>(this, &services);
    currentState->load();
}

template <typename T> void Game::run()
{
    load();
    switchState<T>();
    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Time dt = clock.restart();
        float dtf = dt.asSeconds();

        sf::Event e;

        while (window.pollEvent(e))
        {
            ImGui::SFML::ProcessEvent(window, e);
            if (e.type == sf::Event::Closed)
                window.close();
            else
                handleEvent(e, dtf);
        }
        ImGui::SFML::Update(window, dt);

        window.clear(clearColor);

        ui(dtf);
        update(dtf);
        draw(dtf, window);
        ImGui::SFML::Render(window);

        window.display();
    }

    unload();
}
} // namespace gjt
