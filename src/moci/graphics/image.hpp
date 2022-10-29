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

    auto loadFromFile(std::string const& path) -> bool;

    [[nodiscard]] auto empty() const noexcept -> bool { return data_.empty(); }

    [[nodiscard]] auto getWidth() const noexcept -> int { return width_; }
    [[nodiscard]] auto getHeight() const noexcept -> int { return height_; }
    [[nodiscard]] auto getNumChannels() const noexcept -> int { return numChannels_; }

    [[nodiscard]] auto data() noexcept -> Span<std::uint8_t> { return data_; }

private:
    Vector<std::uint8_t> data_ = {};
    int width_                 = 0;
    int height_                = 0;
    int numChannels_           = 0;
};
}  // namespace moci