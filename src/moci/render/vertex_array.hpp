#pragma once

#include <moci/core/memory.hpp>
#include <moci/render/buffer.hpp>

namespace moci
{

class VertexArray
{
public:
    virtual ~VertexArray() = default;

    virtual void bind() const   = 0;
    virtual void unbind() const = 0;

    virtual void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
    virtual void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer)    = 0;

    [[nodiscard]] virtual auto getVertexBuffer() const -> const Ref<VertexBuffer>& = 0;
    [[nodiscard]] virtual auto getIndexBuffer() const -> const Ref<IndexBuffer>&   = 0;
};

}  // namespace moci
