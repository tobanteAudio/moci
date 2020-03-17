#include "shader.hpp"

#include "renderer.hpp"

#include "moci/render/opengles2/shader.hpp"

namespace moci
{

auto Shader::Create(std::string const& filepath) -> std::shared_ptr<Shader>
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGLES: return std::make_shared<OpenGLESShader>(filepath);
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
        case RendererAPI::API::OpenGLES: return std::make_shared<OpenGLESShader>(name, vertexSrc, fragmentSrc);
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

}  // namespace moci