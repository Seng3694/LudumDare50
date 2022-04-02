#include "Animation.hpp"
#include "CustomMath.hpp"

namespace gjt
{

Animation::Animation(
    std::shared_ptr<Tileset> ts, const bool loop, const bool rev)
    : tileset(ts), frames(), elapsed(0.0f), scale(1.0f), index(0),
      state(AnimationState::NotRunning), looping(loop), reverse(rev)
{
}

Animation::Animation(
    std::shared_ptr<Tileset> ts, uint32_t frameCount, float totalDuration,
    const bool loop, const bool rev)
    : tileset(ts), frames(), elapsed(0.0f), scale(1.0f), index(0),
      state(AnimationState::NotRunning), looping(loop), reverse(rev)
{
    defineAnimation(frameCount, totalDuration);
}

void Animation::addFrame(uint32_t textureIndex, float duration)
{
    frames.push_back({textureIndex, duration});
}

void Animation::defineAnimation(uint32_t frameCount, float totalDuration)
{
    const float durationPerFrame = totalDuration / frameCount;
    for (uint32_t i = 0; i < frameCount; ++i)
        frames.push_back({i, durationPerFrame});
}

void Animation::start()
{
    state = AnimationState::Running;
}

void Animation::reset()
{
    elapsed = 0.0f;
    index = 0;
}

void Animation::stop()
{
    state = AnimationState::NotRunning;
}

void Animation::update(float dt)
{
    if (state == AnimationState::NotRunning)
        return;

    AnimationFrame const *current = &frames[index];
    elapsed += dt;
    
    while (elapsed >= (current->duration * scale))
    {
        elapsed =
            clamp<float>(elapsed - (current->duration * scale), 0.0f, 1.0f);

        if (looping)
        {
            if (reverse)
                index = index == 0 ? frames.size() - 1 : index - 1;
            else
                index = (index + 1) == frames.size() ? 0 : index + 1;
        }
        else
        {
            if (reverse)
            {
                if (index == 0)
                    state = AnimationState::NotRunning;
                else
                    index--;
            }
            else
            {
                if (index == frames.size() - 1)
                    state = AnimationState::NotRunning;
                else
                    index++;
            }
        }

        current = &frames[index];
    }
}

sf::IntRect Animation::getCurrentFrameTextureRectangle() const
{
    return tileset->getTextureRect(frames[index].textureRectangleIndex);
}

}