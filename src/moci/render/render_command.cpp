#include "render_command.hpp"

#include <moci/render/opengl/es2/renderer_api.hpp>
#include <moci/render/opengl/gl4/renderer_api.hpp>

namespace moci
{
#if defined(MOCI_API_OPENGL_LEGACY)
std::unique_ptr<RendererAPI> RenderCommand::sRendererApi = std::make_unique<OpenGLESRendererAPI>();
#else
std::unique_ptr<RendererAPI> RenderCommand::sRendererApi = std::make_unique<OpenGLRendererAPI>();
#endif
}  // namespace moci