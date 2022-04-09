#pragma once

#include "Game.hpp"
#include "PlayState.hpp"
#include "MapSelectionState.hpp"
#include "SaveFileManager.hpp"
#include "TitleState.hpp"
#include "AudioFiles.hpp"
#include <AudioManager.hpp>
#include "VolumeBar.hpp"
#include <memory>

class LDGame : public gjt::Game
{
  public:
    LDGame(
        const std::string &title, const uint32_t width, const uint32_t height)
        : gjt::Game(title, width, height)
    {
    }

  protected:
    std::shared_ptr<VolumeBar> volumeBar;

    virtual void update(float dt) override
    {  
        gjt::Game::update(dt);

        auto audio = services.resolve<gjt::AudioManager<AudioFiles>>();
        audio->update();

        volumeBar->update(dt);
    }

    virtual void draw(float dt, sf::RenderTarget& target, sf::RenderStates states) override
    {
        gjt::Game::draw(dt, target, states);
        target.draw(*volumeBar, states);
    }

    virtual void load() override
    {
        gjt::Game::load();
        
        window.setKeyRepeatEnabled(false);

        services.registerInstance<SaveFileManager>(
            std::make_shared<SaveFileManager>("save.bin"));
        services.registerInstance<gjt::AudioManager<AudioFiles>>();

        auto content = services.resolve<gjt::ContentManager>();
        content->loadFromFile<sf::Font>("content/monogram-extended.ttf");
        content->loadFromFile<sf::Texture>("content/player.png");
        content->loadFromFile<sf::Texture>("content/map_frame.png");
        content->loadFromFile<sf::Texture>("content/frame_glow.png");
        content->loadFromFile<sf::Texture>("content/scrolling_rectangle.png");
        content->loadFromFile<sf::Texture>("content/tiles.png");
        content->loadFromFile<sf::Texture>("content/mown_grass.png");
        content->loadFromFile<sf::Texture>("content/transition_banner_left.png");
        content->loadFromFile<sf::Texture>("content/transition_banner_right.png");
        content->loadFromFile<sf::Texture>("content/volumebar.png");
        content->loadFromFile<sf::Texture>("content/volumebar_part.png");
        content->loadFromFile<sf::Texture>("content/sfml-logo-small.png");
        setWindowIcon(content->loadFromFile<sf::Image>("content/mowbot.png"));

        auto audio = services.resolve<gjt::AudioManager<AudioFiles>>();

        audio->createSound(AudioFiles::Hit, content->loadFromFile<sf::SoundBuffer>("content/hit.ogg"));
        audio->createSound(AudioFiles::Select, content->loadFromFile<sf::SoundBuffer>("content/select.ogg"));
        audio->createSound(AudioFiles::Mow, content->loadFromFile<sf::SoundBuffer>("content/mow.ogg"));
        audio->createSound(AudioFiles::Playstate, content->loadFromFile<sf::SoundBuffer>("content/playstate.ogg"));
        audio->createSound(AudioFiles::Menu, content->loadFromFile<sf::SoundBuffer>("content/menu.ogg"));
        audio->createSound(AudioFiles::Wush, content->loadFromFile<sf::SoundBuffer>("content/wush.ogg"));

        audio->queue(AudioFiles::Menu, true);
        audio->play(AudioFiles::Menu);

        auto saveFileManager = services.resolve<SaveFileManager>();
        auto saveFile = saveFileManager->load();
        audio->setGeneralVolume(saveFile->generalVolume);

        auto volumeBarTexture =
            content->loadFromFile<sf::Texture>("content/volumebar.png");

        volumeBar = std::make_shared<VolumeBar>(
            audio, volumeBarTexture,
            content->loadFromFile<sf::Texture>("content/volumebar_part.png"),
            std::make_shared<gjt::Tileset>(
                content->loadFromFile<sf::Texture>("content/tiles.png"), 16,
                16),
            1.5f);
        volumeBar->setPosition(
            getWindowWidth() - volumeBarTexture->getSize().x - 16.0f,
            getWindowHeight() / 2.0f);
        volumeBar->setOrigin(0, volumeBarTexture->getSize().y / 2.0f);

        switchState(std::static_pointer_cast<gjt::GameState, TitleState>(
            std::make_shared<TitleState>()));
    }

    virtual void handleEvent(const sf::Event &e, float dt) override
    {
        if (e.type == sf::Event::KeyReleased)
        {
            if (e.key.code == sf::Keyboard::Key::Add ||
                e.key.code == sf::Keyboard::Key::Equal)
                volumeBar->increaseVolume();
            if (e.key.code == sf::Keyboard::Key::Subtract ||
                e.key.code == sf::Keyboard::Key::Hyphen)
                volumeBar->decreaseVolume();
        }

        gjt::Game::handleEvent(e, dt);
    }

    virtual void unload() override
    {
        auto audio = services.resolve<gjt::AudioManager<AudioFiles>>();
        auto saveFileManager = services.resolve<SaveFileManager>();
        auto saveFile = saveFileManager->load();
        saveFile->generalVolume = audio->getGeneralVolume();
        saveFileManager->save();

        gjt::Game::unload();
    }
};
