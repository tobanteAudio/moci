#pragma once

#include "buffer.hpp"

#include "moci/core/memory.hpp"

#include <memory>

namespace moci
{

class VertexArray
{
public:
    virtual ~VertexArray() = default;

    virtual void Bind() const   = 0;
    virtual void Unbind() const = 0;

    virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
    virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)    = 0;

    [[nodiscard]] virtual auto GetVertexBuffer() const -> const Ref<VertexBuffer>& = 0;
    [[nodiscard]] virtual auto GetIndexBuffer() const -> const Ref<IndexBuffer>&   = 0;

    static auto Create() -> Ref<VertexArray>;
};

}  // namespace moci
