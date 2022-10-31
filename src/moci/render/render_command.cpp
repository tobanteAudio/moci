#include "render_command.hpp"

#include <moci/render/opengl/es2/renderer_api.hpp>
#include <moci/render/opengl/gl4/renderer_api.hpp>

namespace moci
{
#if defined(MOCI_API_OPENGL_LEGACY)
Scope<RendererAPI> RenderCommand::sRendererApi = makeScope<OpenGLESRendererAPI>();
#else
Scope<RendererAPI> RenderCommand::sRendererApi = makeScope<OpenGLRendererAPI>();
#endif
}  // namespace moci