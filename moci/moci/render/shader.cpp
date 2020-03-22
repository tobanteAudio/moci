#include "shader.hpp"

#include "renderer.hpp"

#include "moci/render/opengl/es2/shader.hpp"
#include "moci/render/opengl/gl4/shader.hpp"

namespace moci
{

auto Shader::Create(std::string const& filepath) -> std::shared_ptr<Shader>
{
    MOCI_CORE_INFO("Creating shader from: {}", filepath);
    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(filepath);
        case RendererAPI::API::OpenGLES: return std::make_shared<OpenGLESShader>(filepath);
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

auto Shader::Create(std::string const& name, std::string const& vertexSrc, std::string const& fragmentSrc)
    -> std::shared_ptr<Shader>
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
        case RendererAPI::API::OpenGLES: return std::make_shared<OpenGLESShader>(name, vertexSrc, fragmentSrc);
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

}  // namespace moci