#pragma once

#include "moci/render/vertex_array.hpp"

#if defined(MOCI_API_OPENGL_MODERN)
namespace moci
{

class OpenGLVertexArray : public VertexArray
{
public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray();

    void Bind() const override;
    void Unbind() const override;

    void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
    void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

    const Ref<VertexBuffer>& GetVertexBuffer() const override { return m_VertexBuffer; }
    const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }

private:
    uint32_t m_RendererID;
    uint32_t m_VertexBufferIndex = 0;
    Ref<VertexBuffer> m_VertexBuffer;
    Ref<IndexBuffer> m_IndexBuffer;
};

}  // namespace moci

#endif