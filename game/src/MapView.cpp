#include "MapView.hpp"
#include "ContentManager.hpp"
#include "StringFormat.hpp"
#include "Content.hpp"

MapView::MapView(
    gjt::ServiceLocator *services, std::shared_ptr<TileMap> tileMap,
    const Score &score)
    : tileMap(tileMap), score(score), renderMoves(false)
{
    auto content = services->resolve<GameContentManager>();
    font = content->get<sf::Font>(Content::MonogramFont);

    frame.setTexture(
        *content->get<sf::Texture>(Content::MapFrameTexture));

    tileset = content->get<gjt::Tileset>(Content::MapTilesTileset);

    starSpacing = 6.0f;
    starOffset = 38.0f;
    float frameOffset = 6.0f;

    // stars
    for (uint32_t i = 0; i < 5; ++i)
    {
        stars[i].setTexture(*tileset->getTexture());
        if (score.value >= i + 1)
        {
            stars[i].setTextureRect(
                tileset->getTextureRect((uint32_t)TileType::Star));
        }
        else
        {
            stars[i].setTextureRect(
                tileset->getTextureRect((uint32_t)TileType::StarEmpty));
        }

        stars[i].setPosition(
            frameOffset + starOffset +
                i * (tileset->getTileWidth() + starSpacing),
            -6 + frameOffset);
    }

    // time text
    clearTimeText.setFont(*font);
    clearTimeText.setCharacterSize(26);
    clearTimeText.setStyle(sf::Text::Bold);
    clearTimeText.setString("N/A");
    clearTimeText.setOutlineThickness(2.0f);
    clearTimeText.setOutlineColor(sf::Color::Black);
    clearTimeText.setFillColor(sf::Color(0xffa300ff));

    char clearTimeBuffer[10];
    if (score.time < 1.0e-5f)
    {
        snprintf(clearTimeBuffer, 10, "N/A");
    }
    else
    {
        StringFormat::formatSeconds(
            score.time, clearTimeBuffer, 10);
    }

    clearTimeText.setString(clearTimeBuffer);
    sf::FloatRect localBounds = clearTimeText.getLocalBounds();
    clearTimeText.setOrigin(
        localBounds.width / 2.0f, localBounds.height / 2.0f);
    clearTimeText.setPosition(
        tileMap->getPixelWidth() / 2.0f,
        tileMap->getPixelHeight() - localBounds.height);

    tileMap->setPosition(frameOffset, frameOffset);
}

MapView::MapView(
    gjt::ServiceLocator *services,
    std::shared_ptr<TileMap> tileMap, const std::vector<PlayerMove> &moves,
    const Score &score)
    : MapView(services, tileMap, score)
{
    renderMoves = true;

    sf::Vector2f tileSize = sf::Vector2f(
        tileMap->getTileset()->getTileWidth(),
        tileMap->getTileset()->getTileHeight());

    sf::Transform mapTransform = tileMap->getTransform();

    lines.resize(moves.size());

    const float thickness = 4.0f;
    for (uint32_t i = 0; i < moves.size(); ++i)
    {
        sf::Vector2f direction =
            gjt::normalize(sf::Vector2f(moves[i].newPosition) - sf::Vector2f(moves[i].oldPosition));

        lines.push_back(Line(
            mapTransform.transformPoint(sf::Vector2f(
                moves[i].oldPosition.x * tileSize.x + tileSize.x / 2.0f - (direction.x * (thickness / 2.0f)),
                moves[i].oldPosition.y * tileSize.y + tileSize.y / 2.0f - (direction.y * (thickness / 2.0f)))),
            mapTransform.transformPoint(sf::Vector2f(
                moves[i].newPosition.x * tileSize.x + tileSize.x / 2.0f + (direction.x * (thickness / 2.0f)),
                moves[i].newPosition.y * tileSize.y + tileSize.y / 2.0f + (direction.y * (thickness / 2.0f)))),
            sf::Color(0x7e2553ff), thickness));
    }
}

void MapView::updateTextScale()
{
    clearTimeText.setCharacterSize(26 * getScale().x);
    clearTimeText.setOutlineThickness(2.0f * getScale().x);
    sf::FloatRect localBounds = clearTimeText.getLocalBounds();
    clearTimeText.setOrigin(
        localBounds.width / 2.0f, localBounds.height / 2.0f);
    clearTimeText.setPosition(
        tileMap->getPixelWidth() / 2.0f,
        tileMap->getPixelHeight() - localBounds.height * 1.5f);
}

void MapView::draw(
    sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::RenderStates copyStates = states;
    states.transform.combine(getTransform());

    target.draw(frame, states);
    target.draw(*tileMap, states);

    if (renderMoves)
    {
        for (auto &line : lines)
            target.draw(line, states);
    }

    for (int i = 0; i < 5; ++i)
        target.draw(stars[i], states);

    copyStates.transform = copyStates.transform.translate(getPosition());
    copyStates.transform = copyStates.transform.translate(-getOrigin().x, getOrigin().y);
    
    target.draw(clearTimeText, copyStates); 
}
