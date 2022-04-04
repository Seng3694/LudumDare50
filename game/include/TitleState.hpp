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

class TitleState : public gjt::GameState
{
  public:
    std::shared_ptr<sf::Texture> frameTexture;
    std::shared_ptr<sf::Font> font;
    sf::Sprite frameSprite;
    sf::Text pressStartText;
    sf::Text titleText;
    float pulseTimer;
    float wiggleTimer;

    virtual void load() override
    {
        auto content = services->resolve<gjt::ContentManager>();
        font =
            content->loadFromFile<sf::Font>("content/monogram-extended.ttf");

        frameTexture =
            content->loadFromFile<sf::Texture>("content/transition_banner_left.png");

        frameSprite.setTexture(*frameTexture);
        frameSprite.setScale(4.0f, 1.0f);

        pressStartText.setFont(*font);
        pressStartText.setCharacterSize(36);
        pressStartText.setStyle(sf::Text::Bold);
        pressStartText.setString("Press anything to start");
        sf::FloatRect localBounds = pressStartText.getLocalBounds();
        pressStartText.setOrigin(
            localBounds.width / 2.0f, localBounds.height / 2.0f);
        pressStartText.setOutlineThickness(2.0f);
        pressStartText.setOutlineColor(sf::Color::Black);
        pressStartText.setFillColor(sf::Color(0xffa300ff));
        pressStartText.setPosition(
            game->getWindowWidth() / 2.0f - localBounds.left, game->getWindowHeight() - localBounds.top - 45.0f);

        titleText.setFont(*font);
        titleText.setCharacterSize(180);
        titleText.setStyle(sf::Text::Bold);
        titleText.setString("MOW BOT");
        localBounds = titleText.getLocalBounds();
        titleText.setOrigin(
            localBounds.width / 2.0f, localBounds.height / 2.0f);
        titleText.setOutlineThickness(2.0f * (180 / 36.0f));
        titleText.setOutlineColor(sf::Color::Black);
        titleText.setFillColor(sf::Color(0xffa300ff));
        titleText.setPosition(
            game->getWindowWidth() / 2.0f - localBounds.left,
            game->getWindowHeight() / 2.0f - localBounds.top - 45.0f);

        pulseTimer = 0.0f;
        wiggleTimer = 0.0f;

        game->setClearColor(sf::Color(0x1d2b53ff));
    }

    virtual void update(float dt) override
    {
        pulseTimer += dt * 4.0f;
        wiggleTimer += dt * 2.3f;
        const float cosinus = ((cosf(pulseTimer) + 1) / 12.0f) + 0.83333f;
        titleText.setScale(1 * cosinus, 1 * cosinus);
        titleText.setRotation(gjt::rad_to_deg(0.1f * sinf(wiggleTimer)));
    }

    virtual void draw(
        float dt, sf::RenderTarget &target,
        sf::RenderStates states = sf::RenderStates()) override
    {
        target.draw(frameSprite, states);
        target.draw(titleText, states);
        target.draw(pressStartText, states);
    }

    virtual void handleEvent(const sf::Event &e, float dt) override
    {
        if (e.type == sf::Event::KeyPressed)
        {
            if (e.key.code == sf::Keyboard::Key::Escape)
            {
                game->shouldClose();
                return;
            }

            game->switchState(
                std::static_pointer_cast<gjt::GameState, TransitionState>(
                    std::make_shared<TransitionState>(
                        game->getCurrentState(),
                        std::make_shared<MapSelectionState>())));
        }
    }
};
