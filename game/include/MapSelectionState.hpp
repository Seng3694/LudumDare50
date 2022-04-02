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

#include "Maps.hpp"

enum class Maps
{
    Map1,
    Map2,
    Map3,
    Map4,
    Map5,
    Map6,
    Map7,
    Map8,
    Map9,
    Count
};

const sf::String &get_map_name(Maps maps)
{
    switch (maps)
    {
    case Maps::Map1:
        return map_1_name;
    case Maps::Map2:
        return map_2_name;
    case Maps::Map3:
        return map_3_name;
    default:
        return map_1_name;
    }
}

const uint8_t const *get_map_data(Maps maps)
{
    switch (maps)
    {
    case Maps::Map1:
        return map_1;
    case Maps::Map2:
        return map_2;
    case Maps::Map3:
        return map_3;
    default:
        return map_1;
    }
}

const sf::Vector2u &get_map_spawn(Maps maps)
{
    switch (maps)
    {
    case Maps::Map1:
        return map_1_spawn;
    case Maps::Map2:
        return map_2_spawn;
    case Maps::Map3:
        return map_3_spawn;
    default:
        return map_1_spawn;
    }
}

struct MapSelectionDebugUiContext
{
    float mapSelectionOffsetLeft;
    float mapSelectionOffsetTop;
    float mapSelectionSpacing;
};

class MapSelectionState : public gjt::GameState
{
  public:
    std::shared_ptr<sf::Font> font;
    std::shared_ptr<sf::Texture> frameTexture;
    std::shared_ptr<gjt::Tileset> tileset;
    sf::RenderTexture renderTexture;
    std::unordered_map<Maps, std::shared_ptr<TileMap>> tileMaps;
    sf::Sprite frameSprite;
    sf::Text headerText;
    sf::Text mapNameText;
    float mapSelectionOffsetLeft;
    float mapSelectionOffsetTop;
    float mapSelectionOffsetBottom;
    float mapSelectionSpacing;

    int32_t currentPage;
    int32_t selectedIndex;

    MapSelectionDebugUiContext context;
    bool enableDebug;

    virtual void load() override
    {
        auto content = services->resolve<gjt::ContentManager>();
        font =
            content->loadFromFile<sf::Font>("content/monogram-extended.ttf");
        frameTexture =
            content->loadFromFile<sf::Texture>("content/map_frame.png");

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
        mapNameText.setString(map_1_name);
        localBounds = mapNameText.getLocalBounds();
        mapNameText.setOrigin(
            localBounds.width / 2.0f, localBounds.height / 2.0f);
        mapNameText.setOutlineThickness(2.0f);
        mapNameText.setOutlineColor(sf::Color::Black);
        mapNameText.setFillColor(sf::Color(0xffa300ff));
        mapNameText.setPosition(game->getWindowWidth() / 2.0f, game->getWindowHeight() - 45.0f);

        frameSprite.setTexture(*frameTexture);

        mapSelectionOffsetLeft = 40.0f;
        mapSelectionOffsetTop = 80.0f;
        mapSelectionOffsetBottom = 50.0f;

        renderTexture.create(
            game->getWindowWidth() - mapSelectionOffsetLeft * 2,
            game->getWindowHeight() - mapSelectionOffsetTop -
                mapSelectionOffsetBottom);
        
        tileset = std::make_shared<gjt::Tileset>(
            content->loadFromFile<sf::Texture>("content/tiles.png"), 16, 16);

        tileMaps[Maps::Map1] =
            std::make_shared<TileMap>(tileset, 12, 10, get_map_data(Maps::Map1));
        tileMaps[Maps::Map2] = std::make_shared<TileMap>(
            tileset, 12, 10, get_map_data(Maps::Map2));
        tileMaps[Maps::Map3] = std::make_shared<TileMap>(
            tileset, 12, 10, get_map_data(Maps::Map3));

        for (Maps i = Maps::Map4; i < Maps::Count; i = (Maps)((int)i + 1))
            tileMaps[i] = std::make_shared<TileMap>(
                tileset, 12, 10, get_map_data(i));
        
        for (auto &m : tileMaps)
            m.second->setRenderGrid(false);

        enableDebug = false;
        currentPage = 0;
        selectedIndex = 0;
        game->setClearColor(sf::Color(0x1d2b53ff));
    }

