#pragma once

#include <moci/core/logging.hpp>
#include <moci/core/span.hpp>
#include <moci/graphics/color.hpp>

#include "glm/glm.hpp"

#include "assimp/Importer.hpp"

#include <vector>

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
    std::unique_ptr<Assimp::Importer> _importer {};
    glm::mat4 _inverseTransform {};
    std::vector<Submesh> _submeshes {};
    std::vector<Vertex> _staticVertices {};
    bool _isAnimated = false;
    std::vector<Index> _indices {};
};

}  // namespace moci