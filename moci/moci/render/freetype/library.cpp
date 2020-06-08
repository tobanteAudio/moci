#include "moci/render/freetype/library.hpp"

#include "moci_core/core/scope_guard.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace moci
{
std::optional<Typeface> FreetypeLibrary::CreateTypefaceFromFont(std::string fontPath)
{
    if (fontPath.empty())
    {
        return std::nullopt;
    }

    FT_Library ft = nullptr;
    if (FT_Init_FreeType(&ft) != 0)
    {
        return std::nullopt;
    }
    auto closeLibrary = moci::MakeScopeGuard([&ft]() { FT_Done_FreeType(ft); });

    FT_Face face = nullptr;
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face) != 0)
    {
        return std::nullopt;
    }
    auto closeFace = moci::MakeScopeGuard([&face]() { FT_Done_Face(face); });

    FT_Set_Pixel_Sizes(face, 0, 48);
    Typeface result {fontPath};
    for (std::uint8_t c = 0; c < 128; c++)
    {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER) != 0)
        {
            continue;
        }

        auto buffer = Vector<std::uint8_t>(
            face->glyph->bitmap.buffer,                                                          //
            face->glyph->bitmap.buffer + (face->glyph->bitmap.width * face->glyph->bitmap.rows)  //
        );

        result.AddCharacter(                                        //
            c,                                                      //
            static_cast<std::uint32_t>(face->glyph->bitmap.width),  //
            static_cast<std::uint32_t>(face->glyph->bitmap.rows),   //
            {static_cast<std::uint32_t>(face->glyph->bitmap_left),  //
             static_cast<std::uint32_t>(face->glyph->bitmap_top)},  //
            static_cast<std::uint32_t>(face->glyph->advance.x),     //
            buffer                                                  //
        );
    }

    return result;
}
}  // namespace moci