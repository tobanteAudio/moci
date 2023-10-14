#pragma once

#include "mesh.hpp"

#include <moci/render/api.hpp>
#include <moci/render/render_command.hpp>
#include <moci/render/render_factory.hpp>
#include <moci/render/shader.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"

#include "glm/gtx/quaternion.hpp"

namespace moci
{
struct Light
{
    struct Vertex
    {
        glm::vec3 position = {};
        glm::vec4 color    = {};
    };

    glm::vec3 position = {8.4F, 4.0F, 3.0F};
    glm::vec4 color    = {1.0F, 1.0F, 1.0F, 1.0F};
    float scale        = 0.5F;

    std::shared_ptr<Shader> shader    = nullptr;
    std::shared_ptr<VertexBuffer> vbo = nullptr;
    std::shared_ptr<IndexBuffer> ibo  = nullptr;
    std::shared_ptr<VertexArray> vao  = nullptr;
    Mesh lightMesh_ {"src/app/sandbox3D/assets/models/sphere.obj"};
    std::vector<Light::Vertex> vertices = {};

public:
    Light()
    {
#if defined(MOCI_API_OPENGL_LEGACY)
        shader = RenderFactory::makeShader("src/app/sandbox3D/assets/shader/es2_light_source.glsl");
#else
        shader = RenderFactory::makeShader("src/app/sandbox3D/assets/shader/gl4_light_source.glsl");
#endif
        shader->bind();
        BufferLayout lightLayout = {
            {ShaderDataType::Float3, "a_Position"},  //
            {ShaderDataType::Float4, "a_Color"},     //
        };

        auto const size = static_cast<uint32_t>(lightMesh_.getVertices().size() * sizeof(Light::Vertex));
        vbo.reset(RenderFactory::makeVertexBuffer(nullptr, size, true));
        vbo->setLayout(lightLayout);
        vbo->unbind();
        auto indexBufferSpecs      = IndexBufferSpecs {};
        indexBufferSpecs.count     = 1;
        indexBufferSpecs.isDynamic = true;
        ibo.reset(RenderFactory::makeIndexBuffer(indexBufferSpecs));
        ibo->unbind();
        vao = RenderFactory::makeVertexArray();
        vao->addVertexBuffer(vbo);
        vao->setIndexBuffer(ibo);
        vao->unbind();
    }

    void render(glm::mat4 const& view, glm::mat4 const& projection)
    {
        auto const model       = glm::translate(glm::mat4(1.0F), position);
        auto const scaleMatrix = glm::scale(glm::mat4(1.0F), {scale, scale, scale});
        for (auto const& vertex : lightMesh_.getVertices())
        {
            auto const transformedPos = model * scaleMatrix * glm::vec4(vertex.position, 1.0F);
            vertices.push_back({glm::vec3(transformedPos), color});
        }

        shader->bind();
        shader->setMat4("u_View", view);
        shader->setMat4("u_Projection", projection);

        vao->bind();
        auto const sizeInBytes = static_cast<std::uint32_t>(vertices.size() * sizeof(Light::Vertex));
        vbo->uploadData(0, sizeInBytes, vertices.data());
        RenderCommand::drawArrays(RenderDrawMode::Triangles, 0, static_cast<std::uint32_t>(vertices.size()));
        // drawStats_.numVertices += vertices.size();
        vertices.clear();
        vao->unbind();
    }
};
}  // namespace moci