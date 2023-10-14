#pragma once

#include <moci/render/buffer.hpp>

#include <cstddef>
#include <span>

namespace moci
{

class OpenGLESVertexBuffer : public VertexBuffer
{
public:
    OpenGLESVertexBuffer(float* vertices, uint32_t size, bool dynamic);
    ~OpenGLESVertexBuffer() override;

    void bind() const override;
    void unbind() const override;
    auto uploadData(std::uint32_t offset, std::uint32_t size, const void* data) const -> void override;
    [[nodiscard]] auto getLayout() const -> const BufferLayout& override { return _layout; }
    void setLayout(const BufferLayout& layout) override { _layout = layout; }

private:
    uint32_t _rendererID {};
    BufferLayout _layout;
};

class OpenGLESIndexBuffer : public IndexBuffer
{
public:
    explicit OpenGLESIndexBuffer(IndexBufferSpecs specs);
    ~OpenGLESIndexBuffer() override;

    void bind() const override;
    void unbind() const override;
    auto uploadData(std::uint32_t offset, std::span<std::uint32_t> indices) const -> void override;
    [[nodiscard]] auto getCount() const -> uint32_t override { return _specs.count; }

private:
    [[nodiscard]] auto convertToUnsignedShorts(std::span<std::uint32_t> indices) const -> std::span<std::uint16_t>;

    IndexBufferSpecs _specs                                   = {};
    uint32_t _rendererID                                      = {};
    std::unique_ptr<std::vector<std::uint16_t>> _indicesShort = std::make_unique<std::vector<std::uint16_t>>();
};

}  // namespace moci