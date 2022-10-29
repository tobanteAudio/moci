#pragma once

#include <string>

#include "glm/glm.hpp"

#include "moci_core/moci_core.hpp"

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
    bool Parse();

    /**
     * @brief Returns a span of the vertex positions.
     */
    [[nodiscard]] Span<glm::vec3> GetVertices() { return vertices_; }

    /**
     * @brief Returns a span of the vertex normals.
     */
    [[nodiscard]] Span<glm::vec3> GetNormals() { return normals_; }

    /**
     * @brief Returns a span of the faces.
     */
    [[nodiscard]] Span<VertexData> GetVertexData() { return model_; }

private:
    static glm::vec3 parseLineToVec3(std::string& line);
    void parseLineToFace(std::string& line);

    std::string path_;
    Vector<glm::vec3> vertices_;
    Vector<glm::vec3> normals_;
    Vector<glm::vec2> uvs_;
    Vector<VertexData> model_;
};
}  // namespace moci
