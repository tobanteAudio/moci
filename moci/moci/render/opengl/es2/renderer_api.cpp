#include "moci/render/opengl/es2/renderer_api.hpp"

#include "moci/render/opengl/es2/es2.hpp"

namespace moci
{

void OpenGLESRendererAPI::Init()
{
    glEnable(GL_BLEND);
    GLCall(glEnable(GL_DEPTH_TEST));
    // Accept fragment if it closer to the camera than the former one
    GLCall(glDepthFunc(GL_LESS));

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLESRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    glViewport(x, y, width, height);
}

void OpenGLESRendererAPI::SetClearColor(const glm::vec4& color) { glClearColor(color.r, color.g, color.b, color.a); }

void OpenGLESRendererAPI::Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void OpenGLESRendererAPI::DrawArrays(RendererAPI::DrawMode mode, uint32_t first, uint32_t count)
{
    GLCall(glDrawArrays(GL_TRIANGLES, first, count));
}

void OpenGLESRendererAPI::DrawElements(RendererAPI::DrawMode mode, uint32_t count, RendererAPI::ElementType type,
                                       void* indices)
{
    GLCall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, indices));
}

auto OpenGLESRendererAPI::MaxTextureSize() -> std::uint32_t
{
    GLint maxTextureSize = 0;
    GLCall(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize));
    return static_cast<std::uint32_t>(maxTextureSize);
}
auto OpenGLESRendererAPI::MaxTextureUnits() -> std::uint32_t
{
    GLint maxTextureUnits = 0;
    GLCall(glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &maxTextureUnits));
    return static_cast<std::uint32_t>(maxTextureUnits);
}
auto OpenGLESRendererAPI::MaxVertexAttributes() -> std::uint32_t
{
    GLint maxAttributes = 0;
    GLCall(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttributes));
    return static_cast<std::uint32_t>(maxAttributes);
}

auto OpenGLESRendererAPI::MaxUniformVectors() -> std::uint32_t
{
    GLint maxUniformVectors = 0;
    // GLCall(glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &maxUniformVectors));
    return static_cast<std::uint32_t>(maxUniformVectors);
}

}  // namespace moci