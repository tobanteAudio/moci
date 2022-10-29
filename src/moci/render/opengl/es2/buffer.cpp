#include "buffer.hpp"

#include "moci/render/opengl/es2/es2.hpp"

namespace moci
{

/////////////////////////////////////////////////////////////////////////////
// VertexBuffer /////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

OpenGLESVertexBuffer::OpenGLESVertexBuffer(float* vertices, uint32_t size, bool dynamic)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));

    if (dynamic) { GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW)); }
    else { GLCall(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW)); }
}

OpenGLESVertexBuffer::~OpenGLESVertexBuffer() { GLCall(glDeleteBuffers(1, &m_RendererID)); }

void OpenGLESVertexBuffer::bind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); }

void OpenGLESVertexBuffer::unbind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); }

auto OpenGLESVertexBuffer::uploadData(std::uint32_t offset, std::uint32_t size, const void* data) const -> void
{
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
}
/////////////////////////////////////////////////////////////////////////////
// IndexBuffer //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

OpenGLESIndexBuffer::OpenGLESIndexBuffer(IndexBufferSpecs specs) : specs_(specs)
{

    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));

    auto const size = specs_.count * sizeof(std::uint16_t);
    if (specs_.isDynamic) { GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW)); }
    else
    {
        auto indicesShort = convertToUnsignedShorts(specs_.indices);
        MOCI_CORE_ASSERT(size == indicesShort.size_bytes(), "Index span size & given size don't match");
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indicesShort.data(), GL_STATIC_DRAW));
    }

    unbind();
}

OpenGLESIndexBuffer::~OpenGLESIndexBuffer() { GLCall(glDeleteBuffers(1, &m_RendererID)); }

void OpenGLESIndexBuffer::bind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); }

void OpenGLESIndexBuffer::unbind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }

auto OpenGLESIndexBuffer::uploadData(std::uint32_t offset, Span<std::uint32_t> indices) const -> void
{
    auto indicesShort = convertToUnsignedShorts(indices);
    auto const size   = indicesShort.size() * sizeof(std::uint16_t);
    GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, indicesShort.data()));
}

auto OpenGLESIndexBuffer::convertToUnsignedShorts(Span<std::uint32_t> indices) const -> Span<std::uint16_t>
{
    indicesShort_->clear();
    indicesShort_->reserve(indices.size());
    for (auto const index : indices) { indicesShort_->push_back(static_cast<std::uint16_t>(index)); }

    return *indicesShort_;
}

}  // namespace moci