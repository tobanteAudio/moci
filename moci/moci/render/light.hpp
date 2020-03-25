#pragma once

#include "moci/render/mesh.hpp"
#include "moci/render/render_command.hpp"
#include "moci/render/shader.hpp"
#include "moci/render/texture.hpp"
#include "moci/render/vertex_array.hpp"

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

    moci::Ref<moci::Shader> shader    = nullptr;
    moci::Ref<moci::VertexBuffer> vbo = nullptr;
    moci::Ref<moci::IndexBuffer> ibo  = nullptr;
    moci::Ref<moci::VertexArray> vao  = nullptr;
    moci::Mesh lightMesh_ {"sandbox3D/assets/models/sphere.obj"};
    std::vector<Light::Vertex> vertices = {};

public:
    Light()
    {
#if defined(MOCI_API_OPENGL_LEGACY)
        shader = moci::Shader::Create("sandbox3D/assets/shader/es2_light_source.glsl");
#else
        shader = moci::Shader::Create("sandbox3D/assets/shader/gl4_light_source.glsl");
#endif
        shader->Bind();
        moci::BufferLayout lightLayout = {
            {moci::ShaderDataType::Float3, "a_Position"},  //
            {moci::ShaderDataType::Float4, "a_Color"},     //
        };

        auto const size = static_cast<uint32_t>(lightMesh_.GetVertices().size() * sizeof(Light::Vertex));
        vbo.reset(moci::VertexBuffer::Create(nullptr, size, true));
        vbo->SetLayout(lightLayout);
        vbo->Unbind();
        ibo.reset(moci::IndexBuffer::Create(nullptr, 1, true));
        ibo->Unbind();
        vao = moci::VertexArray::Create();
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
            auto const position = model * scaleMatrix * glm::vec4(vertex.position, 1.0f);
            vertices.push_back({glm::vec3(position), color});
        }

        shader->Bind();
        shader->SetMat4("u_View", view);
        shader->SetMat4("u_Projection", projection);

        vao->Bind();
        vbo->UploadData(0, vertices.size() * sizeof(Light::Vertex), vertices.data());
        moci::RenderCommand::DrawArrays(moci::RendererAPI::DrawMode::Triangles, 0, vertices.size());
        // drawStats_.numVertices += vertices.size();
        vertices.clear();
        vao->Unbind();
    }
};
}  // namespace moci