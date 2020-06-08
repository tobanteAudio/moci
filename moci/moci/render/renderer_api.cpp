#include "moci_graphics_api_base/moci_graphics_api_base.hpp"

namespace moci
{

#if defined(MOCI_API_OPENGL_LEGACY)
RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGLES;
#elif defined(MOCI_API_OPENGL_MODERN)
RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
#else
#error "Unsupported API"
#endif
}  // namespace moci
