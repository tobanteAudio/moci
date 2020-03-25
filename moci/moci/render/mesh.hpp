#pragma once

#include "moci/core/logging.hpp"
#include "moci/core/memory.hpp"
#include "moci/render/color.hpp"

#include "glm/glm.hpp"

#include <gsl/gsl>

#include "assimp/Importer.hpp"

namespace moci
{

class Mesh
{
public:
    struct Vertex
    {
        glm::vec3 position {};
        glm::vec3 normal {};
        glm::vec4 color {};
        glm::vec2 texCoord {};
    };

    struct Index
    {
        uint32_t V1, V2, V3;
    };

    class Submesh
    {
    public:
        uint32_t BaseVertex;
        uint32_t BaseIndex;
        uint32_t MaterialIndex;
        uint32_t IndexCount;

        glm::mat4 Transform;
    };

public:
    Mesh(std::string filePath);
    gsl::span<Vertex const> GetVertices() const noexcept { return staticVertices_; }

private:
    std::string filePath_ {};
    moci::Scope<Assimp::Importer> importer_ {};
    glm::mat4 inverseTransform_ {};
    std::vector<Submesh> submeshes_ {};
    std::vector<Vertex> staticVertices_ {};
    bool isAnimated_ = false;
    std::vector<Index> indices_ {};
};
}  // namespace moci