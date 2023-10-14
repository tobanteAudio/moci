#pragma once

#include <moci/core/logging.hpp>

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace moci {

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

    virtual ~Texture() = default;

    [[nodiscard]] virtual auto getWidth() const -> uint32_t  = 0;
    [[nodiscard]] virtual auto getHeight() const -> uint32_t = 0;
    [[nodiscard]] virtual auto getId() const -> uint32_t     = 0;

    virtual void bind(uint32_t slot = 0) const = 0;
    virtual void unbind() const                = 0;
};

class Texture2D : public Texture
{
public:
    using Ptr      = std::shared_ptr<Texture2D>;
    using Optional = std::optional<Ptr>;
};

class TextureCube : public Texture
{
public:
};
}  // namespace moci
