#pragma once

#include "moci/core/logging.hpp"

#include <memory>
#include <optional>
#include <string>

namespace moci
{

class Texture
{
public:
    enum class Format
    {
        None = 0,
        Alpha,
        Luminance,
        LuminanceAlpha,
        RGB,
        RGBA,
    };

public:
    virtual ~Texture() = default;

    [[nodiscard]] virtual auto GetWidth() const -> uint32_t  = 0;
    [[nodiscard]] virtual auto GetHeight() const -> uint32_t = 0;
    [[nodiscard]] virtual auto GetID() const -> uint32_t     = 0;

    virtual void Bind(uint32_t slot = 0) const = 0;
    virtual void Unbind() const                = 0;
};

class Texture2D : public Texture
{
public:
    using Ptr      = std::shared_ptr<Texture2D>;
    using Optional = std::optional<Ptr>;

public:
    static auto Create(std::string const& path) -> std::shared_ptr<Texture2D>;
    static auto Create(Texture::Format format, uint32_t width, uint32_t height, void* data)
        -> std::shared_ptr<Texture2D>;
};

}  // namespace moci