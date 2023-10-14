#include <moci/render/api.hpp>

namespace moci {

#if defined(MOCI_API_OPENGL_LEGACY)
RendererAPI::API RendererAPI::sApi = RendererAPI::API::OpenGLES;
#elif defined(MOCI_API_OPENGL_MODERN)
RendererAPI::API RendererAPI::sApi = RendererAPI::API::OpenGL;
#else
    #error "Unsupported API"
#endif
}  // namespace moci
