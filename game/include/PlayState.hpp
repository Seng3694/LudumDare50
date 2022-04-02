#pragma once

#include <SFML/Graphics.hpp>
#include <cstdio>

#include <imgui.h>

#include "Animation.hpp"
#include "AudioManager.hpp"
#include "ContentManager.hpp"
#include "CustomMath.hpp"
#include "GameState.hpp"
#include "Animation.hpp"
#include "Tileset.hpp"
#include "AnimatedSprite.hpp"
#include "TileMap.hpp"
#include "Map1.hpp"

#define STRINGIFY(x) (#x)

struct DebugUiContext
{
    sf::Vector2f mapPosition;
    float mapScale;
};

class PlayState : public gjt::GameState
{
  private:
    std::shared_ptr<sf::Font> font;
    std::shared_ptr<TileMap> map;
    bool enableDebug;
    DebugUiContext context;

  public:
    PlayState(gjt::Game *const game, gjt::ServiceLocator *const services)
        : gjt::GameState(game, services), font(nullptr), map(nullptr), enableDebug(false)
    {
    }

    virtual void load() override
    {
        auto content = services->resolve<gjt::ContentManager>();
        font =
            content->loadFromFile<sf::Font>("content/monogram-extended.ttf");

        map = std::make_shared<TileMap>(
            std::make_shared<gjt::Tileset>(
                content->loadFromFile<sf::Texture>("content/tiles.png"), 16,
                16),
            9, 7, map_1);

        map->setOrigin(
            map->getPixelWidth() / 2.0f, map->getPixelHeight() / 2.0f);
        map->setPosition(
            game->getWindowWidth() / 2.0f, game->getWindowHeight() / 2.0f);
        map->setScale(6.0f, 6.0f);

        game->setClearColor(sf::Color(0x008751ff));
        enableDebug = false;
    }

    virtual void update(float dt) override
    {
        context.mapPosition = map->getPosition();
        context.mapScale = map->getScale().x;   
    }

    virtual void ui(float dt) override
    {
        if (enableDebug)
        {
            ImGui::SetNextWindowSize(
                ImVec2(400, 400), ImGuiCond_::ImGuiCond_Once);
            if (ImGui::Begin("debug"))
            {
                if (ImGui::SliderFloat2("map pos", &context.mapPosition.x, 0, game->getWindowWidth()))
                    map->setPosition(context.mapPosition);
                if (ImGui::SliderFloat("map scale", &context.mapScale, 0.1f, 6.0f))
                    map->setScale(context.mapScale, context.mapScale);

                ImGui::End();
            }
        }
    }

    virtual void draw(
        float dt, sf::RenderTarget &target,
        sf::RenderStates states = sf::RenderStates()) override
    {
        target.draw(*map, states);
    }

    virtual void handleEvent(const sf::Event &e, float dt) override
    {
        if (e.type == sf::Event::KeyPressed)
        {
            if (e.key.code == sf::Keyboard::Key::D && e.key.control)
                enableDebug = !enableDebug;
        }
    }
};
