#pragma once

#include <array>
#include <cstdint>

#if defined(MOCI_API_OPENGL_LEGACY)
constexpr auto shaderSuffix = "es2";
#else
constexpr auto shaderSuffix = "gl4";
#endif

namespace assets
{
inline auto QuadVertices = std::array {
    -0.75f, -0.75f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  //
    0.75f,  -0.75f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,  //
    0.75f,  0.75f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,  //
    -0.75f, 0.75f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  //
};
inline auto QuadIndices = std::array {
    std::uint32_t {0},  //
    std::uint32_t {1},  //
    std::uint32_t {2},  //
    std::uint32_t {0},  //
    std::uint32_t {2},  //
    std::uint32_t {3},  //
};
}  // namespace assets