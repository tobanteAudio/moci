#pragma once

#include "moci/render/vertex_array.hpp"

namespace moci
{

class OpenGLESVertexArray : public VertexArray
{
public:
    OpenGLESVertexArray();
    ~OpenGLESVertexArray() override;

    void Bind() const override;
    void Unbind() const override;

    void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
    void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

    [[nodiscard]] auto GetVertexBuffer() const -> const Ref<VertexBuffer>& override { return m_VertexBuffer; }
    [[nodiscard]] auto GetIndexBuffer() const -> const Ref<IndexBuffer>& override { return m_IndexBuffer; }

private:
    void setLayout() const;

private:
    Ref<VertexBuffer> m_VertexBuffer;
    Ref<IndexBuffer> m_IndexBuffer;
};

}  // namespace moci