#include "renderer.hpp"

// #include "Platform/OpenGL/OpenGLShader.h"
// #include "Platform/OpenGLES/OpenGLESShader.h"
// #include "Renderer2D.h"

namespace moci
{

// std::unique_ptr<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

void Renderer::init()
{
    RenderCommand::init();
    // Renderer2D::Init();
}

void Renderer::onWindowResize(uint32_t width, uint32_t height)
{
    RenderCommand::setViewport(Rectangle<uint32_t> {0, 0, width, height});
}

// void Renderer::BeginScene(OrthographicCamera& camera)
// {
//     s_SceneData->ViewProjectionMatrix = camera.getViewProjectionMatrix();
// }

// void Renderer::EndScene() {}

// void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray,
//                       const glm::mat4& transform)
// {
//     shader->Bind();
//     // std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection",
//     // s_SceneData->ViewProjectionMatrix);
//     // std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

//     std::dynamic_pointer_cast<OpenGLESShader>(shader)->UploadUniformMat4("u_ViewProjection",
//                                                                          s_SceneData->ViewProjectionMatrix);
//     std::dynamic_pointer_cast<OpenGLESShader>(shader)->UploadUniformMat4("u_Transform", transform);

//     vertexArray->Bind();
//     RenderCommand::DrawIndexed(vertexArray);
// }

}  // namespace moci