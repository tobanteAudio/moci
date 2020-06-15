#include "render_command.hpp"

#include "moci_render_opengl/moci_render_opengl.hpp"
#include "moci_render_opengles/moci_render_opengles.hpp"

namespace moci
{
#if defined(MOCI_API_OPENGL_LEGACY)
Scope<RendererAPI> RenderCommand::s_RendererAPI = MakeScope<OpenGLESRendererAPI>();
#else
Scope<RendererAPI> RenderCommand::s_RendererAPI = MakeScope<OpenGLRendererAPI>();
#endif
}  // namespace moci