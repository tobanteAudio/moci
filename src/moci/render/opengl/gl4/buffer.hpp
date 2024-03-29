#pragma once

#if defined(MOCI_API_OPENGL_MODERN)

    #include <moci/render/buffer.hpp>

namespace moci {

class OpenGLVertexBuffer : public VertexBuffer
{
public:
    OpenGLVertexBuffer(float* vertices, uint32_t size, bool dynamic = false);
    ~OpenGLVertexBuffer() override;

    void bind() const override;
    void unbind() const override;

    [[nodiscard]] auto getLayout() const -> BufferLayout const& override { return _layout; }

    void setLayout(BufferLayout const& layout) override { _layout = layout; }

    void
    uploadData(std::uint32_t offset, std::uint32_t size, void const* data) const override;

private:
    uint32_t _rendererID{};
    BufferLayout _layout;
};

class OpenGLIndexBuffer : public IndexBuffer
{
public:
    explicit OpenGLIndexBuffer(IndexBufferSpecs specs);
    ~OpenGLIndexBuffer() override;

    void bind() const override;
    void unbind() const override;

    [[nodiscard]] auto getCount() const -> uint32_t override { return _specs.count; }

    void uploadData(std::uint32_t offset, std::span<std::uint32_t> indices) const override;

private:
    IndexBufferSpecs _specs;
    std::uint32_t _rendererID{};
};

}  // namespace moci

#endif
