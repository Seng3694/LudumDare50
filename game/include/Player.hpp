#pragma once

#include <SFML/Graphics.hpp>
#include <AnimatedSprite.hpp>
#include <CustomMath.hpp>
#include "TileMap.hpp"
#include "Tiles.hpp"

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

    inline void moveTo(const MoveDirection direction)
    {
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

  private:
    std::shared_ptr<TileMap> map;
    sf::Vector2u mapPosition;
    int32_t grassMown;
    int32_t score;
    PlayerGameState gameState;
    int32_t maxHp;
    int32_t hp;

};