#include "buffer.hpp"

#include "renderer.hpp"

#include "moci/render/opengl/es2/buffer.hpp"

namespace moci
{

auto VertexBuffer::Create(float* vertices, uint32_t size, bool dynamic) -> VertexBuffer*
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        // case RendererAPI::API::OpenGL: return new OpenGLVertexBuffer(vertices, size, dynamic);
        case RendererAPI::API::OpenGLES: return new OpenGLESVertexBuffer(vertices, size, dynamic);
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

auto IndexBuffer::Create(uint32_t* indices, uint32_t size, bool dynamic) -> IndexBuffer*
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        // case RendererAPI::API::OpenGL: return new OpenGLIndexBuffer(indices, size, dynamic);
        case RendererAPI::API::OpenGLES: return new OpenGLESIndexBuffer(indices, size, dynamic);
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

}  // namespace moci