
#include "moci_graphics_api_base/moci_graphics_api_base.hpp"

#include "renderer.hpp"

#include "moci/render/opengl/gl4/vertex_array.hpp"
#include "moci_graphics_api_opengles/moci_graphics_api_opengles.hpp"

namespace moci
{

auto VertexArray::Create() -> Ref<VertexArray>
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