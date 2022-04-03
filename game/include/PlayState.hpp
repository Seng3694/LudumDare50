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
#include "Player.hpp"
#include "ResourceBar.hpp"
#include "StringFormat.hpp"
#include "SaveFileManager.hpp"
#include "VictoryState.hpp"
#include "TransitionState.hpp"

class MapSelectionState;

#define STRINGIFY(x) (#x)

#define TILE_SCALING (4.0f)

struct PlayStateDebugUiContext
{
    sf::Vector2f mapPosition;
    sf::Vector2i cameraCenter;
    sf::Vector2f grassMownBarPosition;
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
    std::shared_ptr<SaveData> saveData;
    sf::Text totalTimeText;
    sf::Text mapNameText;
    char totalTimeTextBuffer[10];
    float totalTime;
    float resetTimer;
    float resetTimerElapsed;
    float resetStackCount;
    const uint8_t const *mapData;
    sf::String mapName;
    sf::Vector2u mapSpawn;
    const Maps mapID;

    bool enableDebug;
    PlayStateDebugUiContext context;

  public:
    PlayState(const Maps mapID)
        : font(nullptr), map(nullptr), player(nullptr), saveData(nullptr), enableDebug(false), totalTime(0.0f), mapID(mapID)
    {
        mapData = get_map_data(mapID);
        mapName = get_map_name(mapID);
        mapSpawn = get_map_spawn(mapID);
    }

    virtual void load() override
    {
        auto content = services->resolve<gjt::ContentManager>();
        font =
            content->loadFromFile<sf::Font>("content/monogram-extended.ttf");

        saveData = services->resolve<SaveFileManager>()->load();

        tileset = std::make_shared<gjt::Tileset>(
            content->loadFromFile<sf::Texture>("content/tiles.png"), 16, 16);

        map = std::make_shared<TileMap>(tileset, 12, 10, mapData);

        map->setPosition(0, 8);

        player = std::shared_ptr<Player>(new Player(map));
        player->setIsAnimating(false);
        player->addAnimation(
            PlayerAnimationState::Idle,
            std::make_shared<gjt::Animation>(
                std::make_shared<gjt::Tileset>(
                    content->loadFromFile<sf::Texture>("content/player.png"), 16, 16),
                1, 0.1f, true));
        player->setOrigin(8, 8);

        player->setMapPosition(mapSpawn);
        player->update(0.0f);

        energyBar = std::make_shared<EnergyBar>(tileset, &player->getHp(), 5);
        energyBar->setScale(0.5f, 0.5f);

        grassMownBar = std::make_shared<GrassMownBar>(
            tileset, &player->getGrassMown(), map->getMaxScore());
        grassMownBar->setScale(
            (float)game->getWindowWidth() /
                (grassMownBar->getPixelWidth() * TILE_SCALING),           
            0.5f);
        grassMownBar->setPosition(
            0, game->getWindowHeight() / TILE_SCALING - 8);

        game->setClearColor(sf::Color(0x5F574fff));

        totalTime = 0.0f;
        totalTimeText.setFont(*font);
        totalTimeText.setCharacterSize(36);
        totalTimeText.setStyle(sf::Text::Bold);
        
        StringFormat::formatSeconds(totalTime, totalTimeTextBuffer, 10);
        totalTimeText.setString(totalTimeTextBuffer);
        sf::FloatRect localBounds = totalTimeText.getLocalBounds();
        totalTimeText.setOrigin(0, localBounds.height / 2.0f);
        totalTimeText.setPosition(
            game->getWindowWidth() - localBounds.width - 10, energyBar->getPixelHeight() / 2.0f - 12);
        totalTimeText.setOutlineThickness(2.0f);
        totalTimeText.setOutlineColor(sf::Color::Black);
        totalTimeText.setFillColor(sf::Color(0xffa300ff));
        
        mapNameText.setFont(*font);
        mapNameText.setCharacterSize(36);
        mapNameText.setStyle(sf::Text::Bold);
        mapNameText.setString(mapName);
        localBounds = totalTimeText.getLocalBounds();
        mapNameText.setOrigin(localBounds.width / 2.0f, localBounds.height / 2.0f);
        mapNameText.setPosition(game->getWindowWidth() / 2.0f,
            energyBar->getPixelHeight() / 2.0f - 12);
        mapNameText.setOutlineThickness(2.0f);
        mapNameText.setOutlineColor(sf::Color::Black);
        mapNameText.setFillColor(sf::Color(0xffa300ff));

        resetTimer = 1.0f;
        resetTimerElapsed = resetTimer;

        enableDebug = false;
    }

