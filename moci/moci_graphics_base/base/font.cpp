#include "font.hpp"

#include "moci_math/geometry/point.hpp"

#include <utility>

namespace moci
{
Typeface::Typeface(std::string name) : name_(std::move(name)) { }

void Typeface::AddCharacter(char c, std::uint32_t height, std::uint32_t width, Point<std::uint32_t> topLeft,
                            std::uint32_t advance, Buffer buffer)
{
    AddCharacter(c, {height, width, topLeft, advance, std::move(buffer)});
}

void Typeface::AddCharacter(char c, Character const& character)
{
    characters_.insert(std::pair<char, Character>(c, character));
}

auto Typeface::GetWidthForString(std::string const& text, float scale) -> std::uint32_t
{
    auto position = Point<float> {};
    for (auto const c : text)
    {
        auto const& ch = characters_[c];

        // float const xpos = position.GetX() + ch.topLeft.GetX() * scale;
        // float const ypos = position.GetY() - (ch.width - ch.topLeft.GetY()) * scale;
        // float const w    = ch.width * scale;
        // float const h    = ch.height * scale;

        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        position.SetX(position.GetX() + ((ch.advance >> 6) * scale));
    }

    return static_cast<std::uint32_t>(position.GetX());
}

}  // namespace moci