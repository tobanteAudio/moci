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

auto RenderFactory::makeVertexBuffer(float* vertices, uint32_t size, bool dynamic) -> VertexBuffer*
{
    switch (Renderer::getApi())
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

auto RenderFactory::makeIndexBuffer(IndexBufferSpecs specs) -> IndexBuffer*
{
    switch (Renderer::getApi())
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

auto RenderFactory::makeFramebuffer(FramebufferSpecs spec) -> Ref<Framebuffer>
{
    switch (Renderer::getApi())
    {
        case RendererAPI::API::None:
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL: return makeRef<OpenGLFramebuffer>(spec);
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES: return makeRef<OpenGLESFramebuffer>(spec);
#endif
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

auto RenderFactory::makeShader(const std::string& filepath) -> Ref<Shader>
{
    MOCI_CORE_INFO("Creating shader from: {}", filepath);
    switch (Renderer::getApi())
    {
        case RendererAPI::API::None:
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL: return makeRef<OpenGLShader>(filepath);
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES: return makeRef<OpenGLESShader>(filepath);
#endif
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

auto RenderFactory::makeShader(std::string const& name, std::string const& vertexSrc, std::string const& fragmentSrc)
    -> Ref<Shader>
{
    switch (Renderer::getApi())
    {
        case RendererAPI::API::None:
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL: return makeRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES: return makeRef<OpenGLESShader>(name, vertexSrc, fragmentSrc);
#endif
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

auto RenderFactory::makeTexture2D(std::string const& path) -> Ref<Texture2D>
{
    switch (Renderer::getApi())
    {
        case RendererAPI::API::None:
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL: return makeRef<OpenGLTexture2D>(path);
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES: return makeRef<OpenGLESTexture2D>(path);
#endif
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

auto RenderFactory::makeTexture2D(Texture::Format format, uint32_t width, uint32_t height, void* data) -> Ref<Texture2D>
{
    switch (Renderer::getApi())
    {
        case RendererAPI::API::None:
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL: return makeRef<OpenGLTexture2D>(format, width, height, data);
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES: return makeRef<OpenGLESTexture2D>(format, width, height, data);
#endif
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

auto RenderFactory::makeTextureCube(const Vector<std::string>& paths) -> Ref<TextureCube>
{
    switch (Renderer::getApi())
    {
        case RendererAPI::API::None:
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL: return makeRef<OpenGLTextureCube>(paths);
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES: return makeRef<OpenGLESTextureCube>(paths);
#endif
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

auto RenderFactory::makeVertexArray() -> Ref<VertexArray>
{
    switch (Renderer::getApi())
    {
        case RendererAPI::API::None:
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL: return makeRef<OpenGLVertexArray>();
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES: return makeRef<OpenGLESVertexArray>();
#endif
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}
}  // namespace moci