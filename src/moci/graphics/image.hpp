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

    [[nodiscard]] auto empty() const noexcept -> bool { return _data.empty(); }

    [[nodiscard]] auto getWidth() const noexcept -> int { return _width; }
    [[nodiscard]] auto getHeight() const noexcept -> int { return _height; }
    [[nodiscard]] auto getNumChannels() const noexcept -> int { return _numChannels; }

    [[nodiscard]] auto data() noexcept -> Span<std::uint8_t> { return _data; }

private:
    Vector<std::uint8_t> _data = {};
    int _width                 = 0;
    int _height                = 0;
    int _numChannels           = 0;
};
}  // namespace moci