#pragma once

#include <moci/render/buffer.hpp>
#include <moci/render/vertex_array.hpp>

#if defined(MOCI_API_OPENGL_MODERN)
namespace moci {

class OpenGLVertexArray : public VertexArray
{
public:
    OpenGLVertexArray();
    ~OpenGLVertexArray() override;

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
    uint32_t _rendererID{};
    uint32_t _vertexBufferIndex = 0;
    std::shared_ptr<VertexBuffer> _vertexBuffer;
    std::shared_ptr<IndexBuffer> _indexBuffer;
};

}  // namespace moci

#endif
