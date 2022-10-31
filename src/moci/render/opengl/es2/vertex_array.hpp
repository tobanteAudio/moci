#pragma once

#include <moci/core/memory.hpp>
#include <moci/render/buffer.hpp>
#include <moci/render/vertex_array.hpp>

namespace moci
{

class OpenGLESVertexArray : public VertexArray
{
public:
    OpenGLESVertexArray();
    ~OpenGLESVertexArray() override;

    void bind() const override;
    void unbind() const override;

    void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
    void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

    [[nodiscard]] auto getVertexBuffer() const -> const Ref<VertexBuffer>& override { return _vertexBuffer; }
    [[nodiscard]] auto getIndexBuffer() const -> const Ref<IndexBuffer>& override { return _indexBuffer; }

private:
    void setLayout() const;

    Ref<VertexBuffer> _vertexBuffer;
    Ref<IndexBuffer> _indexBuffer;
};

}  // namespace moci