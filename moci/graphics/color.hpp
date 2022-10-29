#pragma once

#include <glm/glm.hpp>

namespace moci
{
class Color
{
public:
    constexpr Color() noexcept : data_({0.0F, 0.0F, 0.0F, 0.0F}) { }
    constexpr Color(float r, float g, float b) noexcept : data_({r, g, b, 1.0F}) { }
    constexpr Color(float r, float g, float b, float alpha) noexcept : data_({r, g, b, alpha}) { }

    [[nodiscard]] constexpr auto GetData() const noexcept -> glm::vec4 { return data_; }

    [[nodiscard]] constexpr auto GetRed() const noexcept -> float { return data_.r; }
    [[nodiscard]] constexpr auto GetGreen() const noexcept -> float { return data_.g; }
    [[nodiscard]] constexpr auto GetBlue() const noexcept -> float { return data_.b; }
    [[nodiscard]] constexpr auto GetAlpha() const noexcept -> float { return data_.a; }

    auto SetRed(float r) noexcept -> void { data_.r = r; }
    auto SetGreen(float g) noexcept -> void { data_.g = g; }
    auto SetBlue(float b) noexcept -> void { data_.b = b; }
    auto SetAlpha(float a) noexcept -> void { data_.a = a; }

private:
    glm::vec4 data_ {};
};

class Colors
{
public:
    static constexpr Color Black = {0.0F, 0.0F, 0.0F, 1.0F};
    static constexpr Color White = {1.0F, 1.0F, 1.0F, 1.0F};
    static constexpr Color Red   = {1.0F, 0.0F, 0.0F, 1.0F};
    static constexpr Color Green = {0.0F, 1.0F, 0.0F, 1.0F};
    static constexpr Color Blue  = {0.0F, 0.0F, 1.0F, 1.0F};
};
}  // namespace moci