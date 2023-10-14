#pragma once

#include <moci/render/buffer.hpp>
#include <moci/render/vertex_array.hpp>

#include <memory>

namespace moci {

class OpenGLESVertexArray : public VertexArray
{
public:
    OpenGLESVertexArray();
    ~OpenGLESVertexArray() override;

    void bind() const override;
    void unbind() const override;

    void addVertexBuffer(std::shared_ptr<VertexBuffer> const& vertexBuffer) override;
    void setIndexBuffer(std::shared_ptr<IndexBuffer> const& indexBuffer) override;

    [[nodiscard]] auto getVertexBuffer() const
        -> std::shared_ptr<VertexBuffer> const& override
    {
        return _vertexBuffer;
    }

    [[nodiscard]] auto getIndexBuffer() const
        -> std::shared_ptr<IndexBuffer> const& override
    {
        return _indexBuffer;
    }

private:
    void setLayout() const;

    std::shared_ptr<VertexBuffer> _vertexBuffer;
    std::shared_ptr<IndexBuffer> _indexBuffer;
};

}  // namespace moci
