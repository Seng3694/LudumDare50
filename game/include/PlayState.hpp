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
#include "Player.hpp"
#include "ResourceBar.hpp"

#define STRINGIFY(x) (#x)

struct DebugUiContext
{
    sf::Vector2f mapPosition;
    sf::Vector2i cameraCenter;
    float mapScale;
};

typedef ResourceBar<
    TileType::ResourceBarLeftYellow, TileType::ResourceBarCenterYellow>
    EnergyBar;
typedef ResourceBar<
    TileType::ResourceBarLeftGreen, TileType::ResourceBarCenterGreen>
    GrassMownBar;
typedef ResourceBar<
    TileType::ResourceBarLeftRed, TileType::ResourceBarCenterRed>
    ScoreBar;

class PlayState : public gjt::GameState
{
  private:
    std::shared_ptr<sf::Font> font;
    std::shared_ptr<gjt::Tileset> tileset;
    std::shared_ptr<TileMap> map;
    std::shared_ptr<Player> player;
    std::shared_ptr<EnergyBar> energyBar;
    std::shared_ptr<GrassMownBar> grassMownBar;
    sf::Text totalTimeText;
    float totalTime;
    bool enableDebug;
    DebugUiContext context;

  public:
    PlayState(gjt::Game *const game, gjt::ServiceLocator *const services)
        : gjt::GameState(game, services), font(nullptr), map(nullptr), player(nullptr), enableDebug(false)
    {
    }

    virtual void load() override
    {
        auto content = services->resolve<gjt::ContentManager>();
        font =
            content->loadFromFile<sf::Font>("content/monogram-extended.ttf");

        tileset = std::make_shared<gjt::Tileset>(
            content->loadFromFile<sf::Texture>("content/tiles.png"), 16, 16);

        map = std::make_shared<TileMap>(tileset, 9, 7, map_1);

        /*map->setOrigin(
            map->getPixelWidth() / 2.0f, map->getPixelHeight() / 2.0f);
        map->setPosition(
            game->getWindowWidth() / 2.0f, game->getWindowHeight() / 2.0f);*/

        map->setPosition(0, 16);

        player = std::shared_ptr<Player>(new Player(map));
        player->setIsAnimating(false);
        player->addAnimation(
            PlayerAnimationState::Idle,
            std::make_shared<gjt::Animation>(
                std::make_shared<gjt::Tileset>(
                    content->loadFromFile<sf::Texture>("content/player.png"), 16, 16),
                1, 0.1f, true));

        player->setMapPosition(0, (uint32_t)map->getHeight() / 2.0f);

        energyBar = std::make_shared<EnergyBar>(tileset, &player->getHp(), 5);
        energyBar->setScale(0.5f, 0.5f);

        grassMownBar = std::make_shared<GrassMownBar>(
            tileset, &player->getGrassMown(), map->getMaxScore());
        grassMownBar->setScale(
            (float)game->getWindowWidth() /
                (grassMownBar->getPixelWidth() * 6),           
            0.5f);
        grassMownBar->setPosition(0, energyBar->getPixelHeight() * 0.5f);

        game->setClearColor(sf::Color(0x5F574fff));
        sf::View view = game->getView();
        view.setCenter(game->getWindowWidth() / 6.0f / 2.0f, game->getWindowHeight() / 6.0f / 2.0f);
        view.zoom(1 / 6.0f);
        game->setView(view);

        totalTime = 0.0f;

        enableDebug = false;
    }

    virtual void update(float dt) override
    {
        totalTime += dt;
        player->update(dt);
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

                ImGui::LabelText("hp", "%d", player->getHp());
                
                sf::View view = game->getView();
                context.cameraCenter.x = view.getCenter().x;
                context.cameraCenter.y = view.getCenter().y;
                if (ImGui::SliderInt2("cam pos", &context.cameraCenter.x, 0, 1000))
                {
                    view.setCenter(context.cameraCenter.x, context.cameraCenter.y);
                    game->setView(view);
                }
                

                ImGui::End();
            }
        }
    }

    virtual void draw(
        float dt, sf::RenderTarget &target,
        sf::RenderStates states = sf::RenderStates()) override
    {
        target.draw(*map, states);
        target.draw(*player, states);
        target.draw(*energyBar, states);
        target.draw(*grassMownBar, states);
    }

    virtual void handleEvent(const sf::Event &e, float dt) override
    {
        if (e.type == sf::Event::KeyPressed)
        {
            if (e.key.code == sf::Keyboard::Key::D && e.key.control)
            {
                enableDebug = !enableDebug;
                return;
            }
            
            if (e.key.code == sf::Keyboard::Key::D || 
                e.key.code == sf::Keyboard::Key::Right)
            {
                player->moveTo(MoveDirection::Right);
            }
            if (e.key.code == sf::Keyboard::Key::A ||
                e.key.code == sf::Keyboard::Key::Left)
            {
                player->moveTo(MoveDirection::Left);
            }
            if (e.key.code == sf::Keyboard::Key::W ||
                e.key.code == sf::Keyboard::Key::Up)
            {
                player->moveTo(MoveDirection::Up);
            }
            if (e.key.code == sf::Keyboard::Key::S ||
                e.key.code == sf::Keyboard::Key::Down)
            {
                player->moveTo(MoveDirection::Down);
            }
        }
    }
};
