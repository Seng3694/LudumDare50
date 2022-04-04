#include "VolumeBar.hpp"
#include "CustomMath.hpp"

VolumeBar::VolumeBar(
    std::shared_ptr<gjt::AudioManager<AudioFiles>> audioManager,
    std::shared_ptr<sf::Texture> volumeBarTexture,
    std::shared_ptr<sf::Texture> volumeBarPartTexture, const float fadeTimer)
    : audioManager(audioManager), volumeBarTexture(volumeBarTexture),
      volumeBarPartTexture(volumeBarPartTexture),
      state(VolumeBarState::Invisible), fadeTimer(fadeTimer),
      fadeTimerElapsed(fadeTimer), activeTimer(0.2f), activeTimerElapsed(activeTimer)
{
    volume = audioManager->getGeneralVolume() * 12; //0-12 instead of 0.0-1.0
    volumeBarSprite.setTexture(*volumeBarTexture);
}

void VolumeBar::increaseVolume()
{
    activeTimerElapsed = 0.0f;
    volume = gjt::clamp<float>(volume + 1.0f, 0.0f, 12.0f);
    audioManager->setGeneralVolume(volume / 12.0f);
    state = VolumeBarState::Active;
    volumeBarSprite.setColor(sf::Color::White);
}

void VolumeBar::decreaseVolume()
{
    activeTimerElapsed = 0.0f;
    volume = gjt::clamp<float>(volume - 1.0f, 0.0f, 12.0f);
    audioManager->setGeneralVolume(volume / 12.0f);
    state = VolumeBarState::Active;
    volumeBarSprite.setColor(sf::Color::White);
}

void VolumeBar::update(float dt)
{
    switch (state)
    {
    case VolumeBarState::Active:
        activeTimerElapsed += dt;
        if (activeTimerElapsed >= activeTimer)
        {
            fadeTimerElapsed = 0.0f;
            state = VolumeBarState::Fading;
        }
        break;
    case VolumeBarState::Fading:
        fadeTimerElapsed += dt;
        volumeBarSprite.setColor(sf::Color(
            0xff, 0xff, 0xff,
            (uint8_t)((1.0f - (fadeTimerElapsed / fadeTimer)) * 0xff)));

        if (fadeTimerElapsed >= fadeTimer)
        {
            state = VolumeBarState::Invisible;
            volumeBarSprite.setColor(sf::Color::Transparent);
        }
        break;
    }


}

void VolumeBar::draw(
    sf::RenderTarget &target, sf::RenderStates states) const
{
    if (state == VolumeBarState::Invisible)
        return;

    states.transform.combine(getTransform());

    target.draw(volumeBarSprite, states);

    sf::Sprite volumeBarPartSprite(*volumeBarPartTexture);

    if (state == VolumeBarState::Fading)
        volumeBarPartSprite.setColor(sf::Color(
            0xff, 0xff, 0xff,
            (uint8_t)((1.0f - (fadeTimerElapsed / fadeTimer)) * 0xff)));

    for (int i = 0; i < (int)volume; ++i)
    {
        volumeBarPartSprite.setPosition(
            10.0f, 
            10.0f + (11 - i) * volumeBarPartTexture->getSize().y);
        target.draw(volumeBarPartSprite, states);
    }
}
