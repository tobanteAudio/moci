#pragma once

#include <cstdint>

#include "moci/core/logging.hpp"
#include "moci/core/span.hpp"
#include "moci/core/vector.hpp"

namespace moci
{
class Image
{
public:
    explicit Image() = default;
    explicit Image(std::string const& path);

    auto LoadFromFile(std::string const& path) -> bool;

    [[nodiscard]] auto Empty() const noexcept -> bool { return data_.empty(); }

    [[nodiscard]] auto GetWidth() const noexcept -> int { return width_; }
    [[nodiscard]] auto GetHeight() const noexcept -> int { return height_; }
    [[nodiscard]] auto GetNumChannels() const noexcept -> int { return numChannels_; }

    [[nodiscard]] auto Data() noexcept -> Span<std::uint8_t> { return data_; }

private:
    Vector<std::uint8_t> data_ = {};
    int width_                 = 0;
    int height_                = 0;
    int numChannels_           = 0;
};
}  // namespace moci