    virtual void update(float dt) override
    {
    }

    virtual void ui(float dt) override
    {
        if (enableDebug)
        {
            ImGui::SetNextWindowSize(
                ImVec2(400, 400), ImGuiCond_::ImGuiCond_Once);
            if (ImGui::Begin("debug"))
            {
                if (ImGui::SliderFloat(
                        "offset left", &mapSelectionOffsetLeft, 0, 300))
                {
                }
                if (ImGui::SliderFloat(
                        "offset top", &mapSelectionOffsetTop, 0, 300))
                {
                }
                if (ImGui::SliderFloat(
                        "spacing", &mapSelectionSpacing, 0, 300))
                {
                }

                ImGui::End();
            }
        }
    }

    virtual void draw(
        float dt, sf::RenderTarget &target,
        sf::RenderStates states = sf::RenderStates()) override
    {
        renderTexture.clear(sf::Color::Transparent);
        uint32_t mapStart = currentPage * 9;
        uint32_t mapEnd = currentPage * 9 + tileMaps.size();

        for (uint32_t i = mapStart; i < mapEnd; ++i)
        {
            auto tm = tileMaps[(Maps)i];
            uint32_t x = (i % 3);
            uint32_t y = (i / 3);
            float xPos = 0.0f;
            float yPos = 0.0f;

            switch (x)
            {
            case 0://left align
                xPos = 6.0f;
                break;
            case 1://center align
                xPos = renderTexture.getSize().x / 2.0f -
                       tm->getPixelWidth() / 2.0f;
                break;
            case 2://right align
                xPos = renderTexture.getSize().x - tm->getPixelWidth() - 6.0f;
                break;
            }

           switch (y)
            {
            case 0: // top align
                yPos = 6.0f;
                break;
            case 1: // center align
                yPos = renderTexture.getSize().y / 2.0f -
                       tm->getPixelHeight() / 2.0f;
                break;
            case 2: // bottom align
                yPos = renderTexture.getSize().y - tm->getPixelHeight() - 6.0f;
                break;
            }

            frameSprite.setPosition(xPos - 6, yPos - 6);
            if (selectedIndex == i)
                frameSprite.setColor(sf::Color(0xffa300ff));
            else
                frameSprite.setColor(sf::Color(0xab5236ff));

            renderTexture.draw(frameSprite);

            tm->setPosition(xPos, yPos);
            renderTexture.draw(*tm);
        }

        renderTexture.display();
        sf::Sprite s(renderTexture.getTexture());
        s.setPosition(mapSelectionOffsetLeft, mapSelectionOffsetTop);
        target.draw(s, states);
        target.draw(headerText, states);
        target.draw(mapNameText, states);
    }

    virtual void handleEvent(const sf::Event &e, float dt) override
    {
        if (e.type == sf::Event::KeyPressed)
        {
            if (e.key.code == sf::Keyboard::Key::D && e.key.control)
            {
                enableDebug = !enableDebug;
                return;
            }

            int index = selectedIndex;
            if (e.key.code == sf::Keyboard::Key::D ||
                e.key.code == sf::Keyboard::Key::Right)
            {
                selectedIndex = gjt::wrap<int32_t>(selectedIndex + 1, 0, 8);
            }
            if (e.key.code == sf::Keyboard::Key::A ||
                e.key.code == sf::Keyboard::Key::Left)
            {
                selectedIndex = gjt::wrap<int32_t>(selectedIndex -1, 0, 8);
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
                game->switchState<PlayState>(
                    std::make_shared<PlayState>(
                        get_map_data((Maps)selectedIndex),
                        get_map_name((Maps)selectedIndex),
                        get_map_spawn((Maps)selectedIndex)
                        ));
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
            }
        }
    }
};
