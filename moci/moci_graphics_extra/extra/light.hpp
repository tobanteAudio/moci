#pragma once

#include "mesh.hpp"

#include "moci_render_api/moci_render_api.hpp"
#include "moci_render_base/moci_render_base.hpp"

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

    glm::vec3 position = {8.4f, 4.0f, 3.0f};
    glm::vec4 color    = {1.0f, 1.0f, 1.0f, 1.0f};
    float scale        = 0.5f;

    Ref<Shader> shader    = nullptr;
    Ref<VertexBuffer> vbo = nullptr;
    Ref<IndexBuffer> ibo  = nullptr;
    Ref<VertexArray> vao  = nullptr;
    Mesh lightMesh_ {"sandbox3D/assets/models/sphere.obj"};
    Vector<Light::Vertex> vertices = {};

public:
    Light()
    {
#if defined(MOCI_API_OPENGL_LEGACY)
        shader = RenderFactory::MakeShader("sandbox3D/assets/shader/es2_light_source.glsl");
#else
        shader = RenderFactory::MakeShader("sandbox3D/assets/shader/gl4_light_source.glsl");
#endif
        shader->Bind();
        BufferLayout lightLayout = {
            {ShaderDataType::Float3, "a_Position"},  //
            {ShaderDataType::Float4, "a_Color"},     //
        };

        auto const size = static_cast<uint32_t>(lightMesh_.GetVertices().size() * sizeof(Light::Vertex));
        vbo.reset(RenderFactory::MakeVertexBuffer(nullptr, size, true));
        vbo->SetLayout(lightLayout);
        vbo->Unbind();
        auto indexBufferSpecs      = IndexBufferSpecs {};
        indexBufferSpecs.count     = 1;
        indexBufferSpecs.isDynamic = true;
        ibo.reset(RenderFactory::MakeIndexBuffer(indexBufferSpecs));
        ibo->Unbind();
        vao = RenderFactory::MakeVertexArray();
        vao->AddVertexBuffer(vbo);
        vao->SetIndexBuffer(ibo);
        vao->Unbind();
    }

    void Render(glm::mat4 const& view, glm::mat4 const& projection)
    {
        auto const model       = glm::translate(glm::mat4(1.0f), position);
        auto const scaleMatrix = glm::scale(glm::mat4(1.0f), {scale, scale, scale});
        for (auto const& vertex : lightMesh_.GetVertices())
        {
            auto const transformedPos = model * scaleMatrix * glm::vec4(vertex.position, 1.0f);
            vertices.push_back({glm::vec3(transformedPos), color});
        }

        shader->Bind();
        shader->SetMat4("u_View", view);
        shader->SetMat4("u_Projection", projection);

        vao->Bind();
        auto const sizeInBytes = static_cast<std::uint32_t>(vertices.size() * sizeof(Light::Vertex));
        vbo->UploadData(0, sizeInBytes, vertices.data());
        RenderCommand::DrawArrays(RenderDrawMode::Triangles, 0, static_cast<std::uint32_t>(vertices.size()));
        // drawStats_.numVertices += vertices.size();
        vertices.clear();
        vao->Unbind();
    }
};
}  // namespace moci