#pragma once

#include "buffer.hpp"

#include <memory>

namespace moci
{

class VertexArray
{
public:
    virtual ~VertexArray() = default;

    virtual void Bind() const   = 0;
    virtual void Unbind() const = 0;

    virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
    virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)    = 0;

    [[nodiscard]] virtual auto GetVertexBuffer() const -> const std::shared_ptr<VertexBuffer>& = 0;
    [[nodiscard]] virtual auto GetIndexBuffer() const -> const std::shared_ptr<IndexBuffer>&   = 0;

    static auto Create() -> std::shared_ptr<VertexArray>;
};

}  // namespace moci
