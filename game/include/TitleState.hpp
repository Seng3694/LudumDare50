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

class TitleState : public gjt::GameState
{
  public:
    std::shared_ptr<sf::Texture> splashTexture;
    std::shared_ptr<sf::Texture> frameTexture;
    std::shared_ptr<sf::Font> font;
    sf::Sprite splashSprite;
    sf::Sprite frameSprite;
    sf::Text pressStartText;
    float pulseTimer;

    virtual void load() override
    {
        auto content = services->resolve<gjt::ContentManager>();
        font =
            content->loadFromFile<sf::Font>("content/monogram-extended.ttf");

        splashTexture = content->loadFromFile<sf::Texture>("content/splash.png");
        splashTexture->setSmooth(true);
        frameTexture =
            content->loadFromFile<sf::Texture>("content/transition_banner_left.png");

        splashSprite.setTexture(*splashTexture);
        splashSprite.setOrigin(sf::Vector2f(splashTexture->getSize()) / 2.0f);
        splashSprite.setPosition(
            game->getWindowWidth() / 2.0f, game->getWindowHeight() / 2.0f);

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

        game->setClearColor(sf::Color(0x1d2b53ff));
    }

    virtual void update(float dt) override
    {
        pulseTimer += dt * 4.0f;
        const float cosinus = ((cosf(pulseTimer) + 1) / 12.0f) + 0.83333f;
        splashSprite.setScale(1 * cosinus, 1 * cosinus);
    }

    virtual void draw(
        float dt, sf::RenderTarget &target,
        sf::RenderStates states = sf::RenderStates()) override
    {
        target.draw(frameSprite, states);
        target.draw(splashSprite, states);
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
