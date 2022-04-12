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
#include "TileMap.hpp"
#include "SaveFileManager.hpp"
#include <cassert>
#include <stack>
#include "MapView.hpp"
#include <AudioManager.hpp>
#include "AudioFiles.hpp"
#include "TitleState.hpp"

class CreditsState : public gjt::GameState
{
  public:
    std::shared_ptr<sf::Font> font;
    sf::Sprite frameSprite;
    sf::Sprite sfmlSprite;
    
    sf::Text gameCreatedByText;
    sf::Text creatorText;
    sf::Text gameTestedByText;
    sf::Text tester1Text;
    sf::Text tester2Text;
    sf::Text tester3Text;
    sf::Text technologyText;

    sf::Color headerTextColor;
    sf::Color textColor;

    virtual void load() override
    {
        auto content = services->resolve<GameContentManager>();
        font =
            content->get<sf::Font>(Content::MonogramFont);

        headerTextColor = sf::Color(0xab5236ff);
        textColor = sf::Color(0xffa300ff);

        gameCreatedByText.setFont(*font);
        gameCreatedByText.setCharacterSize(50);
        gameCreatedByText.setStyle(sf::Text::Bold);
        gameCreatedByText.setString("Game created by");
        gameCreatedByText.setOutlineThickness(2.0f * (50.0f / 34.0f));
        gameCreatedByText.setOutlineColor(sf::Color::Black);
        gameCreatedByText.setFillColor(headerTextColor);
        sf::FloatRect localBounds = gameCreatedByText.getLocalBounds();
        gameCreatedByText.setOrigin(
            localBounds.width / 2.0f, localBounds.height / 2.0f);
        gameCreatedByText.setPosition(
            game->getWindowWidth() / 2.0f + localBounds.left, 
            localBounds.top + 20.0f);


        creatorText.setFont(*font);
        creatorText.setCharacterSize(50.0f);
        creatorText.setStyle(sf::Text::Bold);
        creatorText.setString("Seng");
        creatorText.setOutlineThickness(2.0f * (50.0f / 34.0f));
        creatorText.setOutlineColor(sf::Color::Black);
        creatorText.setFillColor(textColor);
        localBounds = creatorText.getLocalBounds();
        creatorText.setOrigin(
            localBounds.width / 2.0f, localBounds.height / 2.0f);
        creatorText.setPosition(
            game->getWindowWidth() / 2.0f + localBounds.left,
            localBounds.top + 70.0f);

        gameTestedByText.setFont(*font);
        gameTestedByText.setCharacterSize(50.0f);
        gameTestedByText.setStyle(sf::Text::Bold);
        gameTestedByText.setString("Game tested by");
        gameTestedByText.setOutlineThickness(2.0f * (50.0f / 34.0f));
        gameTestedByText.setOutlineColor(sf::Color::Black);
        gameTestedByText.setFillColor(headerTextColor);
        localBounds = gameTestedByText.getLocalBounds();
        gameTestedByText.setOrigin(
            localBounds.width / 2.0f, localBounds.height / 2.0f);
        gameTestedByText.setPosition(
            game->getWindowWidth() / 2.0f + localBounds.left,
            localBounds.top + 180.0f);

        tester1Text.setFont(*font);
        tester1Text.setCharacterSize(50.0f);
        tester1Text.setStyle(sf::Text::Bold);
        tester1Text.setString("Manitary");
        tester1Text.setOutlineThickness(2.0f * (50.0f / 34.0f));
        tester1Text.setOutlineColor(sf::Color::Black);
        tester1Text.setFillColor(textColor);
        localBounds = tester1Text.getLocalBounds();
        tester1Text.setOrigin(
            localBounds.width / 2.0f, localBounds.height / 2.0f);
        tester1Text.setPosition(
            game->getWindowWidth() / 2.0f + localBounds.left,
            localBounds.top + 230.0f);

        tester2Text.setFont(*font);
        tester2Text.setCharacterSize(50.0f);
        tester2Text.setStyle(sf::Text::Bold);
        tester2Text.setString("SteamGear");
        tester2Text.setOutlineThickness(2.0f * (50.0f / 34.0f));
        tester2Text.setOutlineColor(sf::Color::Black);
        tester2Text.setFillColor(textColor);
        localBounds = tester2Text.getLocalBounds();
        tester2Text.setOrigin(
            localBounds.width / 2.0f, localBounds.height / 2.0f);
        tester2Text.setPosition(
            game->getWindowWidth() / 2.0f + localBounds.left,
            localBounds.top + 280.0f);

        tester3Text.setFont(*font);
        tester3Text.setCharacterSize(50.0f);
        tester3Text.setStyle(sf::Text::Bold);
        tester3Text.setString("Finnhax");
        tester3Text.setOutlineThickness(2.0f * (50.0f / 34.0f));
        tester3Text.setOutlineColor(sf::Color::Black);
        tester3Text.setFillColor(textColor);
        localBounds = tester3Text.getLocalBounds();
        tester3Text.setOrigin(
            localBounds.width / 2.0f, localBounds.height / 2.0f);
        tester3Text.setPosition(
            game->getWindowWidth() / 2.0f + localBounds.left,
            localBounds.top + 330.0f);

        technologyText.setFont(*font);
        technologyText.setCharacterSize(50.0f);
        technologyText.setStyle(sf::Text::Bold);
        technologyText.setString("Technology used");
        technologyText.setOutlineThickness(2.0f * (50.0f / 34.0f));
        technologyText.setOutlineColor(sf::Color::Black);
        technologyText.setFillColor(headerTextColor);
        localBounds = technologyText.getLocalBounds();
        technologyText.setOrigin(
            localBounds.width / 2.0f, localBounds.height / 2.0f);
        technologyText.setPosition(
            game->getWindowWidth() / 2.0f + localBounds.left,
            localBounds.top + 440.0f);

        auto sfmlTexture =
            content->get<sf::Texture>(Content::SFMLTexture);
        sfmlSprite.setTexture(*sfmlTexture);
        sfmlSprite.setScale(0.5f, 0.5f);
        sfmlSprite.setOrigin(sf::Vector2f(sfmlTexture->getSize()) / 2.0f);
        sfmlSprite.setPosition(
            game->getWindowWidth() / 2.0f,
            (sfmlTexture->getSize().y / 2.0f) * 0.5f + 530.0f);

        frameSprite.setTexture(
            *content->get<sf::Texture>(
            Content::TransitionBannerLeftTexture));
        frameSprite.setScale(4.0f, 1.0f);

        game->setClearColor(sf::Color(0x1d2b53ff));
    }

    virtual void update(float dt) override
    {
        
    }

    virtual void draw(
        float dt, sf::RenderTarget &target,
        sf::RenderStates states = sf::RenderStates()) override
    {
        target.draw(frameSprite, states);
        target.draw(gameCreatedByText, states);
        target.draw(creatorText, states);
        target.draw(gameTestedByText, states);
        target.draw(tester1Text, states);
        target.draw(tester2Text, states);
        target.draw(tester3Text, states);
        target.draw(technologyText, states);
        target.draw(sfmlSprite, states);
    }

    virtual void handleEvent(const sf::Event &e, float dt) override
    {
        if (e.type == sf::Event::KeyPressed)
        {
            game->switchState(
                std::static_pointer_cast<gjt::GameState, TransitionState>(
                    std::make_shared<TransitionState>(
                        game->getCurrentState(),
                        std::make_shared<TitleState>())));
        }
    }
};
