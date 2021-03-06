#pragma once

#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "Animation.hpp"
#include <memory>

namespace gjt
{

template<typename AnimationKey>
class AnimatedSprite : public sf::Sprite
{
  public:
    AnimatedSprite() : sf::Sprite(), key(), animations(), isAnimating(true)
    {
    }
    virtual ~AnimatedSprite() {}

    inline void setAnimationKey(const AnimationKey key)
    {
        this->key = key;
    }
    inline AnimationKey &getAnimationKey()
    {
        return this->key;
    }

    inline std::shared_ptr<Animation> getCurrentAnimation()
    {
        return animations[key];
    }

    inline void setIsAnimating(const bool isAnimating)
    {
        this->isAnimating = isAnimating;
    }

    void addAnimation(const AnimationKey &key, std::shared_ptr<Animation> animation)
    {
        animations[key] = animation;

        if (animations.size() == 1)
        {
            setTexture(*animation->getTileset()->getTexture());
            setTextureRect(animation->getCurrentFrameTextureRectangle());
        }
    }

    virtual void update(float dt)
    {
        if (isAnimating)
        {
            std::shared_ptr<Animation> current = animations.find(key)->second;
            current->update(dt);
            setTexture(*current->getTileset()->getTexture());
            setTextureRect(current->getCurrentFrameTextureRectangle());
        }
    }

  private:
    AnimationKey key;
    std::unordered_map<AnimationKey, std::shared_ptr<Animation>> animations;
    bool isAnimating;
};

}
