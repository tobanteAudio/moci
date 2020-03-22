
#include "vertex_array.hpp"
#include "renderer.hpp"

#include "moci/render/opengl/es2/vertex_array.hpp"
#include "moci/render/opengl/gl4/vertex_array.hpp"

namespace moci
{

auto VertexArray::Create() -> std::shared_ptr<VertexArray>
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
#if defined(MOCI_API_OPENGL_MODERN)
        case RendererAPI::API::OpenGL: return std::make_shared<OpenGLVertexArray>();
#endif
#if defined(MOCI_API_OPENGL_LEGACY)
        case RendererAPI::API::OpenGLES: return std::make_shared<OpenGLESVertexArray>();
#endif
        default: break;
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

}  // namespace moci