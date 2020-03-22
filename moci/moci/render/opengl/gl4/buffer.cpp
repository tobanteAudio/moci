#include "moci/render/opengl/gl4/buffer.hpp"

#include "moci/debug/instrumentor.hpp"
#include "moci/render/opengl/gl4/gl4.hpp"

namespace moci
{

OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size, bool dynamic)
{
    MOCI_PROFILE_FUNCTION();

    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);

    if (dynamic)
    {
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }
    else
    {
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }
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

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count, bool dynamic) : m_Count(count)
{
    MOCI_PROFILE_FUNCTION();

    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);

    if (dynamic)
    {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW);
    }
    else
    {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    Unbind();
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() { glDeleteBuffers(1, &m_RendererID); }

void OpenGLIndexBuffer::Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); }

void OpenGLIndexBuffer::Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

auto OpenGLIndexBuffer::UploadData(std::uint32_t offset, std::uint32_t size, const void* data) const -> void
{
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}
}  // namespace moci
