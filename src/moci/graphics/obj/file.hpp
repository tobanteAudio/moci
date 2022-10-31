#pragma once

#include <moci/core/logging.hpp>
#include <moci/core/span.hpp>
#include <moci/core/vector.hpp>

#include "glm/glm.hpp"

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
    [[nodiscard]] auto getVertices() -> Span<glm::vec3> { return _vertices; }

    /**
     * @brief Returns a span of the vertex normals.
     */
    [[nodiscard]] auto getNormals() -> Span<glm::vec3> { return _normals; }

    /**
     * @brief Returns a span of the faces.
     */
    [[nodiscard]] auto getVertexData() -> Span<VertexData> { return _model; }

private:
    static auto parseLineToVec3(std::string& line) -> glm::vec3;
    void parseLineToFace(std::string& line);

    std::string _path;
    Vector<glm::vec3> _vertices;
    Vector<glm::vec3> _normals;
    Vector<glm::vec2> _uvs;
    Vector<VertexData> _model;
};
}  // namespace moci
