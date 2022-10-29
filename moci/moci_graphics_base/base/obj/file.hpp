#pragma once

#include <string>

#include "glm/glm.hpp"

#include "moci/core/logging.hpp"
#include "moci/core/span.hpp"

namespace moci
{
/**
 * @brief Simple Wavefront OBJ file loader.
 */
class OBJFile
{
public:
    struct VertexData
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
    };

    /**
     * @brief Constructor. Takes the file path.
     */
    explicit OBJFile(std::string path);

    /**
     * @brief Parse the model data into memory.
     */
    auto Parse() -> bool;

    /**
     * @brief Returns a span of the vertex positions.
     */
    [[nodiscard]] auto GetVertices() -> Span<glm::vec3> { return vertices_; }

    /**
     * @brief Returns a span of the vertex normals.
     */
    [[nodiscard]] auto GetNormals() -> Span<glm::vec3> { return normals_; }

    /**
     * @brief Returns a span of the faces.
     */
    [[nodiscard]] auto GetVertexData() -> Span<VertexData> { return model_; }

private:
    static auto parseLineToVec3(std::string& line) -> glm::vec3;
    void parseLineToFace(std::string& line);

    std::string path_;
    Vector<glm::vec3> vertices_;
    Vector<glm::vec3> normals_;
    Vector<glm::vec2> uvs_;
    Vector<VertexData> model_;
};
}  // namespace moci
