#include "image.hpp"

#include <cstddef>

#include "moci/core/logging.hpp"

#include "stb_image.h"
#include "stb_image_resize.h"

namespace moci
{
Image::Image(std::string const& path) { loadFromFile(path); }

auto Image::loadFromFile(std::string const& path) -> bool
{
    auto* data = stbi_load(path.c_str(), &width_, &height_, &numChannels_, 0);
    if (data == nullptr)
    {
        MOCI_CORE_ERROR("Image loading: {}", path);
        stbi_image_free(data);
        return false;
    }

    auto* start = reinterpret_cast<std::uint8_t*>(data);
    auto* end   = reinterpret_cast<std::uint8_t*>(data) + static_cast<ptrdiff_t>(width_ * height_ * numChannels_);
    data_       = Vector<std::uint8_t>(start, end);

    stbi_image_free(data);

    return true;
}
}  // namespace moci
