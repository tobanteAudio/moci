#include "render_factory.hpp"

#include "renderer.hpp"

#include <moci/core/logging.hpp>
#include <moci/render/opengl/es2/buffer.hpp>
#include <moci/render/opengl/es2/frame_buffer.hpp>
#include <moci/render/opengl/es2/shader.hpp>
#include <moci/render/opengl/es2/texture.hpp>
#include <moci/render/opengl/es2/vertex_array.hpp>
#include <moci/render/opengl/gl4/buffer.hpp>
#include <moci/render/opengl/gl4/frame_buffer.hpp>
#include <moci/render/opengl/gl4/shader.hpp>
#include <moci/render/opengl/gl4/texture.hpp>
#include <moci/render/opengl/gl4/vertex_array.hpp>

namespace moci
{

auto RenderFactory::makeVertexBuffer(float* vertices, uint32_t size, bool dynamic) -> VertexBuffer*
{
    switch (Renderer::getApi())
    {
        case RendererAPI::API::None:
        {
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        }
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL:
        {
            MOCI_CORE_INFO("Make OpenGL Vertex Buffer");
            return new OpenGLVertexBuffer(vertices, size, dynamic);
        }
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES:
        {
            MOCI_CORE_INFO("Make OpenGLES Vertex Buffer");
            return new OpenGLESVertexBuffer(vertices, size, dynamic);
        }
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
        {
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        }
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL:
        {
            MOCI_CORE_INFO("Make OpenGL Index Buffer");
            return new OpenGLIndexBuffer(specs);
        }
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES:
        {
            MOCI_CORE_INFO("Make OpenGLES Index Buffer");
            return new OpenGLESIndexBuffer(specs);
        }
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
        {
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        }
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL:
        {
            MOCI_CORE_INFO("Make OpenGL FrameBuffer");
            return makeRef<OpenGLFramebuffer>(spec);
        }
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES:
        {
            MOCI_CORE_INFO("Make OpenGLES FrameBuffer");
            return makeRef<OpenGLESFramebuffer>(spec);
        }
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
        {
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        }
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL:
        {
            MOCI_CORE_INFO("Make OpenGL Shader: {:s}", filepath);
            return makeRef<OpenGLShader>(filepath);
        }
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES:
        {
            MOCI_CORE_INFO("Make OpenGLES Shader: {:s}", filepath);
            return makeRef<OpenGLESShader>(filepath);
        }
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
        {
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        }
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL:
        {
            MOCI_CORE_INFO("Make OpenGL Shader: {:s}", name);
            return makeRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
        }
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES:
        {
            MOCI_CORE_INFO("Make OpenGLES Shader: {:s}", name);
            return makeRef<OpenGLESShader>(name, vertexSrc, fragmentSrc);
        }
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
        {
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        }
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL:
        {
            MOCI_CORE_INFO("Make OpenGL Texture2D: {:s}", path);
            return makeRef<OpenGLTexture2D>(path);
        }
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES:
        {
            MOCI_CORE_INFO("Make OpenGLES Texture2D: {:s}", path);
            return makeRef<OpenGLESTexture2D>(path);
        }
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
        {
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        }
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL:
        {
            MOCI_CORE_INFO("Make OpenGL Texture2D from data");
            return makeRef<OpenGLTexture2D>(format, width, height, data);
        }
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES:
        {
            MOCI_CORE_INFO("Make OpenGLES Texture2D from data");
            return makeRef<OpenGLESTexture2D>(format, width, height, data);
        }
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
        {
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        }
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL:
        {
            MOCI_CORE_INFO("Make OpenGL TextureCube");
            return makeRef<OpenGLTextureCube>(paths);
        }
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES:
        {
            MOCI_CORE_INFO("Make OpenGLES TextureCube");
            return makeRef<OpenGLESTextureCube>(paths);
        }
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
        {
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        }
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL:
        {
            MOCI_CORE_INFO("Make OpenGL VertexArray");
            return makeRef<OpenGLVertexArray>();
        }
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES:
        {
            MOCI_CORE_INFO("Make OpenGLES VertexArray");
            return makeRef<OpenGLESVertexArray>();
        }
#endif
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}
}  // namespace moci