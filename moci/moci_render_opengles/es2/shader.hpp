#pragma once

#include "moci_render_base/moci_render_base.hpp"

#include "moci_render_opengles/es2/es2.hpp"

#include <glm/glm.hpp>

#include <array>
#include <unordered_map>

// TODO: REMOVE!
using GLenum = unsigned int;

namespace moci
{

class OpenGLESShader : public Shader
{
public:
    explicit OpenGLESShader(std::string const& filepath);
    OpenGLESShader(std::string name, std::string const& vertexSrc, std::string const& fragmentSrc);
    ~OpenGLESShader() override;

    void Bind() const override;
    void Unbind() const override;

    void SetInt(std::string const& name, int value) override;
    void SetInts(std::string const& name, int count, int* value) override;
    void SetFloat(std::string const& name, float value) override;
    void SetFloat2(std::string const& name, glm::vec2 const& value) override;
    void SetFloat3(std::string const& name, glm::vec3 const& value) override;
    void SetFloat4(std::string const& name, glm::vec4 const& value) override;
    void SetMat3(std::string const& name, glm::mat3 const& value) override;
    void SetMat4(std::string const& name, glm::mat4 const& value) override;

    [[nodiscard]] auto GetName() const -> std::string const& override { return m_Name; }

private:
    [[nodiscard]] std::int32_t getLocation(std::string const& name) const;

    void uploadUniformInt(std::string const& name, int value) const;
    void uploadUniformInts(std::string const& name, int count, int* values) const;
    void uploadUniformFloat(std::string const& name, float value) const;
    void uploadUniformFloat2(std::string const& name, const glm::vec2& value) const;
    void uploadUniformFloat3(std::string const& name, const glm::vec3& value) const;
    void uploadUniformFloat4(std::string const& name, glm::vec4 const& value) const;

    void uploadUniformMat3(std::string const& name, glm::mat3 const& matrix) const;
    void uploadUniformMat4(std::string const& name, glm::mat4 const& matrix) const;

    static auto createShader(const char* vertexSource, const char* fragmentSource) -> GLint;
    static auto parseShader(std::string const& filepath) -> ShaderProgramSource;

    uint32_t m_RendererID;
    std::string m_Name;
};

}  // namespace moci