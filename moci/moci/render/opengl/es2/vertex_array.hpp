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

    void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
    void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

    [[nodiscard]] auto GetVertexBuffer() const -> const std::shared_ptr<VertexBuffer>& override
    {
        return m_VertexBuffer;
    }
    [[nodiscard]] auto GetIndexBuffer() const -> const std::shared_ptr<IndexBuffer>& override { return m_IndexBuffer; }

private:
    void setLayout() const;

private:
    std::shared_ptr<VertexBuffer> m_VertexBuffer;
    std::shared_ptr<IndexBuffer> m_IndexBuffer;
};

}  // namespace moci