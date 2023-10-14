#pragma once

#include <moci/render/buffer.hpp>
#include <moci/render/vertex_array.hpp>

#if defined(MOCI_API_OPENGL_MODERN)
namespace moci
{

class OpenGLVertexArray : public VertexArray
{
public:
    OpenGLVertexArray();
    ~OpenGLVertexArray() override;

    void bind() const override;
    void unbind() const override;

    void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
    void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

    [[nodiscard]] auto getVertexBuffer() const -> const std::shared_ptr<VertexBuffer>& override { return _vertexBuffer; }
    [[nodiscard]] auto getIndexBuffer() const -> const std::shared_ptr<IndexBuffer>& override { return _indexBuffer; }

private:
    uint32_t _rendererID {};
    uint32_t _vertexBufferIndex = 0;
    std::shared_ptr<VertexBuffer> _vertexBuffer;
    std::shared_ptr<IndexBuffer> _indexBuffer;
};

}  // namespace moci

#endif