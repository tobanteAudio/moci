#pragma once

#include <moci/core/memory.hpp>
#include <moci/core/vector.hpp>

#include <glm/glm.hpp>

#include <string>

namespace moci
{
enum class ShaderType
{
    Unknown = 0,
    Vertex,
    Geometry,
    Tesselation,
    Fragment,
};

struct ShaderStageSource
{
    ShaderType type    = ShaderType::Unknown;
    std::string source = {};
};

struct ShaderProgramSource
{
    Vector<ShaderStageSource> shaders = {};
};

class Shader
{
public:
    virtual ~Shader() = default;

    virtual void bind() const   = 0;
    virtual void unbind() const = 0;

    virtual void setInt(std::string const& name, int value)                 = 0;
    virtual void setInts(std::string const& name, int count, int* value)    = 0;
    virtual void setFloat(std::string const& name, float value)             = 0;
    virtual void setFloat2(std::string const& name, glm::vec2 const& value) = 0;
    virtual void setFloat3(std::string const& name, glm::vec3 const& value) = 0;
    virtual void setFloat4(std::string const& name, glm::vec4 const& value) = 0;
    virtual void setMat3(std::string const& name, glm::mat3 const& value)   = 0;
    virtual void setMat4(std::string const& name, glm::mat4 const& value)   = 0;

    [[nodiscard]] virtual auto getName() const -> std::string const& = 0;
};

}  // namespace moci