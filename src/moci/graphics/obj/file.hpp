#pragma once

#include <moci/core/logging.hpp>

#include "glm/glm.hpp"

#include <span>
#include <string>
#include <vector>

namespace moci {
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
    auto parse() -> bool;

    /**
     * @brief Returns a span of the vertex positions.
     */
    [[nodiscard]] auto getVertices() -> std::span<glm::vec3> { return _vertices; }

    /**
     * @brief Returns a span of the vertex normals.
     */
    [[nodiscard]] auto getNormals() -> std::span<glm::vec3> { return _normals; }

    /**
     * @brief Returns a span of the faces.
     */
    [[nodiscard]] auto getVertexData() -> std::span<VertexData> { return _model; }

private:
    static auto parseLineToVec3(std::string& line) -> glm::vec3;
    void parseLineToFace(std::string& line);

    std::string _path;
    std::vector<glm::vec3> _vertices;
    std::vector<glm::vec3> _normals;
    std::vector<glm::vec2> _uvs;
    std::vector<VertexData> _model;
};
}  // namespace moci
