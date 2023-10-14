#pragma once

#include <moci/render/buffer.hpp>

#include <memory>

namespace moci {

class VertexArray
{
public:
    virtual ~VertexArray() = default;

    virtual void bind() const   = 0;
    virtual void unbind() const = 0;

    virtual void addVertexBuffer(std::shared_ptr<VertexBuffer> const& vertexBuffer) = 0;
    virtual void setIndexBuffer(std::shared_ptr<IndexBuffer> const& indexBuffer)    = 0;

    [[nodiscard]] virtual auto getVertexBuffer() const
        -> std::shared_ptr<VertexBuffer> const& = 0;
    [[nodiscard]] virtual auto getIndexBuffer() const
        -> std::shared_ptr<IndexBuffer> const& = 0;
};

}  // namespace moci
