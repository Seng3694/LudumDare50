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
#include "MapView.hpp"
#include "TransitionState.hpp"
#include <AudioManager.hpp>
#include "AudioFiles.hpp"

#include "Maps.hpp"

class TitleState;
class PlayState;

class MapSelectionState : public gjt::GameState
{
  private:
    void updateMapView(std::shared_ptr<MapView> mapView, sf::Vector2u bounds, int32_t page, int32_t i)
    {
        uint32_t x = (i % 3);
        uint32_t y = (i / 3);
        float xPos = 0.0f;
        float yPos = 0.0f;

        switch (x)
        {
        case 0: // left align
            xPos = 0.0f;
            break;
        case 1: // center align
            xPos = bounds.x / 2.0f -
                    mapView->getPixelWidth() / 2.0f;
            break;
        case 2: // right align
            xPos = bounds.x - mapView->getPixelWidth() -
                    0.0f;
            break;
        }

        switch (y)
        {
        case 0: // top align
            yPos = 0.0f;
            break;
        case 1: // center align
            yPos = bounds.y / 2.0f -
                    mapView->getPixelHeight() / 2.0f;
            break;
        case 2: // bottom align
            yPos = bounds.y - mapView->getPixelHeight() -
                    0.0f;
            break;
        }

        mapView->setFrameColor(sf::Color(0x5f574fff));
        mapView->setPosition(xPos, yPos);
    }

  public:
    std::shared_ptr<sf::Font> font;
    std::shared_ptr<gjt::Tileset> tileset;
    std::shared_ptr<SaveData> saveData;
    sf::RenderTexture renderTexture;
    sf::Sprite renderTextureSprite;
    std::vector<std::shared_ptr<MapView>> mapViews;
    std::shared_ptr<MapView> selectedMapView;
    sf::Text headerText;
    sf::Text mapNameText;
    char clearTimeBuffer[10];
    float mapSelectionOffsetLeft;
    float mapSelectionOffsetTop;
    float mapSelectionOffsetBottom;
    float mapSelectionSpacing;

    int32_t currentPage;
    int32_t selectedIndex;

    MapSelectionState(const int32_t startIndex = 0) : selectedIndex(startIndex)
    {
    }

    virtual void load() override
    {
        auto content = services->resolve<GameContentManager>();
        font = content->get<sf::Font>(Content::MonogramFont);

        saveData = services->resolve<SaveFileManager>()->load();
        
        headerText.setFont(*font);
        headerText.setCharacterSize(60);
        headerText.setStyle(sf::Text::Bold);
        headerText.setString("Map Selection");
        sf::FloatRect localBounds = headerText.getLocalBounds();
        headerText.setOrigin(localBounds.width / 2.0f, localBounds.height / 2.0f);
        headerText.setOutlineThickness(2.0f);
        headerText.setOutlineColor(sf::Color::Black);
        headerText.setFillColor(sf::Color(0xffa300ff));
        headerText.setPosition(game->getWindowWidth() / 2.0f, 10.0f);

        mapNameText.setFont(*font);
        mapNameText.setCharacterSize(36);
        mapNameText.setStyle(sf::Text::Bold);
        mapNameText.setString(get_map_name((Maps)selectedIndex));
        localBounds = mapNameText.getLocalBounds();
        mapNameText.setOrigin(
            localBounds.width / 2.0f, localBounds.height / 2.0f);
        mapNameText.setOutlineThickness(2.0f);
        mapNameText.setOutlineColor(sf::Color::Black);
        mapNameText.setFillColor(sf::Color(0xffa300ff));
        mapNameText.setPosition(game->getWindowWidth() / 2.0f, game->getWindowHeight() - 45.0f);

        mapSelectionOffsetLeft = 40.0f;
        mapSelectionOffsetTop = 80.0f;
        mapSelectionOffsetBottom = 50.0f;

        renderTexture.create(
            game->getWindowWidth() - mapSelectionOffsetLeft * 2,
            game->getWindowHeight() - mapSelectionOffsetTop -
                mapSelectionOffsetBottom);
        
        tileset = content->get<gjt::Tileset>(Content::MapTilesTileset);

        for (Maps i = Maps::Map1; i < Maps::Count; i = (Maps)((int)i + 1))
        {
            auto tm = std::make_shared<TileMap>(tileset, 12, 10, get_map_data(i));
            tm->setRenderGrid(false);

            mapViews.push_back(std::make_shared<MapView>(
                services, tm, saveData->scores[(uint32_t)i]));
        }

        currentPage = 0;
        game->setClearColor(sf::Color(0x1d2b53ff));

        renderTexture.clear(sf::Color::Transparent);
        uint32_t mapStart = currentPage * 9;
        uint32_t mapEnd = currentPage * 9 + mapViews.size();

        for (uint32_t i = mapStart; i < mapEnd; ++i)
        {
            std::shared_ptr<MapView> mapView = mapViews[i];
            updateMapView(mapView, renderTexture.getSize(), 0, i);
            renderTexture.draw(*mapView);
        }

        renderTexture.display();
        renderTextureSprite.setTexture(renderTexture.getTexture());
        renderTextureSprite.setPosition(
            mapSelectionOffsetLeft, mapSelectionOffsetTop);

        selectedMapView = mapViews[selectedIndex];
        updateMapView(
            selectedMapView, renderTexture.getSize(), 0, selectedIndex);
        selectedMapView->setFrameColor(sf::Color(0xffa300ff));
        selectedMapView->move(mapSelectionOffsetLeft, mapSelectionOffsetTop);
    }

