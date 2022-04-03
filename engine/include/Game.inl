#pragma once

#include "Game.hpp"
#include <imgui.h>
#include <imgui-SFML.h>

namespace gjt
{
template <typename T> void Game::switchState(std::shared_ptr<T> instance, bool unloadPrevious)
{
    if (currentState != nullptr && unloadPrevious)
    {
        currentState->unload();
    }

    currentState = instance;
    currentState->game = this;
    currentState->services = &services;

    currentState->load();
}

} // namespace gjt
