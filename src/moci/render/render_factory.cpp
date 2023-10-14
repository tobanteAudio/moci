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

namespace moci {

auto RenderFactory::makeVertexBuffer(float* vertices, uint32_t size, bool dynamic)
    -> VertexBuffer*
{
    switch (Renderer::getApi()) {
        case RendererAPI::API::None: {
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        }
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL: {
            MOCI_CORE_INFO("Make OpenGL Vertex Buffer");
            return new OpenGLVertexBuffer(vertices, size, dynamic);
        }
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES: {
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
    switch (Renderer::getApi()) {
        case RendererAPI::API::None: {
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        }
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL: {
            MOCI_CORE_INFO("Make OpenGL Index Buffer");
            return new OpenGLIndexBuffer(specs);
        }
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES: {
            MOCI_CORE_INFO("Make OpenGLES Index Buffer");
            return new OpenGLESIndexBuffer(specs);
        }
#endif
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

auto RenderFactory::makeFramebuffer(FramebufferSpecs spec) -> std::shared_ptr<Framebuffer>
{
    switch (Renderer::getApi()) {
        case RendererAPI::API::None: {
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        }
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL: {
            MOCI_CORE_INFO("Make OpenGL FrameBuffer");
            return std::make_shared<OpenGLFramebuffer>(spec);
        }
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES: {
            MOCI_CORE_INFO("Make OpenGLES FrameBuffer");
            return std::make_shared<OpenGLESFramebuffer>(spec);
        }
#endif
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

auto RenderFactory::makeShader(std::string const& filepath) -> std::shared_ptr<Shader>
{
    MOCI_CORE_INFO("Creating shader from: {}", filepath);
    switch (Renderer::getApi()) {
        case RendererAPI::API::None: {
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        }
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL: {
            MOCI_CORE_INFO("Make OpenGL Shader: {:s}", filepath);
            return std::make_shared<OpenGLShader>(filepath);
        }
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES: {
            MOCI_CORE_INFO("Make OpenGLES Shader: {:s}", filepath);
            return std::make_shared<OpenGLESShader>(filepath);
        }
#endif
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

auto RenderFactory::makeShader(
    std::string const& name,
    std::string const& vertexSrc,
    std::string const& fragmentSrc
) -> std::shared_ptr<Shader>
{
    switch (Renderer::getApi()) {
        case RendererAPI::API::None: {
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        }
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL: {
            MOCI_CORE_INFO("Make OpenGL Shader: {:s}", name);
            return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
        }
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES: {
            MOCI_CORE_INFO("Make OpenGLES Shader: {:s}", name);
            return std::make_shared<OpenGLESShader>(name, vertexSrc, fragmentSrc);
        }
#endif
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

auto RenderFactory::makeTexture2D(std::string const& path) -> std::shared_ptr<Texture2D>
{
    switch (Renderer::getApi()) {
        case RendererAPI::API::None: {
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        }
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL: {
            MOCI_CORE_INFO("Make OpenGL Texture2D: {:s}", path);
            return std::make_shared<OpenGLTexture2D>(path);
        }
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES: {
            MOCI_CORE_INFO("Make OpenGLES Texture2D: {:s}", path);
            return std::make_shared<OpenGLESTexture2D>(path);
        }
#endif
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

auto RenderFactory::makeTexture2D(
    Texture::Format format,
    uint32_t width,
    uint32_t height,
    void* data
) -> std::shared_ptr<Texture2D>
{
    switch (Renderer::getApi()) {
        case RendererAPI::API::None: {
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        }
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL: {
            MOCI_CORE_INFO("Make OpenGL Texture2D from data");
            return std::make_shared<OpenGLTexture2D>(format, width, height, data);
        }
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES: {
            MOCI_CORE_INFO("Make OpenGLES Texture2D from data");
            return std::make_shared<OpenGLESTexture2D>(format, width, height, data);
        }
#endif
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

auto RenderFactory::makeTextureCube(std::vector<std::string> const& paths)
    -> std::shared_ptr<TextureCube>
{
    switch (Renderer::getApi()) {
        case RendererAPI::API::None: {
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        }
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL: {
            MOCI_CORE_INFO("Make OpenGL TextureCube");
            return std::make_shared<OpenGLTextureCube>(paths);
        }
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES: {
            MOCI_CORE_INFO("Make OpenGLES TextureCube");
            return std::make_shared<OpenGLESTextureCube>(paths);
        }
#endif
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

auto RenderFactory::makeVertexArray() -> std::shared_ptr<VertexArray>
{
    switch (Renderer::getApi()) {
        case RendererAPI::API::None: {
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        }
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL: {
            MOCI_CORE_INFO("Make OpenGL VertexArray");
            return std::make_shared<OpenGLVertexArray>();
        }
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES: {
            MOCI_CORE_INFO("Make OpenGLES VertexArray");
            return std::make_shared<OpenGLESVertexArray>();
        }
#endif
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}
}  // namespace moci
