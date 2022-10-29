#pragma once

#include "moci/graphics/font.hpp"

#include <optional>

namespace moci
{

/**
 * @brief Wrapper around freetype.
 */
class FreetypeLibrary
{
public:
    /**
     * @brief Static class.
     */
    FreetypeLibrary() = delete;

    /**
     * @brief Create a new typeface from a font file.
     */
    static auto createTypefaceFromFont(const std::string& fontPath) -> std::optional<Typeface>;

private:
};
}  // namespace moci