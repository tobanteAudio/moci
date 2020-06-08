#pragma once

#include "moci/geometry/point.hpp"

#include <cstdint>

#include "moci_core/core/vector.hpp"
#include <map>
#include <string>
#include <string_view>

namespace moci
{

class Typeface
{
public:
    using Buffer = Vector<std::uint8_t>;

    struct Character
    {
        std::uint32_t width {};
        std::uint32_t height {};
        Point<std::uint32_t> topLeft {};
        std::uint32_t advance {};
        Buffer buffer {};
    };

public:
    Typeface(std::string name);
    void AddCharacter(char c, std::uint32_t height, std::uint32_t width, Point<std::uint32_t> topLeft,
                      std::uint32_t advance, Buffer buffer);
    void AddCharacter(char c, Character const& character);

    [[nodiscard]] std::string_view GetName() const noexcept { return std::string_view(name_); }
    [[nodiscard]] std::size_t GetNumCharacters() const noexcept { return characters_.size(); }
    [[nodiscard]] Character const& GetCharacter(char c) const noexcept { return (*characters_.find(c)).second; }

    [[nodiscard]] std::uint32_t GetWidthForString(std::string const& text, float scale = 1.0f);

private:
    std::string name_ {};
    std::map<char, Character> characters_ {};
};

class Font
{
public:
    Font(float height);
    Font(std::string typefaceName, float height);
};
}  // namespace moci