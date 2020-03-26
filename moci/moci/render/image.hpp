#pragma once

#include <gsl/gsl>

#include <cstdint>

#include "moci/core/vector.hpp"

namespace moci
{
class Image
{
public:
    explicit Image() = default;
    explicit Image(std::string const& path);

    bool LoadFromFile(std::string const& path);

    [[nodiscard]] bool Empty() const noexcept { return data_.empty(); }

    [[nodiscard]] int GetWidth() const noexcept { return width_; }
    [[nodiscard]] int GetHeight() const noexcept { return height_; }
    [[nodiscard]] int GetNumChannels() const noexcept { return numChannels_; }

    [[nodiscard]] gsl::span<std::uint8_t> Data() noexcept { return data_; }

private:
    Vector<std::uint8_t> data_ = {};
    int width_                 = 0;
    int height_                = 0;
    int numChannels_           = 0;
};
}  // namespace moci