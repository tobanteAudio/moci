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

    [[nodiscard]] auto getVertexBuffer() const -> const Ref<VertexBuffer>& override { return m_VertexBuffer; }
    [[nodiscard]] auto getIndexBuffer() const -> const Ref<IndexBuffer>& override { return m_IndexBuffer; }

private:
    uint32_t m_RendererID {};
    uint32_t m_VertexBufferIndex = 0;
    Ref<VertexBuffer> m_VertexBuffer;
    Ref<IndexBuffer> m_IndexBuffer;
};

}  // namespace moci

#endif