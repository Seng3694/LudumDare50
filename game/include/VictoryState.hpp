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
#include "TileMap.hpp"
#include "SaveFileManager.hpp"
#include <cassert>
#include <stack>
#include "MapView.hpp"
#include "TransitionState.hpp"

#include "Maps.hpp"

class MapSelectionState;
class PlayState;

enum class VictoryStates
{
    RectangleSlide,
    NewScoreDisplay,
    AwaitingInput
};

class VictoryState : public gjt::GameState
{
  public:
    std::shared_ptr<sf::Font> font;
    std::shared_ptr<gjt::Tileset> tileset;
    std::shared_ptr<sf::Texture> bannerTexture;
    std::shared_ptr<MapView> mapView;
    std::shared_ptr<SaveData> saveData;
    std::vector<PlayerMove> moves;
    sf::Text victoryText;
    sf::Text pressToContinueText;
    sf::Text newScoreText;
    sf::Sprite newScoreBannerLeftRectangle;
    sf::Sprite newScoreBannerRightRectangle;
    VictoryStates state;
    Maps map;
    const Score oldScore;
    const Score newScore;
    float rectangleSlideTimer;
    float rectangleSlideTimerElapsed;
    float newScoreDisplayTimer;
    float newScoreDisplayTimerElapsed;
    bool hasNewScore;
    bool hasNewTime;
    bool enableDebug;

    VictoryState(const Maps map, const std::vector<PlayerMove> &moves, const Score &oldScore, const Score &newScore)
        : oldScore(oldScore), newScore(newScore), map(map), moves(moves),
          hasNewScore(false), hasNewTime(false),
          state(VictoryStates::AwaitingInput), rectangleSlideTimer(0.2f),
          rectangleSlideTimerElapsed(0.0f), newScoreDisplayTimer(0.5f),
          newScoreDisplayTimerElapsed(0.0f)
    {
    }

    virtual void load() override
    {
        auto content = services->resolve<gjt::ContentManager>();
        font =
            content->loadFromFile<sf::Font>("content/monogram-extended.ttf");

        saveData = services->resolve<SaveFileManager>()->load();

        tileset = std::make_shared<gjt::Tileset>(
            content->loadFromFile<sf::Texture>("content/tiles.png"), 16, 16);

        mapView = std::make_shared<MapView>(
            services,
            std::make_shared<TileMap>(tileset, 12, 10, get_map_data(map)),
            moves,
            newScore);
        
        mapView->setOrigin(
            mapView->getPixelWidth() / 2.0f, mapView->getPixelHeight() / 2.0f);
        mapView->setPosition(
            game->getWindowWidth() / 2.0f, game->getWindowHeight() / 2.0f);

        mapView->setFrameColor(sf::Color(0xffa300ff));

        mapView->setScale(3, 3);
        mapView->updateTextScale();

        if (newScore.value > oldScore.value)
        {
            saveData->scores[(uint32_t)map].time = newScore.time;
            saveData->scores[(uint32_t)map].value = newScore.value;
            services->resolve<SaveFileManager>()->save();
            hasNewScore = true;
            state = VictoryStates::RectangleSlide;
        }
        else if (newScore.value == oldScore.value && newScore.time < oldScore.time)
        {
            saveData->scores[(uint32_t)map].time = newScore.time;
            services->resolve<SaveFileManager>()->save();
            hasNewTime = true;
            state = VictoryStates::RectangleSlide;
        }

        victoryText.setFont(*font);
        victoryText.setCharacterSize(120);
        victoryText.setStyle(sf::Text::Bold);
        victoryText.setString("Victory!");
        sf::FloatRect localBounds = victoryText.getLocalBounds();
        victoryText.setOrigin(
            localBounds.width / 2.0f, localBounds.height / 2.0f);
        victoryText.setPosition(
            game->getWindowWidth() / 2.0f, -15.0f);
        victoryText.setOutlineThickness(2.0f * (120.0f / 36.0f));
        victoryText.setOutlineColor(sf::Color::Black);
        victoryText.setFillColor(sf::Color(0xffa300ff));

        pressToContinueText.setFont(*font);
        pressToContinueText.setCharacterSize(36);
        pressToContinueText.setStyle(sf::Text::Bold);
        pressToContinueText.setString("Press R to restart or Escape to go back");
        localBounds = pressToContinueText.getLocalBounds();
        pressToContinueText.setOrigin(
            localBounds.width / 2.0f, localBounds.height / 2.0f);
        pressToContinueText.setPosition(game->getWindowWidth() / 2.0f, game->getWindowHeight() - localBounds.height - 50.0f);
        pressToContinueText.setOutlineThickness(2.0f);
        pressToContinueText.setOutlineColor(sf::Color::Black);
        pressToContinueText.setFillColor(sf::Color(0xffa300ff));

        newScoreText.setFont(*font);
        newScoreText.setCharacterSize(120);
        newScoreText.setStyle(sf::Text::Bold);
        newScoreText.setOutlineThickness(2.0f * (120 / 36.0f));
        newScoreText.setOutlineColor(sf::Color::Black);
        if (hasNewScore)
            newScoreText.setString("New High Score");
        else if (hasNewTime)
            newScoreText.setString("New Best Time");
        localBounds = newScoreText.getLocalBounds();
        newScoreText.setPosition(
            game->getWindowWidth() / 2.0f - localBounds.left,
            game->getWindowHeight() / 2.0f - localBounds.top);
        newScoreText.setOrigin(
            localBounds.width / 2.0f, localBounds.height / 2.0f);
        newScoreText.setFillColor(sf::Color(0xffa300ff));

        bannerTexture =
            content->loadFromFile<sf::Texture>("content/scrolling_rectangle.png");
        newScoreBannerLeftRectangle.setTexture(*bannerTexture);
        newScoreBannerLeftRectangle.setPosition(
            (float)game->getWindowWidth() / -2.0f,
            game->getWindowHeight() / 2.0f - bannerTexture->getSize().y / 2.0f);

        newScoreBannerRightRectangle.setTexture(*bannerTexture);
        newScoreBannerRightRectangle.setPosition(
            (float)game->getWindowWidth(),
            game->getWindowHeight() / 2.0f -
                bannerTexture->getSize().y / 2.0f);

        enableDebug = false;
        game->setClearColor(sf::Color(0x1d2b53ff));
    }

