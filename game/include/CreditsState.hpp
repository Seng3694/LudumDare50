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
#include "TitleState.hpp"

enum class CreditsStates
{
    Show,
    Fade,
    Wait,
    Done
};

class CreditsState : public gjt::GameState
{
  public:
    std::shared_ptr<sf::Font> font;
    sf::Sprite frameSprite;
    sf::Sprite sfmlSprite;
    
    sf::Text gameCreatedByText;
    sf::Text creatorText;
    sf::Text gameTestedByText;
    sf::Text testerText;
    sf::Text technologyText;

    sf::Color headerTextColor;
    sf::Color textColor;

    CreditsStates state;
    float showTimer;
    float showTimerElapsed;
    float fadeTimer;
    float fadeTimerElapsed;
    float waitTimer;
    float waitTimerElapsed;

    virtual void load() override
    {
        auto content = services->resolve<gjt::ContentManager>();
        font =
            content->loadFromFile<sf::Font>("content/monogram-extended.ttf");

        headerTextColor = sf::Color(0xab5236ff);
        textColor = sf::Color(0xffa300ff);

        state = CreditsStates::Show;
        showTimer = 2.0f;
        showTimerElapsed = 0.0f;

        fadeTimer = 1.0f;
        fadeTimerElapsed = 0.0f;

        waitTimer = 0.2f;
        waitTimerElapsed = 0.0f;

        gameCreatedByText.setFont(*font);
        gameCreatedByText.setCharacterSize(50);
        gameCreatedByText.setStyle(sf::Text::Bold);
        gameCreatedByText.setString("Game created by");
        gameCreatedByText.setOutlineThickness(2.0f * (50.0f / 34.0f));
        gameCreatedByText.setOutlineColor(sf::Color::Black);
        gameCreatedByText.setFillColor(headerTextColor);
        gameCreatedByText.setPosition(50.0f, 20.0f);

        creatorText.setFont(*font);
        creatorText.setCharacterSize(50.0f);
        creatorText.setStyle(sf::Text::Bold);
        creatorText.setString("Seng");
        creatorText.setOutlineThickness(2.0f * (50.0f / 34.0f));
        creatorText.setOutlineColor(sf::Color::Black);
        creatorText.setFillColor(textColor);
        creatorText.setPosition(150.0f, 70.0f);

        gameTestedByText.setFont(*font);
        gameTestedByText.setCharacterSize(50.0f);
        gameTestedByText.setStyle(sf::Text::Bold);
        gameTestedByText.setString("Game tested by");
        gameTestedByText.setOutlineThickness(2.0f * (50.0f / 34.0f));
        gameTestedByText.setOutlineColor(sf::Color::Black);
        gameTestedByText.setFillColor(headerTextColor);
        gameTestedByText.setPosition(50.0f, 180.0f);

        testerText.setFont(*font);
        testerText.setCharacterSize(50.0f);
        testerText.setStyle(sf::Text::Bold);
        testerText.setString("Manitary\nSteamGear\nFinnhax");
        testerText.setOutlineThickness(2.0f * (50.0f / 34.0f));
        testerText.setOutlineColor(sf::Color::Black);
        testerText.setFillColor(textColor);
        testerText.setPosition(150.0f, 230.0f);

        technologyText.setFont(*font);
        technologyText.setCharacterSize(50.0f);
        technologyText.setStyle(sf::Text::Bold);
        technologyText.setString("Technology used");
        technologyText.setOutlineThickness(2.0f * (50.0f / 34.0f));
        technologyText.setOutlineColor(sf::Color::Black);
        technologyText.setFillColor(headerTextColor);
        technologyText.setPosition(50.0f, 420.0f);

        auto sfmlTexture =
            content->loadFromFile<sf::Texture>("content/sfml-logo-small.png");
        sfmlSprite.setTexture(*sfmlTexture);
        sfmlSprite.setScale(0.5f, 0.5f);
        sfmlSprite.setOrigin(sf::Vector2f(sfmlTexture->getSize()) / 2.0f);
        sfmlSprite.setPosition(
            150.0f + (sfmlTexture->getSize().x / 2.0f) * 0.5f,
            500.0f + (sfmlTexture->getSize().y / 2.0f) * 0.5f);

        frameSprite.setTexture(*content->loadFromFile<sf::Texture>(
            "content/transition_banner_left.png"));
        frameSprite.setScale(4.0f, 1.0f);

        game->setClearColor(sf::Color(0x1d2b53ff));
    }

    virtual void update(float dt) override
    {
        switch (state)
        {
        case CreditsStates::Show:
            showTimerElapsed += dt;
            if (showTimerElapsed >= showTimer)
            {
                state = CreditsStates::Fade;
            }
            break;
        case CreditsStates::Fade: {
            fadeTimerElapsed += dt;

            const uint8_t alpha =
                (uint8_t)(255.0f * (1 - (fadeTimerElapsed / fadeTimer)));
            
            headerTextColor.a = alpha;
            textColor.a = alpha;

            sfmlSprite.setColor(sf::Color(0xff, 0xff, 0xff, alpha));

            gameCreatedByText.setFillColor(headerTextColor);
            gameCreatedByText.setOutlineColor(
                sf::Color(0x00, 0x00, 0x00, alpha));

            creatorText.setFillColor(textColor);
            creatorText.setOutlineColor(
                sf::Color(0x00, 0x00, 0x00, alpha));

            gameTestedByText.setFillColor(headerTextColor);
            gameTestedByText.setOutlineColor(
                sf::Color(0x00, 0x00, 0x00, alpha));

            testerText.setFillColor(textColor);
            testerText.setOutlineColor(
                sf::Color(0x00, 0x00, 0x00, alpha));

            technologyText.setFillColor(headerTextColor);
            technologyText.setOutlineColor(
                sf::Color(0x00, 0x00, 0x00, alpha));

            if (fadeTimerElapsed >= fadeTimer)
            {
                state = CreditsStates::Wait;
            }
            break;
        }
        case CreditsStates::Wait: 
            waitTimerElapsed += dt;
            if (waitTimerElapsed >= waitTimer)
                state = CreditsStates::Done;
            break;
        case CreditsStates::Done: {
            game->switchState(std::static_pointer_cast<gjt::GameState, TitleState>(
                    std::make_shared<TitleState>()));
            break;
        }
        }
    }

    virtual void draw(
        float dt, sf::RenderTarget &target,
        sf::RenderStates states = sf::RenderStates()) override
    {
        target.draw(frameSprite, states);
        switch (state)
        {
        case CreditsStates::Show:
        case CreditsStates::Fade:
            target.draw(gameCreatedByText, states);
            target.draw(creatorText, states);
            target.draw(gameTestedByText, states);
            target.draw(testerText, states);
            target.draw(technologyText, states);
            target.draw(sfmlSprite, states);
            break;
        }
    }

    virtual void handleEvent(const sf::Event &e, float dt) override
    {
        if (e.type == sf::Event::KeyPressed)
        {
            state = CreditsStates::Done;
        }
    }
};
