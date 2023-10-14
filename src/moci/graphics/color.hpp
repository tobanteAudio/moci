#pragma once

#include <glm/glm.hpp>

#include <array>

namespace moci
{
class ColorRGBA32
{
public:
    constexpr ColorRGBA32() noexcept = default;
    constexpr ColorRGBA32(float r, float g, float b) noexcept : _data {r, g, b, 1.0F} { }
    constexpr ColorRGBA32(float r, float g, float b, float alpha) noexcept : _data {r, g, b, alpha} { }

    [[nodiscard]] constexpr auto red() const noexcept -> float { return _data[0]; }
    [[nodiscard]] constexpr auto green() const noexcept -> float { return _data[1]; }
    [[nodiscard]] constexpr auto blue() const noexcept -> float { return _data[2]; }
    [[nodiscard]] constexpr auto alpha() const noexcept -> float { return _data[3]; }

    auto red(float r) noexcept -> void { _data[0] = r; }
    auto green(float g) noexcept -> void { _data[1] = g; }
    auto blue(float b) noexcept -> void { _data[2] = b; }
    auto alpha(float a) noexcept -> void { _data[3] = a; }

    explicit constexpr operator glm::vec4() const noexcept { return {_data[0], _data[1], _data[2], _data[3]}; }

private:
    std::array<float, 4> _data {0.0F, 0.0F, 0.0F, 0.0F};
};

class Colors
{
public:
    static constexpr ColorRGBA32 black = {0.0F, 0.0F, 0.0F, 1.0F};
    static constexpr ColorRGBA32 white = {1.0F, 1.0F, 1.0F, 1.0F};
    static constexpr ColorRGBA32 red   = {1.0F, 0.0F, 0.0F, 1.0F};
    static constexpr ColorRGBA32 green = {0.0F, 1.0F, 0.0F, 1.0F};
    static constexpr ColorRGBA32 blue  = {0.0F, 0.0F, 1.0F, 1.0F};
};
}  // namespace moci
