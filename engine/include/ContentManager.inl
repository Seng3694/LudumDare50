#pragma once

#include "ContentManager.hpp"

namespace gjt
{
template <typename T> 
std::shared_ptr<T> ContentManager::loadFromFile(const std::string &path)
{
    const size_t hash = typeid(T).hash_code();
    auto typeItr = _content.find(hash);

    if (typeItr == _content.end())
    {
        _content.emplace(
            hash, std::unordered_map<std::string, std::shared_ptr<void>>());
        typeItr = _content.find(hash);
    }

    auto pathItr = typeItr->second.find(path);
    if (pathItr == typeItr->second.end())
    {
        auto content = std::shared_ptr<T>(new T());
        if (!content->loadFromFile(path))
            return nullptr;
        typeItr->second.emplace(
            path, std::static_pointer_cast<void, T>(content));
        pathItr = typeItr->second.find(path);
    }

    return std::static_pointer_cast<T, void>(pathItr->second);
}
}
