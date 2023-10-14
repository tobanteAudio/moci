#pragma once

#include <moci/render/buffer.hpp>

#include <memory>

namespace moci
{

class VertexArray
{
public:
    virtual ~VertexArray() = default;

    virtual void bind() const   = 0;
    virtual void unbind() const = 0;

    virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
    virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)    = 0;

    [[nodiscard]] virtual auto getVertexBuffer() const -> const std::shared_ptr<VertexBuffer>& = 0;
    [[nodiscard]] virtual auto getIndexBuffer() const -> const std::shared_ptr<IndexBuffer>&   = 0;
};

}  // namespace moci
