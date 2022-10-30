#include "moci/core/logging.hpp"

#include "stb_image.h"
#include "stb_image_resize.h"
#include "stb_image_write.h"

int main(int, char**)
{
    moci::Log::Init();
    auto const path = "src/app/sandbox3D/assets/textures/cerberus_A_4096x4096.png";

    // Load
    stbi_uc* buffer = nullptr;
    int width       = 0;
    int height      = 0;
    int bpp         = 0;

    stbi_set_flip_vertically_on_load(1);
    buffer = stbi_load(path, &width, &height, &bpp, 0);
    if (buffer == nullptr) { MOCI_ERROR("stbi load error"); }

    MOCI_INFO("Width: {}", width);
    MOCI_INFO("Height: {}", height);
    MOCI_INFO("BPP: {}", bpp);

    // Resize
    auto const newSize = 512;
    moci::Vector<stbi_uc> outBuffer {};
    outBuffer.resize(newSize * newSize * bpp);
    if (stbir_resize_uint8(buffer, width, height, 0, outBuffer.data(), newSize, newSize, 0, bpp) == 0)
    {
        MOCI_ERROR("stbi resize error");
    }

    // Write
    auto const outPath = "test.png";
    if (stbi_write_png(outPath, newSize, newSize, bpp, outBuffer.data(), newSize * bpp) == 0)
    {
        MOCI_ERROR("stbi write error");
    }

    // Cleanup
    if (buffer != nullptr) { stbi_image_free(buffer); }

    return EXIT_SUCCESS;
}