#pragma once

#include <SFML/Graphics.hpp>
#include <AnimatedSprite.hpp>
#include <CustomMath.hpp>
#include "TileMap.hpp"
#include "Tiles.hpp"
#include <stack>
#include <AudioManager.hpp>
#include "AudioFiles.hpp"
#include <functional>

enum class PlayerAnimationState
{
	Idle,
	Walking,
	Eating
};

enum class PlayerTiles
{
    Right,
    Left,
    Up,
    Down
};

enum class PlayerGameState
{
    Preparation,
    Mowing
};

enum class MoveDirection
{
    Left,
    Up,
    Right,
    Down
};

struct PlayerMove
{
    sf::Vector2u oldPosition;
    sf::Vector2u newPosition;
    int32_t hp;
    TileType tile;
};

class Player : public gjt::AnimatedSprite<PlayerAnimationState>
{
  public:
    Player(std::shared_ptr<TileMap> m)
        : gjt::AnimatedSprite<PlayerAnimationState>(), map(m),
          gameState(PlayerGameState::Preparation), maxHp(5), hp(maxHp), score(0), grassMown(0)
    {
    }

    inline sf::Vector2u getMapPosition() const
    {
        return mapPosition;
    }

    inline void setMapPosition(const uint32_t x, const uint32_t y)
    {
        mapPosition.x = x;
        mapPosition.y = y;
        
        auto animation = getCurrentAnimation();
        
        sf::Vector2f origin = getOrigin();
        setPosition(map->getTransform().transformPoint(
            mapPosition.x * animation->getTileset()->getTileWidth() + origin.x,
            mapPosition.y * animation->getTileset()->getTileHeight() + origin.y));
    }

    inline void setMapPosition(const sf::Vector2u position)
    {
        setMapPosition(position.x, position.y);
    }

    inline PlayerGameState getGameState()
    {
        return gameState;
    }
    inline void setGameState(const PlayerGameState state)
    {
        gameState = state;
    }

    inline void setGrassMownCallback(
        std::function<void(uint32_t, uint32_t)> callback)
    {
        grassMownCallback = callback;
    }

    inline void setPlayerHitCallback(std::function<void(void)> callback)
    {
        playerHitCallback = callback;
    }

    void moveTo(const MoveDirection direction)
    {
        sf::Vector2u oldPosition = mapPosition;
        sf::Vector2u position = mapPosition;

        switch (direction)
        {
        case MoveDirection::Left:
            position.x -= 1;
            break;
        case MoveDirection::Up:
            position.y -= 1;
            break;
        case MoveDirection::Right:
            position.x += 1;
            break;
        case MoveDirection::Down:
            position.y += 1;
            break;
        }

        if (position.x >= 0 && position.x < map->getWidth() &&
            position.y >= 0 && position.y < map->getHeight())
        {
            TileType type = (TileType)map->getTile(position.x, position.y);

            setFacingDirection(oldPosition, position);

            if (type != TileType::Boulder)
            {
                if (type != TileType::Walkway)
                    gameState = PlayerGameState::Mowing;

                if (gameState == PlayerGameState::Mowing)
                {
                    moves.push({oldPosition, position, hp, type});

                    if (type == TileType::HighGrass)
                    {
                        map->setTile(
                            position.x, position.y,
                            TileType::LowGrass);
                        grassMown++;
                        score = gjt::clamp<int32_t>(
                            grassMown - (maxHp - hp), 0, map->getMaxScore());
                        if (grassMownCallback)
                            grassMownCallback(position.x, position.y);
                    }
                    else
                    {
                        hp = gjt::clamp<int32_t>(hp - 1, 0, 5);
                        if (playerHitCallback)
                            playerHitCallback();
                        score = gjt::clamp<int32_t>(
                            grassMown - (maxHp - hp), 0, map->getMaxScore());
                    }
                }
                
                setMapPosition(position);
            }
        }
    }

    bool undoMove()
    {
        if (moves.size() == 0)
            return false;
        PlayerMove lastMove = moves.top();
        moves.pop();

        if (lastMove.tile == TileType::HighGrass)
        {
            map->setTile(
                lastMove.newPosition.x, lastMove.newPosition.y,
                TileType::HighGrass);

            grassMown--;
            score = gjt::clamp<int32_t>(
                grassMown - (maxHp - hp), 0, map->getMaxScore());
        }

        hp = lastMove.hp;

        
        
        setFacingDirection(lastMove.oldPosition, lastMove.newPosition);
        setMapPosition(lastMove.oldPosition);

        return true;
    }

    inline int32_t &getScore()
    {
        return score;
    }

    inline int32_t &getGrassMown()
    {
        return grassMown;
    }

    inline int32_t &getHp()
    {
        return hp;
    }

    inline uint32_t getPlayerMoveCount()
    {
        return (uint32_t)moves.size();
    }

    inline const std::stack<PlayerMove> &getPlayerMoves() const
    {
        return moves;
    }

  private:
    std::stack<PlayerMove> moves;
    std::shared_ptr<TileMap> map;
    sf::Vector2u mapPosition;
    std::function<void(uint32_t, uint32_t)> grassMownCallback;
    std::function<void(void)> playerHitCallback;
    int32_t grassMown;
    int32_t score;
    PlayerGameState gameState;
    int32_t maxHp;
    int32_t hp;


    void setFacingDirection(const sf::Vector2u oldPosition, const sf::Vector2u newPosition)
    {
        const sf::Vector2f direction = gjt::normalize(
            sf::Vector2f(newPosition) - sf::Vector2f(oldPosition));

        if (direction.x > 0)
            setTextureRect(getCurrentAnimation()->getTileset()->getTextureRect(
                (uint32_t)PlayerTiles::Right));
        else if (direction.x < 0)
            setTextureRect(getCurrentAnimation()->getTileset()->getTextureRect(
                (uint32_t)PlayerTiles::Left));
        else if (direction.y > 0)
            setTextureRect(getCurrentAnimation()->getTileset()->getTextureRect(
                (uint32_t)PlayerTiles::Down));
        else if (direction.y < 0)
            setTextureRect(getCurrentAnimation()->getTileset()->getTextureRect(
                (uint32_t)PlayerTiles::Up));
    }
};