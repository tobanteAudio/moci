#pragma once

#include "glm/glm.hpp"

#include <gsl/gsl>

#include <string>
#include <vector>

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

public:
    /**
     * @brief Constructor. Takes the file path.
     */
    OBJFile(std::string path);

    /**
     * @brief Parse the model data into memory.
     */
    bool Parse();

    /**
     * @brief Returns a span of the vertex positions.
     */
    [[nodiscard]] gsl::span<glm::vec3> GetVertices() { return vertices_; }

    /**
     * @brief Returns a span of the vertex normals.
     */
    [[nodiscard]] gsl::span<glm::vec3> GetNormals() { return normals_; }

    /**
     * @brief Returns a span of the faces.
     */
    [[nodiscard]] gsl::span<VertexData> GetVertexData() { return model_; }

private:
    glm::vec3 parseLineToVec3(std::string& line);
    void parseLineToFace(std::string& line);

private:
    std::string path_;
    std::vector<glm::vec3> vertices_;
    std::vector<glm::vec3> normals_;
    std::vector<glm::vec2> uvs_;
    std::vector<VertexData> model_;
};
}  // namespace moci
