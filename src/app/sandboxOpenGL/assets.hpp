#pragma once

#include <array>
#include <cstdint>

#if defined(MOCI_API_OPENGL_LEGACY)
constexpr auto ShaderSuffix = "es2";
#else
constexpr auto ShaderSuffix = "gl4";
#endif

namespace assets {
inline auto quadVertices = std::array{
    -0.75F, -0.75F, 0.0F, 1.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F,  //
    0.75F,  -0.75F, 0.0F, 0.0F, 1.0F, 0.0F, 1.0F, 1.0F, 0.0F,  //
    0.75F,  0.75F,  0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F,  //
    -0.75F, 0.75F,  0.0F, 0.0F, 1.0F, 1.0F, 1.0F, 0.0F, 1.0F,  //
};
inline auto quadIndices = std::array{
    std::uint32_t{0},  //
    std::uint32_t{1},  //
    std::uint32_t{2},  //
    std::uint32_t{0},  //
    std::uint32_t{2},  //
    std::uint32_t{3},  //
};
}  // namespace assets
