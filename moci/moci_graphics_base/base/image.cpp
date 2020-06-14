#include "image.hpp"

#include "moci_core/core/logging.hpp"

#include "stb_image.h"
#include "stb_image_resize.h"

namespace moci
{
Image::Image(std::string const& path) { LoadFromFile(path); }

bool Image::LoadFromFile(std::string const& path)
{
    auto* data = stbi_load(path.c_str(), &width_, &height_, &numChannels_, 0);
    if (data == nullptr)
    {
        MOCI_CORE_ERROR("Image loading: {}", path);
        stbi_image_free(data);
        return false;
    }

    auto* start = reinterpret_cast<std::uint8_t*>(data);
    auto* end   = reinterpret_cast<std::uint8_t*>(data) + width_ * height_ * numChannels_;
    data_       = Vector<std::uint8_t>(start, end);

    stbi_image_free(data);

    return true;
}
}  // namespace moci