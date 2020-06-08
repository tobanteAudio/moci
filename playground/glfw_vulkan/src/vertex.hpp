#pragma once
#include <array>

#include "glm/glm.hpp"

namespace th
{

struct Vertex
{
    glm::vec2 position = {};
    glm::vec3 color    = {};
};

constexpr auto Triangle = std::array {
    Vertex {{0.0f, -0.5f}, {0.8f, 0.2f, 0.01}},
    Vertex {{0.5f, 0.5f}, {0.0f, 0.95f, 0.0f}},
    Vertex {{-0.5f, 0.5f}, {0.0f, 0.0f, 0.95f}},
};

}  // namespace th
