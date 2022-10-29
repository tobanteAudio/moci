#include "buffer.hpp"

#if defined(MOCI_API_OPENGL_MODERN)

#include "gl4.hpp"
#include "moci/benchmark/profile.hpp"
#include "moci/core/logging.hpp"

namespace moci
{

OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size, bool dynamic)
{
    MOCI_PROFILE_FUNCTION();

    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);

    if (dynamic) { glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW); }
    else { glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW); }
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() { glDeleteBuffers(1, &m_RendererID); }

void OpenGLVertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); }

void OpenGLVertexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

auto OpenGLVertexBuffer::UploadData(std::uint32_t offset, std::uint32_t size, const void* data) const -> void
{
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

/////////////////////////////////////////////////////////////////////////////
// IndexBuffer //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

OpenGLIndexBuffer::OpenGLIndexBuffer(IndexBufferSpecs specs) : specs_(specs)
{
    MOCI_PROFILE_FUNCTION();

    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);

    auto const size = specs_.count * sizeof(uint32_t);
    if (specs_.isDynamic) { glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW); }
    else { glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, specs_.indices.data(), GL_STATIC_DRAW); }

    Unbind();
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() { glDeleteBuffers(1, &m_RendererID); }

void OpenGLIndexBuffer::Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); }

void OpenGLIndexBuffer::Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

auto OpenGLIndexBuffer::UploadData(std::uint32_t offset, Span<std::uint32_t> indices) const -> void
{
    auto const size = indices.size() * sizeof(std::uint32_t);
    auto* data      = indices.data();
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}
}  // namespace moci

#endif