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
#include <AudioManager.hpp>
#include "AudioFiles.hpp"

enum class TransitionStates
{
    Closing,
    Closed,
    Opening,
    Done
};

class TransitionState : public gjt::GameState
{
  public:
    std::shared_ptr<gjt::GameState> previousState;
    std::shared_ptr<gjt::GameState> nextState;
    std::shared_ptr<sf::Texture> bannerLeftTexture;
    std::shared_ptr<sf::Texture> bannerRightTexture;
    sf::Sprite bannerLeftSprite;
    sf::Sprite bannerRightSprite;
    sf::Vector2f bannerLeftStart;
    sf::Vector2f bannerLeftEnd;
    sf::Vector2f bannerRightStart;
    sf::Vector2f bannerRightEnd;
    TransitionStates state;
    float closingTimer;
    float closedTimer;
    float openingTimer;
    float closingTimerElapsed;
    float closedTimerElapsed;
    float openingTimerElapsed;

    TransitionState(
        std::shared_ptr<gjt::GameState> previous,
        std::shared_ptr<gjt::GameState> next)
        : previousState(previous), nextState(next),
          state(TransitionStates::Closing), closingTimer(0.3f),
          closedTimer(0.1f), openingTimer(0.3f), closingTimerElapsed(0.0f),
          closedTimerElapsed(0.0f), openingTimerElapsed(0.0f)
    {
    }

    virtual void load() override
    {
        auto content = services->resolve<gjt::ContentManager>();
        
        bannerLeftTexture = content->loadFromFile<sf::Texture>(
            "content/transition_banner_left.png");
        bannerRightTexture = content->loadFromFile<sf::Texture>(
            "content/transition_banner_right.png");

        bannerLeftStart = sf::Vector2f(-1.0f * (float)bannerLeftTexture->getSize().x, 0.0f);
        bannerLeftEnd = sf::Vector2f(-32.0f, 0.0f);
        bannerRightStart = sf::Vector2f(game->getWindowWidth(), 0.0f);
        bannerRightEnd = sf::Vector2f(
            game->getWindowWidth() - bannerRightTexture->getSize().x + 32.0f,
            0.0f);

        bannerLeftSprite.setTexture(*bannerLeftTexture);
        bannerLeftSprite.setPosition(bannerLeftStart);
        bannerRightSprite.setTexture(*bannerRightTexture);
        bannerRightSprite.setPosition(bannerRightStart);

        
        auto audio = services->resolve<gjt::AudioManager<AudioFiles>>();
        audio->setVolume(AudioFiles::Wush, 30.0f);
        audio->play(AudioFiles::Wush);

        nextState->game = game;
        nextState->services = services;
    }

    virtual void update(float dt) override
    {
        switch (state)
        {
        case TransitionStates::Closing: {
            closingTimerElapsed += dt;
            bannerLeftSprite.setPosition(gjt::lerp2(
                bannerLeftStart, bannerLeftEnd,
                closingTimerElapsed / closingTimer));
            bannerRightSprite.setPosition(gjt::lerp2(
                bannerRightStart, bannerRightEnd,
                closingTimerElapsed / closingTimer));

            if (closingTimerElapsed >= closingTimer)
            {
                closingTimerElapsed = 0.0f;
                state = TransitionStates::Closed;

                bannerLeftEnd = sf::Vector2f(
                    -1.0f * (float)bannerLeftTexture->getSize().x, 0.0f);
                bannerLeftStart = sf::Vector2f(-32.0f, 0.0f);

                bannerRightEnd = sf::Vector2f(game->getWindowWidth(), 0.0f);
                bannerRightStart = sf::Vector2f(
                    game->getWindowWidth() - bannerRightTexture->getSize().x +
                        32.0f,
                    0.0f);

                bannerLeftSprite.setPosition(bannerLeftStart);
                bannerRightSprite.setPosition(bannerRightStart);

                nextState->load();
            }

            break;
        }
        case TransitionStates::Closed: {

            closedTimerElapsed += dt;

            if (closedTimerElapsed >= closedTimer)
            {
                closedTimerElapsed = 0.0f;
                state = TransitionStates::Opening;

                auto audio =
                    services->resolve<gjt::AudioManager<AudioFiles>>();
                audio->setVolume(AudioFiles::Wush, 30.0f);
                audio->play(AudioFiles::Wush);
            }
            break;
        }
        case TransitionStates::Opening: {

            openingTimerElapsed += dt;

            bannerLeftSprite.setPosition(gjt::lerp2(
                bannerLeftStart, bannerLeftEnd,
                openingTimerElapsed / openingTimer));
            bannerRightSprite.setPosition(gjt::lerp2(
                bannerRightStart, bannerRightEnd,
                openingTimerElapsed / openingTimer));

            if (openingTimerElapsed >= openingTimer)
            {
                openingTimer = 0.0f;
                state = TransitionStates::Done;
                bannerLeftSprite.setPosition(bannerLeftEnd);
                bannerRightSprite.setPosition(bannerRightEnd);
            }
            break;
        }
        case TransitionStates::Done:
            previousState->unload();
            game->switchState(nextState, true, false);
            break;
        }
    }

    virtual void draw(
        float dt, sf::RenderTarget &target,
        sf::RenderStates states = sf::RenderStates()) override
    {
        switch (state)
        {
        case TransitionStates::Closing: {
            previousState->draw(dt, target, states);
            target.draw(bannerLeftSprite, states);
            target.draw(bannerRightSprite, states);
            break;
        }
        case TransitionStates::Closed: {
            target.draw(bannerLeftSprite, states);
            target.draw(bannerRightSprite, states);
            break;
        }
        case TransitionStates::Opening: {
            nextState->draw(dt, target, states);
            target.draw(bannerLeftSprite, states);
            target.draw(bannerRightSprite, states);
            break;
        case TransitionStates::Done:
            nextState->draw(dt, target, states);
            break;
        }
        }
    }
};
