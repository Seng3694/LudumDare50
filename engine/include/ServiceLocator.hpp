#pragma once

#include <unordered_map>
#include <memory>

namespace gjt
{
class ServiceLocator
{
  public:
    template <typename T>
    void registerInstance(std::shared_ptr<T> instance = std::make_shared<T>());
    template <typename T> 
    std::shared_ptr<T> resolve();

  private:
    std::unordered_map<size_t, std::shared_ptr<void>> services;
};
} // namespace gjt

#include "ServiceLocator.inl"
