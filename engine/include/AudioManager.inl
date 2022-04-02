#pragma once

#include "AudioManager.hpp"
#include <SFML/Audio.hpp>

namespace gjt
{
template<typename ResourceKey>
AudioManager<ResourceKey>::AudioManager()
{
    generalVolume = 1.0f;
}

template <typename ResourceKey>
void AudioManager<ResourceKey>::createSound(
    const ResourceKey &key, const std::shared_ptr<sf::SoundBuffer> buffer)
{
    setBuffer(key, buffer);
}

template <typename ResourceKey>
void AudioManager<ResourceKey>::setBuffer(
    const ResourceKey &key, const std::shared_ptr<sf::SoundBuffer> buffer)
{
    auto itr = sounds.find(key);
    if (itr != sounds.end())
    {
        itr->second.setBuffer(*buffer);
    }
    else
    {
        sounds.emplace(key, sf::Sound(*buffer));
        volumes.emplace(key, 100.0f);
        itr = sounds.find(key);
    }
    itr->second.setVolume(volumes.find(key)->second * generalVolume);
}

template <typename ResourceKey>
void AudioManager<ResourceKey>::setPitch(
    const ResourceKey &key, const float pitch)
{
    auto itr = sounds.find(key);
    if (itr != sounds.end())
    {
        itr->second.setPitch(pitch);
    }
}

template <typename ResourceKey>
void AudioManager<ResourceKey>::setVolume(
    const ResourceKey &key, const float volume)
{
    auto itr = sounds.find(key);
    if (itr != sounds.end())
    {
        volumes[key] = volume;
        itr->second.setVolume(volume * generalVolume);
    }
}

template <typename ResourceKey>
sf::Time AudioManager<ResourceKey>::getOffset(const ResourceKey &key)
{
    auto itr = sounds.find(key);
    if (itr != sounds.end())
    {
        return itr->second.getPlayingOffset();
    }
    return sf::Time();
}
template <typename ResourceKey>
void AudioManager<ResourceKey>::setOffset(
    const ResourceKey &key, const sf::Time &offset)
{
    auto itr = sounds.find(key);
    if (itr != sounds.end())
    {
        itr->second.setPlayingOffset(offset);
    }
}

template <typename ResourceKey>
void AudioManager<ResourceKey>::queue(const ResourceKey &key, const bool loop)
{
    auto itr = sounds.find(key);
    if (itr != sounds.end())
    {
        itr->second.setLoop(loop);
        soundQueue.push(&itr->second);
        if (soundQueue.size() == 1)
            itr->second.play();
    }
}
template <typename ResourceKey>
void AudioManager<ResourceKey>::play(const ResourceKey &key)
{
    auto itr = sounds.find(key);
    if (itr != sounds.end())
    {
        itr->second.play();
    }
}
template <typename ResourceKey>
void AudioManager<ResourceKey>::stop(const ResourceKey &key)
{
    auto itr = sounds.find(key);
    if (itr != sounds.end())
    {
        itr->second.stop();
    }
}
template <typename ResourceKey>
void AudioManager<ResourceKey>::pause(const ResourceKey &key)
{
    auto itr = sounds.find(key);
    if (itr != sounds.end())
    {
        itr->second.pause();
    }
}

template <typename ResourceKey>
sf::SoundSource::Status AudioManager<ResourceKey>::getStatus(
    const ResourceKey &key)
{
    auto itr = sounds.find(key);
    if (itr != sounds.end())
    {
        return itr->second.getStatus();
    }
    return sf::SoundSource::Status::Stopped;
}

template <typename ResourceKey>
inline float AudioManager<ResourceKey>::getGeneralVolume() const
{
    return generalVolume;
}

template <typename ResourceKey>
void AudioManager<ResourceKey>::setGeneralVolume(const float volume)
{
    generalVolume = volume;
    for (auto &pair : sounds)
        pair.second.setVolume(volumes[pair.first] * generalVolume);
}

template <typename ResourceKey> void AudioManager<ResourceKey>::update()
{
    if (soundQueue.size() > 0)
    {
        if (soundQueue.front() != NULL && soundQueue.front()->getStatus() == sf::SoundSource::Status::Stopped)
        {
            soundQueue.pop();

            if(soundQueue.size() > 0)
            {
                if (soundQueue.front() != NULL &&
                    soundQueue.front()->getStatus() ==
                        sf::SoundSource::Status::Stopped)
                    soundQueue.front()->play();
            }

        }
    }
}
template <typename ResourceKey>
void AudioManager<ResourceKey>::remove(const ResourceKey &key)
{
    auto itr = sounds.find(key);
    if (itr != sound.end())
    {
        itr->second.stop();
        update();
        sounds.erase(key);
        volumes.erase(key);
    }
}
template <typename ResourceKey> void AudioManager<ResourceKey>::removeAll()
{
    while (soundQueue.size() > 0)
        soundQueue.pop();
    sounds.clear();
    volumes.clear();
}

}
