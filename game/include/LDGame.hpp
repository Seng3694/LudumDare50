#pragma once

#include "Game.hpp"
#include "PlayState.hpp"
#include "MapSelectionState.hpp"
#include "Message.hpp"
#include "MessageDispatcher.hpp"
#include "SaveFileManager.hpp"
#include "TitleState.hpp"
#include "AudioFiles.hpp"
#include <AudioManager.hpp>
#include "VolumeBar.hpp"
#include "Content.hpp"
#include "ContentManager.hpp"
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

        services.registerInstance<GameContentManager>();

        services.registerInstance<GameMessageDispatcher>();
        services.registerInstance<SaveFileManager>(
            std::make_shared<SaveFileManager>("save.bin"));
        services.registerInstance<gjt::AudioManager<AudioFiles>>();

        auto content = services.resolve<GameContentManager>();
        content->loadFromFile<sf::Font>("content/monogram-extended.ttf", Content::MonogramFont);

        content->loadFromFile<sf::Texture>("content/player.png", Content::MowBotTexture);
        content->addFromMemory(
            Content::MowBotTileset,
            std::make_shared<gjt::Tileset>(
                content->get<sf::Texture>(Content::MowBotTexture), 16, 16));

        content->loadFromFile<sf::Texture>("content/map_frame.png", Content::MapFrameTexture);
        content->loadFromFile<sf::Texture>("content/scrolling_rectangle.png", Content::ScrollingRectangleTexture);

        content->loadFromFile<sf::Texture>("content/mown_grass.png", Content::MownGrassTexture);
        content->addFromMemory(
            Content::MownGrassTileset,
            std::make_shared<gjt::Tileset>(
                content->get<sf::Texture>(Content::MownGrassTexture), 32, 32));

        content->loadFromFile<sf::Texture>("content/transition_banner_left.png", Content::TransitionBannerLeftTexture);
        content->loadFromFile<sf::Texture>("content/transition_banner_right.png", Content::TransitionBannerRightTexture);
        content->loadFromFile<sf::Texture>("content/volumebar.png", Content::VolumeBarTexture);
        content->loadFromFile<sf::Texture>("content/volumebar_part.png", Content::VolumeBarPartTexture);
        content->loadFromFile<sf::Texture>("content/sfml-logo-small.png", Content::SFMLTexture);

        content->loadFromFile<sf::Image>(
            "content/mowbot.png", Content::MowBotIconTexture);
        setWindowIcon(content->get<sf::Image>(Content::MowBotIconTexture));

        content->loadFromFile<sf::Texture>(
            "content/tiles.png", Content::MapTilesTexture);
        content->addFromMemory(
            Content::MapTilesTileset,
            std::make_shared<gjt::Tileset>(
                content->get<sf::Texture>(Content::MapTilesTexture), 16, 16));

        content->loadFromFile<sf::SoundBuffer>("content/hit.ogg", Content::HitSound);
        content->loadFromFile<sf::SoundBuffer>("content/select.ogg", Content::SelectSound);
        content->loadFromFile<sf::SoundBuffer>("content/mow.ogg", Content::MowSound);
        content->loadFromFile<sf::SoundBuffer>("content/playstate.ogg", Content::PlayStateSoundtrack);
        content->loadFromFile<sf::SoundBuffer>("content/menu.ogg", Content::MenuSoundtrack);
        content->loadFromFile<sf::SoundBuffer>("content/wush.ogg", Content::WushSound);

        auto audio = services.resolve<gjt::AudioManager<AudioFiles>>();

        auto saveFileManager = services.resolve<SaveFileManager>();
        auto saveFile = saveFileManager->load();
        audio->setGeneralVolume(saveFile->generalVolume);

        audio->createSound(AudioFiles::Hit,       content->get<sf::SoundBuffer>(Content::HitSound));
        audio->createSound(AudioFiles::Select,    content->get<sf::SoundBuffer>(Content::SelectSound));
        audio->createSound(AudioFiles::Mow,       content->get<sf::SoundBuffer>(Content::MowSound));
        audio->createSound(AudioFiles::Playstate, content->get<sf::SoundBuffer>(Content::PlayStateSoundtrack));
        audio->createSound(AudioFiles::Menu,      content->get<sf::SoundBuffer>(Content::MenuSoundtrack));
        audio->createSound(AudioFiles::Wush,      content->get<sf::SoundBuffer>(Content::WushSound));

        audio->queue(AudioFiles::Menu, true);
        audio->play(AudioFiles::Menu);

        auto volumeBarTexture =
            content->get<sf::Texture>(Content::VolumeBarTexture);

        volumeBar = std::make_shared<VolumeBar>(
            audio, volumeBarTexture,
            content->get<sf::Texture>(Content::VolumeBarPartTexture),
            content->get<gjt::Tileset>(Content::MapTilesTileset), 1.5f);
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
        auto dispatcher = services.resolve<GameMessageDispatcher>();
        dispatcher->broadcast(Message(e));
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
