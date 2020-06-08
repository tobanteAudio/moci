#pragma once

#include "moci_render_base/moci_render_base.hpp"

#include "moci_render_opengles/es2.hpp"

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
    OpenGLESShader(std::string const& filepath);
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

public:
    void UploadUniformInt(std::string const& name, int value) const;
    void UploadUniformInts(std::string const& name, int count, int* values) const;
    void UploadUniformFloat(std::string const& name, float value) const;
    void UploadUniformFloat2(std::string const& name, const glm::vec2& value) const;
    void UploadUniformFloat3(std::string const& name, const glm::vec3& value) const;
    void UploadUniformFloat4(std::string const& name, glm::vec4 const& value) const;

    void UploadUniformMat3(std::string const& name, glm::mat3 const& matrix) const;
    void UploadUniformMat4(std::string const& name, glm::mat4 const& matrix) const;

private:
    // std::string ReadFile(std::string const& filepath);
    // std::unordered_map<GLenum, std::string> PreProcess(std::string const& source);
    // void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
    static auto createShader(const char* vertexSource, const char* fragmentSource) -> GLint;
    static auto parseShader(std::string const& filepath) -> ShaderProgramSource;

private:
    uint32_t m_RendererID;
    std::string m_Name;
};

}  // namespace moci