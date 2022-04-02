#pragma once

#include "ServiceLocator.hpp"

#include <type_traits>

namespace gjt
{
template <typename T>
void ServiceLocator::registerInstance(std::shared_ptr<T> instance)
{
    const size_t hash = typeid(T).hash_code();
    auto result = services.find(hash);
    if (result != services.end())
    {
        result->second = std::static_pointer_cast<void, T>(instance);
    }
    else
    {
        services.emplace(hash, std::static_pointer_cast<void, T>(instance));
    }
}

template <typename T> std::shared_ptr<T> ServiceLocator::resolve()
{
    const size_t hash = typeid(T).hash_code();
    auto result = services.find(hash);
    if (result != services.end())
    {
        return std::static_pointer_cast<T, void>(result->second);
    }
    else
    {
        return nullptr;
    }
}
}
