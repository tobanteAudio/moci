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

    [[nodiscard]] constexpr auto getData() const noexcept -> glm::vec4 { return data_; }

    [[nodiscard]] constexpr auto getRed() const noexcept -> float { return data_.r; }
    [[nodiscard]] constexpr auto getGreen() const noexcept -> float { return data_.g; }
    [[nodiscard]] constexpr auto getBlue() const noexcept -> float { return data_.b; }
    [[nodiscard]] constexpr auto getAlpha() const noexcept -> float { return data_.a; }

    auto setRed(float r) noexcept -> void { data_.r = r; }
    auto setGreen(float g) noexcept -> void { data_.g = g; }
    auto setBlue(float b) noexcept -> void { data_.b = b; }
    auto setAlpha(float a) noexcept -> void { data_.a = a; }

private:
    glm::vec4 data_ {};
};

class Colors
{
public:
    static constexpr Color black = {0.0F, 0.0F, 0.0F, 1.0F};
    static constexpr Color white = {1.0F, 1.0F, 1.0F, 1.0F};
    static constexpr Color red   = {1.0F, 0.0F, 0.0F, 1.0F};
    static constexpr Color green = {0.0F, 1.0F, 0.0F, 1.0F};
    static constexpr Color blue  = {0.0F, 0.0F, 1.0F, 1.0F};
};
}  // namespace moci