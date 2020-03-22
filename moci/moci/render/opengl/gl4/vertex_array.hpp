#pragma once

#include "moci/render/vertex_array.hpp"

#if defined(MOCI_API_OPENGL)
namespace moci
{

class OpenGLVertexArray : public VertexArray
{
public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray();

    void Bind() const override;
    void Unbind() const override;

    void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
    void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

    const std::shared_ptr<VertexBuffer>& GetVertexBuffer() const override { return m_VertexBuffer; }
    const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }

private:
    uint32_t m_RendererID;
    uint32_t m_VertexBufferIndex = 0;
    std::shared_ptr<VertexBuffer> m_VertexBuffer;
    std::shared_ptr<IndexBuffer> m_IndexBuffer;
};

}  // namespace moci

#endif