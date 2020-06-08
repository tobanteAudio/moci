#pragma once
#include <array>

#include <vulkan/vulkan.h>

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

inline auto GetVertexBindingDescription() -> VkVertexInputBindingDescription
{
    auto result      = VkVertexInputBindingDescription {};
    result.binding   = 0;
    result.stride    = sizeof(Vertex);
    result.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return result;
}

inline auto GetVertexAttributeDescription() -> std::vector<VkVertexInputAttributeDescription>
{
    auto result = std::vector<VkVertexInputAttributeDescription> {2};

    result[0].location = 0;
    result[0].binding  = 0;
    result[0].format   = VK_FORMAT_R32G32_SFLOAT;
    result[0].offset   = offsetof(Vertex, position);

    result[1].location = 0;
    result[1].binding  = 0;
    result[1].format   = VK_FORMAT_R32G32B32_SFLOAT;
    result[1].offset   = offsetof(Vertex, color);

    return result;
}
}  // namespace th
