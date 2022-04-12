#pragma once

#include <SFML/Graphics.hpp>
#include <cstdio>

#include "Animation.hpp"
#include "AudioManager.hpp"
#include "Content.hpp"
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
#include <functional>
#include <deque>

class MapSelectionState;

#define STRINGIFY(x) (#x)

#define TILE_SCALING (4.0f)

enum class MownGrassAnimation
{
    Default,
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
    std::shared_ptr<gjt::Tileset> grassTiles;
    std::shared_ptr<TileMap> map;
    std::shared_ptr<Player> player;
    std::shared_ptr<EnergyBar> energyBar;
    std::shared_ptr<GrassMownBar> grassMownBar;
    std::shared_ptr<SaveData> saveData;
    std::deque<gjt::AnimatedSprite<MownGrassAnimation>> animations;
    sf::Text totalTimeText;
    sf::Text mapNameText;
    char totalTimeTextBuffer[10];
    float totalTime;
    float resetTimer;
    float resetTimerElapsed;
    float resetStackCount;
    const uint8_t *mapData;
    sf::String mapName;
    sf::Vector2u mapSpawn;
    const Maps mapID;

  public:
    PlayState(const Maps mapID)
        : font(nullptr), map(nullptr), player(nullptr), saveData(nullptr), totalTime(0.0f), mapID(mapID)
    {
        mapData = get_map_data(mapID);
        mapName = get_map_name(mapID);
        mapSpawn = get_map_spawn(mapID);
    }

    virtual void load() override
    {
        auto content = services->resolve<GameContentManager>();
        font = content->get<sf::Font>(Content::MonogramFont);

        saveData = services->resolve<SaveFileManager>()->load();

        tileset = content->get<gjt::Tileset>(Content::MapTilesTileset);

        grassTiles = content->get<gjt::Tileset>(Content::MownGrassTileset);

        map = std::make_shared<TileMap>(tileset, 12, 10, mapData);

        map->setPosition(0, 8);

        player = std::shared_ptr<Player>(new Player(map));
        player->setIsAnimating(false);
        player->addAnimation(
            PlayerAnimationState::Idle,
            std::make_shared<gjt::Animation>(
                content->get<gjt::Tileset>(Content::MowBotTileset),
                1, 0.1f, true));
        player->setOrigin(8, 8);

        player->setGrassMownCallback([&](uint32_t x, uint32_t y) {
            auto audio = services->resolve<gjt::AudioManager<AudioFiles>>();
            audio->setVolume(AudioFiles::Mow, 20.0f);
            audio->play(AudioFiles::Mow);

            auto sprite = gjt::AnimatedSprite<MownGrassAnimation>();
            auto animation =
                std::make_shared<gjt::Animation>(grassTiles, 5, 0.8f);
            animation->start();
            sprite.addAnimation(
                MownGrassAnimation::Default,
                animation);
            sprite.setOrigin(16, 16);

            sf::Vector2f origin = sprite.getOrigin();
            sprite.setPosition(
                map->getTransform().transformPoint(
                x * 16 + origin.x / 2.0f, y * 16 + origin.y / 2.0f));
            animations.push_back(sprite);
        });

        player->setPlayerHitCallback([&](void) {
            auto audio = services->resolve<gjt::AudioManager<AudioFiles>>();
            audio->setVolume(AudioFiles::Hit, 50.0f);
            audio->play(AudioFiles::Hit);
        });

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
    }

    virtual void update(float dt) override
    {
        if (player->getGameState() == PlayerGameState::Mowing)
            totalTime += dt;

        StringFormat::formatSeconds(totalTime, totalTimeTextBuffer, 10);
        totalTimeText.setString(totalTimeTextBuffer);

        while (animations.size() > 0 
            && animations[0].getCurrentAnimation()->getAnimationState() != gjt::AnimationState::Running)
        {
            animations.pop_front();
        }

        for (auto &anim : animations)
            anim.update(dt);

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

        for (auto &anim : animations)
            target.draw(anim, states);

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
                auto audio =
                    services->resolve<gjt::AudioManager<AudioFiles>>();
                audio->stop(AudioFiles::Playstate);
                audio->queue(AudioFiles::Menu, true);
                audio->play(AudioFiles::Menu);

                game->switchState(
                    std::static_pointer_cast<gjt::GameState, TransitionState>(
                        std::make_shared<TransitionState>(
                            game->getCurrentState(),
                            std::make_shared<MapSelectionState>(
                                (uint32_t)mapID))));

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
