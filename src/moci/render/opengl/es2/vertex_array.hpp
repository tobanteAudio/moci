#pragma once

#include <moci/render/buffer.hpp>
#include <moci/render/vertex_array.hpp>

#include <memory>

namespace moci
{

class OpenGLESVertexArray : public VertexArray
{
public:
    OpenGLESVertexArray();
    ~OpenGLESVertexArray() override;

    void bind() const override;
    void unbind() const override;

    void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
    void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

    [[nodiscard]] auto getVertexBuffer() const -> const std::shared_ptr<VertexBuffer>& override { return _vertexBuffer; }
    [[nodiscard]] auto getIndexBuffer() const -> const std::shared_ptr<IndexBuffer>& override { return _indexBuffer; }

private:
    void setLayout() const;

    std::shared_ptr<VertexBuffer> _vertexBuffer;
    std::shared_ptr<IndexBuffer> _indexBuffer;
};

}  // namespace moci
