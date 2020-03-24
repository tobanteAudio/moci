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
    Mesh(const std::string& filename);
    gsl::span<Vertex const> GetVertices() const noexcept { return m_StaticVertices; }

private:
    std::string m_FilePath {};
    moci::Scope<Assimp::Importer> m_Importer {};
    glm::mat4 m_InverseTransform {};
    std::vector<Submesh> m_Submeshes {};
    std::vector<Vertex> m_StaticVertices {};
    bool m_IsAnimated = false;
    std::vector<Index> m_Indices {};
};
}  // namespace moci