    virtual void update(float dt) override
    {
        if (player->getGameState() == PlayerGameState::Mowing)
            totalTime += dt;

        StringFormat::formatSeconds(totalTime, totalTimeTextBuffer, 10);
        totalTimeText.setString(totalTimeTextBuffer);

        if (player->getHp() == 0)
        {
            startReset();
        }

        if (resetTimerElapsed < resetTimer)
        {
            updateReset(dt);
        }
        else
        {
            resetTimerElapsed = resetTimer;
            player->update(dt);
        }

        context.mapPosition = map->getPosition();
        context.mapScale = map->getScale().x;

        if (player->getGrassMown() == map->getMaxScore())
        {
            std::stack<PlayerMove> moveStack = player->getPlayerMoves();
            std::vector<PlayerMove> moves;
            moves.reserve(moveStack.size());
            while (moveStack.size() > 0)
            {
                moves.push_back(moveStack.top());
                moveStack.pop();
            }
            Score newScore = {mapID, player->getHp(), totalTime};

            game->switchState(
                std::static_pointer_cast<gjt::GameState, TransitionState>(
                    std::make_shared<TransitionState>(
                        game->getCurrentState(),
                        std::make_shared<VictoryState>(
                            mapID, moves, saveData->scores[(uint32_t)mapID],
                            newScore))));

            //game->switchState(std::make_shared<VictoryState>(
            //    mapID, moves, saveData->scores[(uint32_t)mapID], newScore));
        }
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

                context.grassMownBarPosition = grassMownBar->getPosition();
                if (ImGui::SliderFloat2("grassmownbarpos", &context.grassMownBarPosition.x, -100, 300))
                {
                    grassMownBar->setPosition(context.grassMownBarPosition);
                }

                ImGui::End();
            }
        }
    }

    virtual void draw(
        float dt, sf::RenderTarget &target,
        sf::RenderStates states = sf::RenderStates()) override
    {
        sf::View view = game->getView();
        view.setCenter(
            game->getWindowWidth() / TILE_SCALING / 2.0f,
            game->getWindowHeight() / TILE_SCALING / 2.0f);
        view.zoom(1 / TILE_SCALING);
        game->setView(view);

        target.draw(*map, states);
        target.draw(*player, states);
        target.draw(*energyBar, states);
        target.draw(*grassMownBar, states);

        game->resetView();
        target.draw(totalTimeText, states);
        target.draw(mapNameText, states);
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
            if (resetTimerElapsed != resetTimer)
                return;

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
            if (e.key.code == sf::Keyboard::Escape)
            {
                game->switchState(
                    std::static_pointer_cast<gjt::GameState, TransitionState>(
                        std::make_shared<TransitionState>(
                            game->getCurrentState(),
                            std::make_shared<MapSelectionState>(
                                (uint32_t)mapID))));

                /*game->switchState(std::static_pointer_cast<
                                  gjt::GameState, MapSelectionState>(
                    std::make_shared<MapSelectionState>((uint32_t)mapID)));*/
                return;
            }
            if (e.key.code == sf::Keyboard::R &&
                player->getGameState() == PlayerGameState::Mowing)
            {
                startReset();
            }
        }
    }

  private:
    void startReset()
    {
        totalTime = 0.0f;
        resetTimerElapsed = 0.0f;
        resetStackCount = (float)player->getPlayerMoveCount();
        player->setGameState(PlayerGameState::Preparation);
    }

    void updateReset(float dt)
    {
        resetTimerElapsed += dt;
        uint32_t current =
            (uint32_t)gjt::lerp(resetStackCount, 0, resetTimerElapsed);

        while (player->getPlayerMoveCount() > current)
        {
            player->undoMove();
        }
    }
};
