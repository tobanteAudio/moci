#include "image.hpp"

#include <cstddef>

#include <moci/core/logging.hpp>

#include "stb_image.h"
#include "stb_image_resize.h"

namespace moci
{
Image::Image(std::string const& path) { loadFromFile(path); }

auto Image::loadFromFile(std::string const& path) -> bool
{
    auto* data = stbi_load(path.c_str(), &_width, &_height, &_numChannels, 0);
    if (data == nullptr)
    {
        MOCI_CORE_ERROR("Image loading: {}", path);
        stbi_image_free(data);
        return false;
    }

    auto* start = reinterpret_cast<std::uint8_t*>(data);
    auto* end   = reinterpret_cast<std::uint8_t*>(data) + static_cast<ptrdiff_t>(_width * _height * _numChannels);
    _data       = Vector<std::uint8_t>(start, end);

    stbi_image_free(data);

    return true;
}
}  // namespace moci