    virtual void update(float dt) override
    {
    }

    virtual void draw(
        float dt, sf::RenderTarget &target,
        sf::RenderStates states = sf::RenderStates()) override
    {
        target.draw(renderTextureSprite, states);
        target.draw(*selectedMapView, states);
        target.draw(headerText, states);
        target.draw(mapNameText, states);
    }

    virtual void handleEvent(const sf::Event &e, float dt) override
    {
        if (e.type == sf::Event::KeyPressed)
        {
            if (e.key.code == sf::Keyboard::Key::Escape)
            {
                game->switchState(
                    std::static_pointer_cast<gjt::GameState, TransitionState>(
                        std::make_shared<TransitionState>(
                            game->getCurrentState(),
                            std::make_shared<TitleState>())));
                return;
            }

            int index = selectedIndex;
            int y = index / 3;
            if (e.key.code == sf::Keyboard::Key::D ||
                e.key.code == sf::Keyboard::Key::Right)
            {
                selectedIndex = gjt::wrap<int32_t>(selectedIndex + 1, (y * 3), (y * 3) + 2);
            }
            if (e.key.code == sf::Keyboard::Key::A ||
                e.key.code == sf::Keyboard::Key::Left)
            {
                selectedIndex = gjt::wrap<int32_t>(selectedIndex -1, (y * 3), (y * 3) + 2);
            }
            if (e.key.code == sf::Keyboard::Key::W ||
                e.key.code == sf::Keyboard::Key::Up)
            {
                selectedIndex = gjt::wrap<int32_t>(selectedIndex - 3, 0, 8);
            }
            if (e.key.code == sf::Keyboard::Key::S ||
                e.key.code == sf::Keyboard::Key::Down)
            {
                selectedIndex = gjt::wrap<int32_t>(selectedIndex + 3, 0, 8);
            }
            if (e.key.code == sf::Keyboard::Enter)
            {

                auto audio = services->resolve<gjt::AudioManager<AudioFiles>>();
                audio->stop(AudioFiles::Menu);
                audio->queue(AudioFiles::Playstate, true);
                audio->play(AudioFiles::Playstate);

                game->switchState(
                    std::static_pointer_cast<gjt::GameState, TransitionState>(
                        std::make_shared<TransitionState>(
                            game->getCurrentState(),
                            std::make_shared<PlayState>(
                                (Maps)selectedIndex))));
                return;
            }

            if (index != selectedIndex)
            {
                mapNameText.setString(get_map_name((Maps)selectedIndex));
                sf::FloatRect localBounds = mapNameText.getLocalBounds();
                mapNameText.setOrigin(
                    localBounds.width / 2.0f, localBounds.height / 2.0f);
                mapNameText.setPosition(
                    game->getWindowWidth() / 2.0f,
                    game->getWindowHeight() - 45.0f);

                selectedMapView = mapViews[selectedIndex];
                updateMapView(
                    selectedMapView, renderTexture.getSize(), 0,
                    selectedIndex);
                selectedMapView->setFrameColor(sf::Color(0xffa300ff));
                selectedMapView->move(
                    mapSelectionOffsetLeft, mapSelectionOffsetTop);

                auto audio =
                    services->resolve<gjt::AudioManager<AudioFiles>>();
                audio->setVolume(AudioFiles::Select, 50.0f);
                audio->play(AudioFiles::Select);
            }
        }
    }
};
