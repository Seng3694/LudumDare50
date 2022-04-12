#pragma once

#include <memory>
#include <type_traits>
#include <unordered_map>

namespace gjt
{

template<typename ContentKey>
class ContentManager
{
  public:
    ~ContentManager()
    {
        _content.clear();
    }

    template <typename T> 
    bool loadFromFile(const std::string &path, const ContentKey &key)
    {
        const size_t hash = typeid(T).hash_code();
        auto typeItr = _content.find(hash);

        if (typeItr == _content.end())
        {
            _content.emplace(
                hash, std::unordered_map<ContentKey, std::shared_ptr<void>>());
            typeItr = _content.find(hash);
        }

        auto keyItr = typeItr->second.find(key);
        if (keyItr == typeItr->second.end())
        {
            auto content = std::shared_ptr<T>(new T());
            if (!content->loadFromFile(path))
                return false;

            typeItr->second.emplace(
                key, std::static_pointer_cast<void, T>(content));
        }

        return true;
    }

    template <typename T> 
    std::shared_ptr<T> get(const ContentKey &key)
    {
        const size_t hash = typeid(T).hash_code();
        auto typeItr = _content.find(hash);
        if (typeItr != _content.end())
        {
            auto keyItr = typeItr->second.find(key);
            if (keyItr != typeItr->second.end())
            {
                return std::static_pointer_cast<T, void>(keyItr->second);
            }
        }

        return nullptr;
    }

    template <typename T>
    void addFromMemory(
        const ContentKey &key, std::shared_ptr<T> data)
    {
        const size_t hash = typeid(T).hash_code();
        auto typeItr = _content.find(hash);

        if (typeItr == _content.end())
        {
            _content.emplace(
                hash,
                std::unordered_map<ContentKey, std::shared_ptr<void>>());
            typeItr = _content.find(hash);
        }

        auto keyItr = typeItr->second.find(key);
        if (keyItr == typeItr->second.end())
        {
            typeItr->second.emplace(
                key, std::static_pointer_cast<void, T>(data));
        }
    }

  private:
    std::unordered_map<
        size_t, std::unordered_map<ContentKey, std::shared_ptr<void>>>
        _content;
};
} // namespace gjt

