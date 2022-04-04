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
#include "CreditsState.hpp"

class TitleState : public gjt::GameState
{
  public:
    std::shared_ptr<sf::Texture> frameTexture;
    std::shared_ptr<sf::Font> font;
    sf::Sprite frameSprite;
    sf::Text startLabelText;
    sf::Text creditsLabelText;
    sf::Text exitLabelText;
    sf::Text titleText;
    sf::Color labelActive;
    sf::Color labelInactive;
    float pulseTimer;
    float wiggleTimer;
    int32_t labelIndex;

    virtual void load() override
    {
        auto content = services->resolve<gjt::ContentManager>();
        font =
            content->loadFromFile<sf::Font>("content/monogram-extended.ttf");

        frameTexture =
            content->loadFromFile<sf::Texture>("content/transition_banner_left.png");

        frameSprite.setTexture(*frameTexture);
        frameSprite.setScale(4.0f, 1.0f);

        labelIndex = 0;
        labelActive = sf::Color(0xffa300ff);
        labelInactive = sf::Color(0x5f574fff);

        startLabelText.setFont(*font);
        startLabelText.setCharacterSize(50.0f);
        startLabelText.setStyle(sf::Text::Bold);
        startLabelText.setString("Start");
        sf::FloatRect localBounds = startLabelText.getLocalBounds();
        startLabelText.setOrigin(
            localBounds.width / 2.0f, localBounds.height / 2.0f);
        startLabelText.setOutlineThickness(2.0f * (50.0f / 36.0f));
        startLabelText.setOutlineColor(sf::Color::Black);
        startLabelText.setFillColor(labelActive);
        startLabelText.setPosition(
            game->getWindowWidth() / 2.0f - localBounds.left, 
            game->getWindowHeight() / 2.0f - localBounds.top + 40.0f);

        creditsLabelText.setFont(*font);
        creditsLabelText.setCharacterSize(50.0f);
        creditsLabelText.setStyle(sf::Text::Bold);
        creditsLabelText.setString("Credits");
        localBounds = creditsLabelText.getLocalBounds();
        creditsLabelText.setOrigin(
            localBounds.width / 2.0f, localBounds.height / 2.0f);
        creditsLabelText.setOutlineThickness(2.0f * (50.0f / 36.0f));
        creditsLabelText.setOutlineColor(sf::Color::Black);
        creditsLabelText.setFillColor(labelInactive);
        creditsLabelText.setPosition(
            game->getWindowWidth() / 2.0f - localBounds.left,
            game->getWindowHeight() / 2.0f - localBounds.top + 100.0f);

        exitLabelText.setFont(*font);
        exitLabelText.setCharacterSize(50.0f);
        exitLabelText.setStyle(sf::Text::Bold);
        exitLabelText.setString("Exit");
        localBounds = exitLabelText.getLocalBounds();
        exitLabelText.setOrigin(
            localBounds.width / 2.0f, localBounds.height / 2.0f);
        exitLabelText.setOutlineThickness(2.0f * (50.0f / 36.0f));
        exitLabelText.setOutlineColor(sf::Color::Black);
        exitLabelText.setFillColor(labelInactive);
        exitLabelText.setPosition(
            game->getWindowWidth() / 2.0f - localBounds.left,
            game->getWindowHeight() / 2.0f - localBounds.top + 160.0f);

        titleText.setFont(*font);
        titleText.setCharacterSize(240);
        titleText.setStyle(sf::Text::Bold);
        titleText.setString("MOW BOT");
        localBounds = titleText.getLocalBounds();
        titleText.setOrigin(
            localBounds.width / 2.0f, localBounds.height / 2.0f);
        titleText.setOutlineThickness(2.0f * (240.0f / 36.0f));
        titleText.setOutlineColor(sf::Color::Black);
        titleText.setFillColor(sf::Color(0xffa300ff));
        titleText.setPosition(
            game->getWindowWidth() / 2.0f - localBounds.left,
            game->getWindowHeight() / 2.0f - localBounds.top - 140.0f);

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
        target.draw(startLabelText, states);
        target.draw(creditsLabelText, states);
        target.draw(exitLabelText, states);
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

            int32_t index = labelIndex;
            if (e.key.code == sf::Keyboard::Key::W ||
                e.key.code == sf::Keyboard::Key::Up)
            {
                labelIndex = gjt::wrap<int32_t>(labelIndex - 1, 0, 2);
            }
            if (e.key.code == sf::Keyboard::Key::S ||
                e.key.code == sf::Keyboard::Key::Down)
            {
                labelIndex = gjt::wrap<int32_t>(labelIndex + 1, 0, 2);
            }

            if (index != labelIndex)
            {
                switch (labelIndex)
                {
                case 0:
                    startLabelText.setColor(labelActive);
                    creditsLabelText.setColor(labelInactive);
                    exitLabelText.setColor(labelInactive);
                    break;
                case 1:
                    startLabelText.setColor(labelInactive);
                    creditsLabelText.setColor(labelActive);
                    exitLabelText.setColor(labelInactive);
                    break;
                case 2:
                    startLabelText.setColor(labelInactive);
                    creditsLabelText.setColor(labelInactive);
                    exitLabelText.setColor(labelActive);
                    break;
                }
                auto audio =
                    services->resolve<gjt::AudioManager<AudioFiles>>();
                audio->setVolume(AudioFiles::Select, 50.0f);
                audio->play(AudioFiles::Select);
            }

            

            if (e.key.code == sf::Keyboard::Enter)
            {
                switch (labelIndex)
                {
                case 0:
                    game->switchState(std::static_pointer_cast<
                                      gjt::GameState, TransitionState>(
                        std::make_shared<TransitionState>(
                            game->getCurrentState(),
                            std::make_shared<MapSelectionState>())));
                    return;
                case 1:
                    game->switchState(std::static_pointer_cast<
                                      gjt::GameState, TransitionState>(
                        std::make_shared<TransitionState>(
                            game->getCurrentState(),
                            std::make_shared<CreditsState>())));
                    return;
                case 2:
                    game->shouldClose();
                    return;
                }
                
                return;
            }

            
        }
    }
};
