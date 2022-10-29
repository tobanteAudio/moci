#pragma once

#include "moci_core/core/logging.hpp"
#include "moci_core/core/memory.hpp"

#include "moci_core/core/vector.hpp"
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
    using Ptr      = Ref<Texture2D>;
    using Optional = std::optional<Ptr>;
};

class TextureCube : public Texture
{
public:
};
}  // namespace moci