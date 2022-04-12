#pragma once

#include <SFML/Graphics.hpp>
#include <cstdio>

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
#include <AudioManager.hpp>
#include "AudioFiles.hpp"

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
    sf::RenderTexture renderTexture;
    sf::Sprite renderTextureSprite;
    std::shared_ptr<sf::Font> font;
    std::shared_ptr<gjt::Tileset> tileset;
    std::shared_ptr<sf::Texture> bannerTexture;
    std::shared_ptr<MapView> mapView;
    std::shared_ptr<SaveData> saveData;
    std::vector<PlayerMove> moves;
    sf::Text victoryText;
    sf::Text newScoreText;
    sf::Text retryLabelText;
    sf::Text exitLabelText;
    sf::Sprite newScoreBannerLeftRectangle;
    sf::Sprite newScoreBannerRightRectangle;
    sf::Color labelActive;
    sf::Color labelInactive;
    VictoryStates state;
    Maps map;
    const Score oldScore;
    const Score newScore;
    float rectangleSlideTimer;
    float rectangleSlideTimerElapsed;
    float newScoreDisplayTimer;
    float newScoreDisplayTimerElapsed;
    int32_t labelIndex;
    bool hasNewScore;
    bool hasNewTime;

    VictoryState(
        const Maps map, const std::vector<PlayerMove> &moves,
        const Score &oldScore, const Score &newScore)
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
            moves, newScore);

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
        else if (
            newScore.value == oldScore.value && newScore.time < oldScore.time)
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
        victoryText.setPosition(game->getWindowWidth() / 2.0f, -15.0f);
        victoryText.setOutlineThickness(2.0f * (120.0f / 36.0f));
        victoryText.setOutlineColor(sf::Color::Black);
        victoryText.setFillColor(sf::Color(0xffa300ff));

        labelIndex = 1;
        labelActive = sf::Color(0xffa300ff);
        labelInactive = sf::Color(0x5f574fff);

        retryLabelText.setFont(*font);
        retryLabelText.setCharacterSize(50.0f);
        retryLabelText.setStyle(sf::Text::Bold);
        retryLabelText.setString("Retry");
        localBounds = retryLabelText.getLocalBounds();
        retryLabelText.setOrigin(
            localBounds.width / 2.0f, localBounds.height / 2.0f);
        retryLabelText.setPosition(
            game->getWindowWidth() / 2.0f + localBounds.left - 180.0f,
            game->getWindowHeight() + localBounds.top - 110.0f);
        retryLabelText.setOutlineThickness(2.0f * (50.0f / 36.0f));
        retryLabelText.setOutlineColor(sf::Color::Black);
        retryLabelText.setFillColor(labelInactive);

        exitLabelText.setFont(*font);
        exitLabelText.setCharacterSize(50.0f);
        exitLabelText.setStyle(sf::Text::Bold);
        exitLabelText.setString("Exit");
        localBounds = exitLabelText.getLocalBounds();
        exitLabelText.setOrigin(
            localBounds.width / 2.0f, localBounds.height / 2.0f);
        exitLabelText.setPosition(
            game->getWindowWidth() / 2.0f + localBounds.left + 180.0f,
            game->getWindowHeight() + localBounds.top - 110.0f);
        exitLabelText.setOutlineThickness(2.0f * (50.0f / 36.0f));
        exitLabelText.setOutlineColor(sf::Color::Black);
        exitLabelText.setFillColor(labelActive);

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

        bannerTexture = content->loadFromFile<sf::Texture>(
            "content/scrolling_rectangle.png");
        newScoreBannerLeftRectangle.setTexture(*bannerTexture);
        newScoreBannerLeftRectangle.setPosition(
            (float)game->getWindowWidth() / -2.0f,
            game->getWindowHeight() / 2.0f -
                bannerTexture->getSize().y / 2.0f);

        newScoreBannerRightRectangle.setTexture(*bannerTexture);
        newScoreBannerRightRectangle.setPosition(
            (float)game->getWindowWidth(),
            game->getWindowHeight() / 2.0f -
                bannerTexture->getSize().y / 2.0f);

        renderTexture.create(game->getWindowWidth(), game->getWindowHeight());
        renderTexture.clear(sf::Color(0x1d2b53ff));
        renderTexture.draw(*mapView);
        renderTexture.draw(victoryText);
        renderTexture.display();
        renderTextureSprite.setTexture(renderTexture.getTexture());

        game->setClearColor(sf::Color(0x1d2b53ff));
    }

    virtual void update(float dt) override
    {
        switch (state)
        {
        case VictoryStates::RectangleSlide: {
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
        case VictoryStates::NewScoreDisplay: {
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

    virtual void draw(
        float dt, sf::RenderTarget &target,
        sf::RenderStates states = sf::RenderStates()) override
    {
        target.draw(renderTextureSprite, states);
        target.draw(retryLabelText, states);
        target.draw(exitLabelText, states);

        switch (state)
        {
        case VictoryStates::RectangleSlide:
        case VictoryStates::NewScoreDisplay: {
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
            if ((state == VictoryStates::RectangleSlide ||
                 state == VictoryStates::NewScoreDisplay) &&
                e.key.code == sf::Keyboard::Key::Enter)
            {
                state = VictoryStates::AwaitingInput;
                return;
            }

            int32_t index = labelIndex;
            if (e.key.code == sf::Keyboard::Key::A ||
                e.key.code == sf::Keyboard::Key::Left)
            {
                labelIndex = gjt::wrap<int32_t>(labelIndex - 1, 0, 1);
            }
            if (e.key.code == sf::Keyboard::Key::D ||
                e.key.code == sf::Keyboard::Key::Right)
            {
                labelIndex = gjt::wrap<int32_t>(labelIndex + 1, 0, 1);
            }

            if (index != labelIndex)
            {
                switch (labelIndex)
                {
                case 0:
                    retryLabelText.setColor(labelActive);
                    exitLabelText.setColor(labelInactive);
                    break;
                case 1:
                    retryLabelText.setColor(labelInactive);
                    exitLabelText.setColor(labelActive);
                    break;
                }
                auto audio =
                    services->resolve<gjt::AudioManager<AudioFiles>>();
                audio->setVolume(AudioFiles::Select, 50.0f);
                audio->play(AudioFiles::Select);
            }

            bool gotoMapSelection = false;
            bool gotoPlayState = false;
            if (e.key.code == sf::Keyboard::Enter)
            {
                switch (labelIndex)
                {
                case 0:
                    gotoPlayState = true;
                    break;
                case 1:
                    gotoMapSelection = true;
                    break;
                }
            }

            if (e.key.code == sf::Keyboard::Escape || gotoMapSelection)
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
                                (uint32_t)map))));

                return;
            }
            if (e.key.code == sf::Keyboard::Key::R || gotoPlayState)
            {
                game->switchState(
                    std::static_pointer_cast<gjt::GameState, TransitionState>(
                        std::make_shared<TransitionState>(
                            game->getCurrentState(),
                            std::make_shared<PlayState>(map))));
                return;
            }

        }
    }
};
