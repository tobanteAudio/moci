#include "render_command.hpp"

#include "moci/render/opengl/es2/renderer_api.hpp"
#include "moci/render/opengl/gl4/renderer_api.hpp"

namespace moci
{

std::unique_ptr<RendererAPI> RenderCommand::s_RendererAPI = std::make_unique<OpenGLRendererAPI>();
}