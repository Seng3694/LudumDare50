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
    AnimatedSprite() : sf::Sprite(), key(), animations()
    {
    }

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

    void addAnimation(const AnimationKey &key, std::shared_ptr<Animation> animation)
    {
        animations[key] = animation;
    }

    void update(float dt)
    {
        std::shared_ptr<Animation> current = animations.find(key)->second;
        current->update(dt);
        setTexture(*current->getTileset()->getTexture());
        setTextureRect(current->getCurrentFrameTextureRectangle());
    }


  private:
    AnimationKey key;
    std::unordered_map<AnimationKey, std::shared_ptr<Animation>> animations;
};

}
