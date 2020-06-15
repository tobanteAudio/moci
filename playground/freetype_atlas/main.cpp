#include "moci/moci.hpp"

#include "stb_image_write.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <cmath>
#include <cstdio>

constexpr auto NUM_GLYPHS = 128;

struct glyph_info
{
    int x0, y0, x1, y1;  // coords of glyph in the texture atlas
    int x_off, y_off;    // left & top bearing when rendering
    int advance;         // x advance when rendering
};

int main(int argc, char** argv)
{
    auto const arguments = moci::Span<char*>(argv, argc);
    if (argc < 3)
    {
        printf("usage: %s <font> <size>\n", arguments[0]);
        return 1;
    }

    FT_Library ft;
    FT_Face face;

    FT_Init_FreeType(&ft);
    FT_New_Face(ft, arguments[1], 0, &face);
    FT_Set_Char_Size(face, 0, atoi(arguments[2]) << 6, 96, 96);

    // quick and dirty max texture size estimate

    int max_dim   = (1 + (face->size->metrics.height >> 6)) * static_cast<int>(ceilf(sqrtf(NUM_GLYPHS)));
    int tex_width = 1;
    while (tex_width < max_dim)
    {
        tex_width <<= 1;
    }
    int tex_height = tex_width;

    // render glyphs to atlas

    char* pixels = (char*)calloc(tex_width * tex_height, 1);
    int pen_x    = 0;
    int pen_y    = 0;

    std::array<glyph_info, NUM_GLYPHS> info {};

    auto index = 0;
    for (auto& glyph : info)
    {
        FT_Load_Char(face, index, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT);
        index++;

        FT_Bitmap* bmp = &face->glyph->bitmap;

        if (pen_x + static_cast<int>(bmp->width) >= tex_width)
        {
            pen_x = 0;
            pen_y += ((face->size->metrics.height >> 6) + 1);
        }

        for (int row = 0; row < static_cast<int>(bmp->rows); ++row)
        {
            for (int col = 0; col < static_cast<int>(bmp->width); ++col)
            {
                int x                     = pen_x + col;
                int y                     = pen_y + row;
                pixels[y * tex_width + x] = bmp->buffer[row * bmp->pitch + col];
            }
        }

        // this is stuff you'd need when rendering individual glyphs out of the atlas

        glyph.x0 = pen_x;
        glyph.y0 = pen_y;
        glyph.x1 = pen_x + bmp->width;
        glyph.y1 = pen_y + bmp->rows;

        glyph.x_off   = face->glyph->bitmap_left;
        glyph.y_off   = face->glyph->bitmap_top;
        glyph.advance = face->glyph->advance.x >> 6;

        pen_x += bmp->width + 1;
    }

    FT_Done_FreeType(ft);

    // write png

    char* png_data = (char*)calloc(tex_width * tex_height * 4, 1);
    for (int i = 0; i < (tex_width * tex_height); ++i)
    {
        png_data[i * 4 + 0] |= pixels[i];
        png_data[i * 4 + 1] |= pixels[i];
        png_data[i * 4 + 2] |= pixels[i];
        png_data[i * 4 + 3] = 0xff;
    }

    stbi_write_png("font_output.png", tex_width, tex_height, 4, png_data, tex_width * 4);

    free(png_data);
    free(pixels);

    return 0;
}