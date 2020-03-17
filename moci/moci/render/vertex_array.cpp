
#include "vertex_array.hpp"
#include "renderer.hpp"

#include "moci/render/opengles2/vertex_array.hpp"

namespace moci
{

auto VertexArray::Create() -> std::shared_ptr<VertexArray>
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:
            MOCI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        // case RendererAPI::API::OpenGL: return std::make_shared<OpenGLVertexArray>();
        case RendererAPI::API::OpenGLES: return std::make_shared<OpenGLESVertexArray>();
    }

    MOCI_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

}  // namespace moci