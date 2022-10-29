#pragma once

#include "moci/core/vector.hpp"
#include "moci/math/geometry/point.hpp"

#include <cstdint>
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

    explicit Typeface(std::string name);
    void AddCharacter(char c, std::uint32_t height, std::uint32_t width, Point<std::uint32_t> topLeft,
                      std::uint32_t advance, Buffer buffer);
    void AddCharacter(char c, Character const& character);

    [[nodiscard]] auto GetName() const noexcept -> std::string_view { return std::string_view(name_); }
    [[nodiscard]] auto GetNumCharacters() const noexcept -> std::size_t { return characters_.size(); }
    [[nodiscard]] auto GetCharacter(char c) const noexcept -> Character const& { return (*characters_.find(c)).second; }

    [[nodiscard]] auto GetWidthForString(std::string const& text, float scale = 1.0F) -> std::uint32_t;

private:
    std::string name_ {};
    std::map<char, Character> characters_ {};
};

class Font
{
public:
    explicit Font(float height);
    Font(std::string typefaceName, float height);
};
}  // namespace moci