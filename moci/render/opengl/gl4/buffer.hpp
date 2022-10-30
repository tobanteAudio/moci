#pragma once

#if defined(MOCI_API_OPENGL_MODERN)

#include "moci/render/buffer.hpp"

namespace moci
{

class OpenGLVertexBuffer : public VertexBuffer
{
public:
    OpenGLVertexBuffer(float* vertices, uint32_t size, bool dynamic = false);
    ~OpenGLVertexBuffer() override;

    void Bind() const override;
    void Unbind() const override;

    [[nodiscard]] auto GetLayout() const -> const BufferLayout& override { return m_Layout; }
    void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
    void UploadData(std::uint32_t offset, std::uint32_t size, const void* data) const override;

private:
    uint32_t m_RendererID {};
    BufferLayout m_Layout;
};

class OpenGLIndexBuffer : public IndexBuffer
{
public:
    explicit OpenGLIndexBuffer(IndexBufferSpecs specs);
    ~OpenGLIndexBuffer() override;

    void Bind() const override;
    void Unbind() const override;

    [[nodiscard]] auto GetCount() const -> uint32_t override { return specs_.count; }
    void UploadData(std::uint32_t offset, Span<std::uint32_t> indices) const override;

private:
    IndexBufferSpecs specs_;
    std::uint32_t m_RendererID {};
};

}  // namespace moci

#endif