#pragma once

#include <SFML/Graphics.hpp>
#include <AnimatedSprite.hpp>
#include <CustomMath.hpp>
#include "TileMap.hpp"
#include "Tiles.hpp"
#include <stack>

enum class PlayerAnimationState
{
	Idle,
	Walking,
	Eating
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
        
        setPosition(map->getTransform().transformPoint(
            mapPosition.x * animation->getTileset()->getTileWidth(),
            mapPosition.y * animation->getTileset()->getTileHeight()));
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

    void moveTo(const MoveDirection direction)
    {
        sf::Vector2u oldPostion = mapPosition;
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

            if (type != TileType::Boulder)
            {
                if (type != TileType::Walkway)
                    gameState = PlayerGameState::Mowing;

                if (gameState == PlayerGameState::Mowing)
                {
                    moves.push({oldPostion, position, hp, type});
                    if (type == TileType::HighGrass)
                    {
                        map->setTile(
                            position.x, position.y,
                            TileType::LowGrass);
                        grassMown++;
                        score = gjt::clamp<int32_t>(grassMown - (maxHp - hp), 0, map->getMaxScore());
                    }
                    else
                    {
                        hp = gjt::clamp<int32_t>(hp - 1, 0, 5);
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
    int32_t grassMown;
    int32_t score;
    PlayerGameState gameState;
    int32_t maxHp;
    int32_t hp;

};