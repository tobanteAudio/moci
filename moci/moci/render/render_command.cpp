#include "render_command.hpp"

#include "moci/render/opengles2/renderer_api.hpp"

namespace moci
{

std::unique_ptr<RendererAPI> RenderCommand::s_RendererAPI = std::make_unique<OpenGLESRendererAPI>();
}