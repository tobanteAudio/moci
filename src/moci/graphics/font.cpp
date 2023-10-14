#include "font.hpp"

#include <moci/core/geometry/point.hpp>

#include <utility>

namespace moci {
Typeface::Typeface(std::string name) : _name(std::move(name)) {}

void Typeface::addCharacter(
    char c,
    std::uint32_t height,
    std::uint32_t width,
    Point<std::uint32_t> topLeft,
    std::uint32_t advance,
    Buffer buffer
)
{
    addCharacter(c, {height, width, topLeft, advance, std::move(buffer)});
}

void Typeface::addCharacter(char c, Character const& character)
{
    _characters.insert(std::pair<char, Character>(c, character));
}

auto Typeface::getWidthForString(std::string const& text, float scale) -> std::uint32_t
{
    auto position = Point<float>{};
    for (auto const c : text) {
        auto const& ch = _characters[c];

        // float const xpos = position.getX() + ch.topLeft.getX() * scale;
        // float const ypos = position.getY() - (ch.width - ch.topLeft.getY()) * scale;
        // float const w    = ch.width * scale;
        // float const h    = ch.height * scale;

        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        position.setX(position.getX() + ((ch.advance >> 6) * scale));
    }

    return static_cast<std::uint32_t>(position.getX());
}

}  // namespace moci
