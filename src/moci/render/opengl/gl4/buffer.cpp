#include "buffer.hpp"

#if defined(MOCI_API_OPENGL_MODERN)

#include "gl4.hpp"
#include "moci/core/benchmark/profile.hpp"
#include "moci/core/logging.hpp"

namespace moci
{

OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size, bool dynamic)
{
    MOCI_PROFILE_FUNCTION();

    glGenBuffers(1, &_rendererID);
    glBindBuffer(GL_ARRAY_BUFFER, _rendererID);

    if (dynamic) { glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW); }
    else { glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW); }
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() { glDeleteBuffers(1, &_rendererID); }

void OpenGLVertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, _rendererID); }

void OpenGLVertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

auto OpenGLVertexBuffer::uploadData(std::uint32_t offset, std::uint32_t size, const void* data) const -> void
{
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

/////////////////////////////////////////////////////////////////////////////
// IndexBuffer //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

OpenGLIndexBuffer::OpenGLIndexBuffer(IndexBufferSpecs specs) : _specs(specs)
{
    MOCI_PROFILE_FUNCTION();

    glGenBuffers(1, &_rendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererID);

    auto const size = _specs.count * sizeof(uint32_t);
    if (_specs.isDynamic) { glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW); }
    else { glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, _specs.indices.data(), GL_STATIC_DRAW); }

    unbind();
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() { glDeleteBuffers(1, &_rendererID); }

void OpenGLIndexBuffer::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererID); }

void OpenGLIndexBuffer::unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

auto OpenGLIndexBuffer::uploadData(std::uint32_t offset, Span<std::uint32_t> indices) const -> void
{
    auto const size = indices.size() * sizeof(std::uint32_t);
    auto* data      = indices.data();
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}
}  // namespace moci

#endif