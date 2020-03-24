#include "shader.hpp"

#include "renderer.hpp"

#include "moci/render/opengl/es2/shader.hpp"
#include "moci/render/opengl/gl4/shader.hpp"

namespace moci
{

auto Shader::Create(std::string const& filepath) -> Ref<Shader>
{
    MOCI_CORE_INFO("Creating shader from: {}", filepath);
    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL: return MakeRef<OpenGLShader>(filepath);
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES: return MakeRef<OpenGLESShader>(filepath);
#endif
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

auto Shader::Create(std::string const& name, std::string const& vertexSrc, std::string const& fragmentSrc)
    -> Ref<Shader>
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL: return MakeRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES: return MakeRef<OpenGLESShader>(name, vertexSrc, fragmentSrc);
#endif
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

}  // namespace moci