#pragma once

#include <memory>
#include <type_traits>
#include <unordered_map>

namespace gjt
{
class ContentManager
{
  public:
    ~ContentManager()
    {
        _content.clear();
    }

    template <typename T>
    std::shared_ptr<T> loadFromFile(const std::string &path);

  private:
    std::unordered_map<
        size_t, std::unordered_map<std::string, std::shared_ptr<void>>>
        _content;
};
} // namespace gjt

#include "ContentManager.inl"
