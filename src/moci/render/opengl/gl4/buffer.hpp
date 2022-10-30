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

    void bind() const override;
    void unbind() const override;

    [[nodiscard]] auto getLayout() const -> const BufferLayout& override { return m_Layout; }
    void setLayout(const BufferLayout& layout) override { m_Layout = layout; }
    void uploadData(std::uint32_t offset, std::uint32_t size, const void* data) const override;

private:
    uint32_t m_RendererID {};
    BufferLayout m_Layout;
};

class OpenGLIndexBuffer : public IndexBuffer
{
public:
    explicit OpenGLIndexBuffer(IndexBufferSpecs specs);
    ~OpenGLIndexBuffer() override;

    void bind() const override;
    void unbind() const override;

    [[nodiscard]] auto getCount() const -> uint32_t override { return specs_.count; }
    void uploadData(std::uint32_t offset, Span<std::uint32_t> indices) const override;

private:
    IndexBufferSpecs specs_;
    std::uint32_t m_RendererID {};
};

}  // namespace moci

#endif