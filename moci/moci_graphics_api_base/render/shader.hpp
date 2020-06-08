#pragma once

#include "moci_core/core/memory.hpp"

#include <glm/glm.hpp>

#include <memory>
#include <string>

namespace moci
{

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
public:
    virtual ~Shader() = default;

    virtual void Bind() const   = 0;
    virtual void Unbind() const = 0;

    virtual void SetInt(std::string const& name, int value)                 = 0;
    virtual void SetInts(std::string const& name, int count, int* value)    = 0;
    virtual void SetFloat(std::string const& name, float value)             = 0;
    virtual void SetFloat2(std::string const& name, glm::vec2 const& value) = 0;
    virtual void SetFloat3(std::string const& name, glm::vec3 const& value) = 0;
    virtual void SetFloat4(std::string const& name, glm::vec4 const& value) = 0;
    virtual void SetMat3(std::string const& name, glm::mat3 const& value)   = 0;
    virtual void SetMat4(std::string const& name, glm::mat4 const& value)   = 0;

    [[nodiscard]] virtual auto GetName() const -> std::string const& = 0;

    static auto Create(std::string const& filepath) -> Ref<Shader>;
    static auto Create(std::string const& name, std::string const& vertexSrc, std::string const& fragmentSrc)
        -> Ref<Shader>;
};

}  // namespace moci