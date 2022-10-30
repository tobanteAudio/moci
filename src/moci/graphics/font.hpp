#pragma once

#include "moci/core/geometry/point.hpp"
#include "moci/core/vector.hpp"


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
    void addCharacter(char c, std::uint32_t height, std::uint32_t width, Point<std::uint32_t> topLeft,
                      std::uint32_t advance, Buffer buffer);
    void addCharacter(char c, Character const& character);

    [[nodiscard]] auto getName() const noexcept -> std::string_view { return std::string_view(name_); }
    [[nodiscard]] auto getNumCharacters() const noexcept -> std::size_t { return characters_.size(); }
    [[nodiscard]] auto getCharacter(char c) const noexcept -> Character const& { return (*characters_.find(c)).second; }

    [[nodiscard]] auto getWidthForString(std::string const& text, float scale = 1.0F) -> std::uint32_t;

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