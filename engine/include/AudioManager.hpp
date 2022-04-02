#pragma once

#include <SFML/Audio.hpp>

#include <memory>
#include <unordered_map>
#include <queue>

namespace gjt
{

template<typename ResourceKey>
class AudioManager
{
  public:
    AudioManager();
    void createSound(
        const ResourceKey &key, const std::shared_ptr<sf::SoundBuffer> buffer);
    void setBuffer(
        const ResourceKey &key, const std::shared_ptr<sf::SoundBuffer> buffer);
    void setPitch(const ResourceKey &key, const float pitch);
    void setVolume(const ResourceKey &key, const float volume);
    
    sf::Time getOffset(const ResourceKey &key);
    void setOffset(const ResourceKey &key, const sf::Time &offset);

    void queue(const ResourceKey &key, const bool loop);
    void play(const ResourceKey &key);
    void stop(const ResourceKey &key);
    void pause(const ResourceKey &key);

    sf::SoundSource::Status getStatus(const ResourceKey &key);

    float getGeneralVolume() const;
    void setGeneralVolume(const float volume);

    void update();
    void remove(const ResourceKey &key);
    void removeAll();

private:
    std::unordered_map<ResourceKey, sf::Sound> sounds;
    std::unordered_map<ResourceKey, float> volumes;
    std::queue<sf::Sound const *> soundQueue;
    float generalVolume;
};

} // namespace gjt

#include "AudioManager.inl"

