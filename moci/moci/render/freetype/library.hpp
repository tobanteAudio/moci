#pragma once

#include "moci/render/font.hpp"

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
    static std::optional<Typeface> CreateTypefaceFromFont(std::string fontPath);

private:
};
}  // namespace moci