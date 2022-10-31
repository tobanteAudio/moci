#pragma once

#include <moci/core/logging.hpp>
#include <moci/core/span.hpp>
#include <moci/core/vector.hpp>
#include <moci/graphics/color.hpp>

#include "glm/glm.hpp"

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

    explicit Mesh(std::string filePath);
    [[nodiscard]] auto getVertices() const noexcept -> Span<Vertex const> { return _staticVertices; }

private:
    std::string _filePath {};
    moci::Scope<Assimp::Importer> _importer {};
    glm::mat4 _inverseTransform {};
    Vector<Submesh> _submeshes {};
    Vector<Vertex> _staticVertices {};
    bool _isAnimated = false;
    Vector<Index> _indices {};
};
}  // namespace moci