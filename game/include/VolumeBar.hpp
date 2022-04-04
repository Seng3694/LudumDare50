#pragma once

#include <AudioManager.hpp>
#include <SFML/Graphics.hpp>
#include "AudioFiles.hpp"
#include <memory>

class VolumeBar : public sf::Transformable, public sf::Drawable
{
  public:
    VolumeBar(
        std::shared_ptr<gjt::AudioManager<AudioFiles>> audioManager,
        std::shared_ptr<sf::Texture> volumeBarTexture,
        std::shared_ptr<sf::Texture> volumeBarPartTexture, const float fadeTimer);

    void increaseVolume();
    void decreaseVolume();
    void update(float dt);

  private:
    enum class VolumeBarState
    {
        Invisible,
        Active,
        Fading
    };

    std::shared_ptr<gjt::AudioManager<AudioFiles>> audioManager;
    std::shared_ptr<sf::Texture> volumeBarTexture;
    std::shared_ptr<sf::Texture> volumeBarPartTexture;
    sf::Sprite volumeBarSprite;
    VolumeBarState state;
    float volume;
    float activeTimer;
    float activeTimerElapsed;
    float fadeTimer;
    float fadeTimerElapsed;

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};
