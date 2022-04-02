#pragma once

#include <cstdint>
#include <SFML/Graphics.hpp>
#include <Tileset.hpp>
#include <vector>
#include <memory>

namespace gjt
{

struct AnimationFrame
{
    uint32_t textureRectangleIndex;
    float duration;
};

enum class AnimationState
{
    NotRunning,
    Running
};

class Animation
{
  public:
    Animation(
        std::shared_ptr<Tileset> tileset, const bool looping = false,
        const bool reverse = false);
    Animation(
        std::shared_ptr<Tileset> tileset, uint32_t frameCount, float totalDuration, const bool looping = false,
        const bool reverse = false);

    void addFrame(uint32_t textureIndex, float duration);
    void defineAnimation(uint32_t frameCount, float totalDuration);
    void start();
    void reset();
    void stop();

    inline const std::shared_ptr<Tileset> getTileset() const
    {
        return this->tileset;
    }

    inline void setLooping(const bool looping)
    {
        this->looping = looping;
    }
    inline void setReverse(const bool reverse)
    {
        this->reverse = reverse;
    }
    inline void setScale(const float scale)
    {
        this->scale = scale;
    }

    inline float &getScale()
    {
        return this->scale;
    }

    inline bool &getLooping()
    {
        return this->looping;
    }

    inline bool &getReverse()
    {
        return this->reverse;
    }

    void update(float dt);

    sf::IntRect getCurrentFrameTextureRectangle() const;

  private:
    std::shared_ptr<Tileset> tileset;
    std::vector<AnimationFrame> frames;
    float elapsed;
    float scale;
    uint32_t index;
    AnimationState state;
    bool looping;
    bool reverse;
};

}
