#pragma once

#include <memory>
#include <utility>

namespace moci
{
template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T, typename... Args>
constexpr auto makeScope(Args&&... args) -> Scope<T>
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T, typename... Args>
constexpr auto makeRef(Args&&... args) -> Ref<T>
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}
}  // namespace moci