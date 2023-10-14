#include "buffer.hpp"

#include <moci/render/opengl/es2/es2.hpp>

namespace moci {

/////////////////////////////////////////////////////////////////////////////
// VertexBuffer /////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

OpenGLESVertexBuffer::OpenGLESVertexBuffer(float* vertices, uint32_t size, bool dynamic)
{
    GLCall(glGenBuffers(1, &_rendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, _rendererID));

    if (dynamic) {
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
    } else {
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));
    }
}

OpenGLESVertexBuffer::~OpenGLESVertexBuffer() { GLCall(glDeleteBuffers(1, &_rendererID)); }

void OpenGLESVertexBuffer::bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, _rendererID));
}

void OpenGLESVertexBuffer::unbind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); }

auto OpenGLESVertexBuffer::uploadData(
    std::uint32_t offset,
    std::uint32_t size,
    void const* data
) const -> void
{
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
}

/////////////////////////////////////////////////////////////////////////////
// IndexBuffer //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

OpenGLESIndexBuffer::OpenGLESIndexBuffer(IndexBufferSpecs specs) : _specs(specs)
{

    GLCall(glGenBuffers(1, &_rendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererID));

    auto const size = _specs.count * sizeof(std::uint16_t);
    if (_specs.isDynamic) {
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
    } else {
        auto indicesShort = convertToUnsignedShorts(_specs.indices);
        MOCI_CORE_ASSERT(
            size == indicesShort.size_bytes(),
            "Index span size & given size don't match"
        );
        GLCall(
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indicesShort.data(), GL_STATIC_DRAW)
        );
    }

    unbind();
}

OpenGLESIndexBuffer::~OpenGLESIndexBuffer() { GLCall(glDeleteBuffers(1, &_rendererID)); }

void OpenGLESIndexBuffer::bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererID));
}

void OpenGLESIndexBuffer::unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

auto OpenGLESIndexBuffer::uploadData(std::uint32_t offset, std::span<std::uint32_t> indices)
    const -> void
{
    auto indicesShort = convertToUnsignedShorts(indices);
    auto const size   = indicesShort.size() * sizeof(std::uint16_t);
    GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, indicesShort.data()));
}

auto OpenGLESIndexBuffer::convertToUnsignedShorts(std::span<std::uint32_t> indices) const
    -> std::span<std::uint16_t>
{
    _indicesShort->clear();
    _indicesShort->reserve(indices.size());
    for (auto const index : indices) {
        _indicesShort->push_back(static_cast<std::uint16_t>(index));
    }

    return *_indicesShort;
}

}  // namespace moci
