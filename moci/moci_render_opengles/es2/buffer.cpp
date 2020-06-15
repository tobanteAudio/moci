#include "moci_render_opengles/es2/buffer.hpp"

#include "moci_render_opengles/es2/es2.hpp"

namespace moci
{

/////////////////////////////////////////////////////////////////////////////
// VertexBuffer /////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

OpenGLESVertexBuffer::OpenGLESVertexBuffer(float* vertices, uint32_t size, bool dynamic)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));

    if (dynamic)
    {
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
    }
    else
    {
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));
    }
}

OpenGLESVertexBuffer::~OpenGLESVertexBuffer() { GLCall(glDeleteBuffers(1, &m_RendererID)); }

void OpenGLESVertexBuffer::Bind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); }

void OpenGLESVertexBuffer::Unbind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); }

auto OpenGLESVertexBuffer::UploadData(std::uint32_t offset, std::uint32_t size, const void* data) const -> void
{
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
}
/////////////////////////////////////////////////////////////////////////////
// IndexBuffer //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

OpenGLESIndexBuffer::OpenGLESIndexBuffer(uint32_t* indices, uint32_t count, bool dynamic) : m_Count(count)
{

    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));

    auto const size = m_Count * sizeof(std::uint16_t);
    if (dynamic)
    {
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
    }
    else
    {
        auto const indicesSpan = Span<std::uint32_t> {indices, count};
        auto indicesShort      = Vector<std::uint16_t> {};
        indicesShort.reserve(indicesSpan.size());
        for (auto const index : indicesSpan)
        {
            indicesShort.push_back(gsl::narrow<std::uint16_t>(index));
        }
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indicesShort.data(), GL_STATIC_DRAW));
    }

    Unbind();
}

OpenGLESIndexBuffer::~OpenGLESIndexBuffer() { GLCall(glDeleteBuffers(1, &m_RendererID)); }

void OpenGLESIndexBuffer::Bind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); }

void OpenGLESIndexBuffer::Unbind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }

auto OpenGLESIndexBuffer::UploadData(std::uint32_t offset, Span<std::uint32_t> indices) const -> void
{
    Vector<std::uint16_t> indicesShort = {};
    indicesShort.reserve(indices.size());
    for (auto const index : indices)
    {
        indicesShort.push_back(gsl::narrow<std::uint16_t>(index));
    }
    auto const size = indicesShort.size() * sizeof(std::uint16_t);
    GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, indicesShort.data()));
}

}  // namespace moci