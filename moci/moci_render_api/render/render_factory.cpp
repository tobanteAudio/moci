#include "render_factory.hpp"

#include "renderer.hpp"

#include "moci/render/opengl/es2/buffer.hpp"
#include "moci/render/opengl/es2/frame_buffer.hpp"
#include "moci/render/opengl/es2/shader.hpp"
#include "moci/render/opengl/es2/texture.hpp"
#include "moci/render/opengl/es2/vertex_array.hpp"
#include "moci/render/opengl/gl4/buffer.hpp"
#include "moci/render/opengl/gl4/frame_buffer.hpp"
#include "moci/render/opengl/gl4/shader.hpp"
#include "moci/render/opengl/gl4/texture.hpp"
#include "moci/render/opengl/gl4/vertex_array.hpp"

namespace moci
{

auto RenderFactory::MakeVertexBuffer(float* vertices, uint32_t size, bool dynamic) -> VertexBuffer*
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL: return new OpenGLVertexBuffer(vertices, size, dynamic);
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES: return new OpenGLESVertexBuffer(vertices, size, dynamic);
#endif
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

auto RenderFactory::MakeIndexBuffer(IndexBufferSpecs specs) -> IndexBuffer*
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL: return new OpenGLIndexBuffer(specs);
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES: return new OpenGLESIndexBuffer(specs);
#endif
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

auto RenderFactory::MakeFramebuffer(FramebufferSpecs spec) -> Ref<Framebuffer>
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL: return MakeRef<OpenGLFramebuffer>(spec);
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES: return MakeRef<OpenGLESFramebuffer>(spec);
#endif
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

auto RenderFactory::MakeShader(const std::string& filepath) -> Ref<Shader>
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

auto RenderFactory::MakeShader(std::string const& name, std::string const& vertexSrc, std::string const& fragmentSrc)
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

auto RenderFactory::MakeTexture2D(std::string const& path) -> Ref<Texture2D>
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL: return MakeRef<OpenGLTexture2D>(path);
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES: return MakeRef<OpenGLESTexture2D>(path);
#endif
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

auto RenderFactory::MakeTexture2D(Texture::Format format, uint32_t width, uint32_t height, void* data) -> Ref<Texture2D>
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL: return MakeRef<OpenGLTexture2D>(format, width, height, data);
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES: return MakeRef<OpenGLESTexture2D>(format, width, height, data);
#endif
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

auto RenderFactory::MakeTextureCube(const Vector<std::string>& paths) -> Ref<TextureCube>
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL: return MakeRef<OpenGLTextureCube>(paths);
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES: return MakeRef<OpenGLESTextureCube>(paths);
#endif
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

auto RenderFactory::MakeVertexArray() -> Ref<VertexArray>
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL: return MakeRef<OpenGLVertexArray>();
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES: return MakeRef<OpenGLESVertexArray>();
#endif
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}
}  // namespace moci