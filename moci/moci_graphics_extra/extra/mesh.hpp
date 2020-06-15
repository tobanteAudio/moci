#pragma once

#include "moci_core/moci_core.hpp"
#include "moci_graphics_base/moci_graphics_base.hpp"

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
    Span<Vertex const> GetVertices() const noexcept { return staticVertices_; }

private:
    std::string filePath_ {};
    moci::Scope<Assimp::Importer> importer_ {};
    glm::mat4 inverseTransform_ {};
    Vector<Submesh> submeshes_ {};
    Vector<Vertex> staticVertices_ {};
    bool isAnimated_ = false;
    Vector<Index> indices_ {};
};
}  // namespace moci