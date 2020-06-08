#include "render_command.hpp"

#include "moci/render/opengl/gl4/renderer_api.hpp"
#include "moci_graphics_api_opengles/moci_graphics_api_opengles.hpp"

namespace moci
{
#if defined(MOCI_API_OPENGL_LEGACY)
Scope<RendererAPI> RenderCommand::s_RendererAPI = MakeScope<OpenGLESRendererAPI>();
#else
Scope<RendererAPI> RenderCommand::s_RendererAPI = MakeScope<OpenGLRendererAPI>();
#endif
}  // namespace moci