    virtual void update(float dt) override
    {
        switch (state)
        {
        case VictoryStates::RectangleSlide: 
        {
            rectangleSlideTimerElapsed += dt;
            if (rectangleSlideTimerElapsed >= rectangleSlideTimer)
                state = VictoryStates::NewScoreDisplay;

            newScoreBannerLeftRectangle.setPosition(gjt::lerp2(
                {(float)game->getWindowWidth() / -2.0f,
                 game->getWindowHeight() / 2.0f -
                     bannerTexture->getSize().y / 2.0f},
                sf::Vector2f(
                    0, game->getWindowHeight() / 2.0f -
                           bannerTexture->getSize().y / 2.0f),
                rectangleSlideTimerElapsed / rectangleSlideTimer));

            newScoreBannerRightRectangle.setPosition(gjt::lerp2(
                {(float)game->getWindowWidth(),
                 game->getWindowHeight() / 2.0f -
                     bannerTexture->getSize().y / 2.0f},
                sf::Vector2f(
                    game->getWindowWidth() / 2.0f,
                    game->getWindowHeight() / 2.0f -
                        bannerTexture->getSize().y / 2.0f),
                rectangleSlideTimerElapsed / rectangleSlideTimer));

            break;
        }
        case VictoryStates::NewScoreDisplay: 
        {
            newScoreDisplayTimerElapsed += dt;
            if (newScoreDisplayTimerElapsed >= newScoreDisplayTimer)
                state = VictoryStates::AwaitingInput;

            newScoreBannerLeftRectangle.setPosition(sf::Vector2f(
                0, game->getWindowHeight() / 2.0f -
                       bannerTexture->getSize().y / 2.0f));

            newScoreBannerRightRectangle.setPosition(sf::Vector2f(
                game->getWindowWidth() / 2.0f,
                game->getWindowHeight() / 2.0f -
                    bannerTexture->getSize().y / 2.0f));

            break;
        }
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

                ImGui::End();
            }
        }
    }

    virtual void draw(
        float dt, sf::RenderTarget &target,
        sf::RenderStates states = sf::RenderStates()) override
    {
        target.draw(*mapView, states);
        target.draw(victoryText, states);
        target.draw(pressToContinueText, states);

        switch (state)
        {
        case VictoryStates::RectangleSlide:
        case VictoryStates::NewScoreDisplay: 
        {
            sf::RectangleShape shape;
            shape.setSize(
                sf::Vector2f(game->getWindowWidth(), game->getWindowHeight()));
            shape.setFillColor(sf::Color(0x000000c0));
            shape.setPosition(0, 0);

            target.draw(shape, states);
            target.draw(newScoreBannerLeftRectangle, states);
            target.draw(newScoreBannerRightRectangle, states);
            target.draw(newScoreText, states);
            break;
        }
        }
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

            if ((state == VictoryStates::RectangleSlide || state == VictoryStates::NewScoreDisplay) &&
                e.key.code == sf::Keyboard::Key::Enter)
            {
                state = VictoryStates::AwaitingInput;
                return;
            }

            if (e.key.code == sf::Keyboard::Key::Enter || e.key.code == sf::Keyboard::Key::Space || e.key.code == sf::Keyboard::Escape)
            {
                game->switchState(
                    std::static_pointer_cast<gjt::GameState, TransitionState>(
                        std::make_shared<TransitionState>(
                            game->getCurrentState(),
                            std::make_shared<MapSelectionState>(
                                (uint32_t)map))));

                //game->switchState(
                //    std::static_pointer_cast<gjt::GameState, MapSelectionState>(std::make_shared<MapSelectionState>((uint32_t)map)));
                return;
            }
            if (e.key.code == sf::Keyboard::Key::R)
            {
                game->switchState(
                    std::static_pointer_cast<gjt::GameState, TransitionState>(
                        std::make_shared<TransitionState>(
                            game->getCurrentState(),
                            std::make_shared<PlayState>(map))));

                /*game->switchState(std::static_pointer_cast<gjt::GameState, PlayState>(
                    std::make_shared<PlayState>(map)));*/
                return;
            }
        }
    }
};
