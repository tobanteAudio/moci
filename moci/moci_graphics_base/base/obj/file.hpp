#pragma once

#include "glm/glm.hpp"

#include <gsl/gsl>

#include "moci_core/core/vector.hpp"
#include <string>

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
    static glm::vec3 parseLineToVec3(std::string& line);
    void parseLineToFace(std::string& line);

private:
    std::string path_;
    Vector<glm::vec3> vertices_;
    Vector<glm::vec3> normals_;
    Vector<glm::vec2> uvs_;
    Vector<VertexData> model_;
};
}  // namespace moci
