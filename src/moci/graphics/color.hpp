#pragma once

#include <glm/glm.hpp>

namespace moci
{
class Color
{
public:
    constexpr Color() noexcept : _data({0.0F, 0.0F, 0.0F, 0.0F}) { }
    constexpr Color(float r, float g, float b) noexcept : _data({r, g, b, 1.0F}) { }
    constexpr Color(float r, float g, float b, float alpha) noexcept : _data({r, g, b, alpha}) { }

    [[nodiscard]] constexpr auto getData() const noexcept -> glm::vec4 { return _data; }

    [[nodiscard]] constexpr auto getRed() const noexcept -> float { return _data.r; }
    [[nodiscard]] constexpr auto getGreen() const noexcept -> float { return _data.g; }
    [[nodiscard]] constexpr auto getBlue() const noexcept -> float { return _data.b; }
    [[nodiscard]] constexpr auto getAlpha() const noexcept -> float { return _data.a; }

    auto setRed(float r) noexcept -> void { _data.r = r; }
    auto setGreen(float g) noexcept -> void { _data.g = g; }
    auto setBlue(float b) noexcept -> void { _data.b = b; }
    auto setAlpha(float a) noexcept -> void { _data.a = a; }

private:
    glm::vec4 _data {};
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