#pragma once

#include "moci/render/buffer.hpp"
#include "moci/render/vertex_array.hpp"

#if defined(MOCI_API_OPENGL_MODERN)
namespace moci
{

class OpenGLVertexArray : public VertexArray
{
public:
    OpenGLVertexArray();
    ~OpenGLVertexArray() override;

    void bind() const override;
    void unbind() const override;

    void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
    void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

    [[nodiscard]] auto getVertexBuffer() const -> const Ref<VertexBuffer>& override { return _vertexBuffer; }
    [[nodiscard]] auto getIndexBuffer() const -> const Ref<IndexBuffer>& override { return _indexBuffer; }

private:
    uint32_t _rendererID {};
    uint32_t _vertexBufferIndex = 0;
    Ref<VertexBuffer> _vertexBuffer;
    Ref<IndexBuffer> _indexBuffer;
};

}  // namespace moci

#endif