#pragma once

#include "moci/render/shader.hpp"

#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

// TODO: REMOVE!
typedef unsigned int GLenum;

namespace moci
{

class OpenGLShader : public Shader
{
public:
    OpenGLShader(const std::string& filepath);
    OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
    ~OpenGLShader() override;

    void Bind() const override;
    void Unbind() const override;

    void SetInt(const std::string& name, int value) override;
    void SetFloat3(const std::string& name, const glm::vec3& value) override;
    void SetFloat4(const std::string& name, const glm::vec4& value) override;
    void SetMat4(const std::string& name, const glm::mat4& value) override;

    void SetInts(std::string const& name, int count, int* value) override {}
    void SetFloat(std::string const& name, float value) override {}
    void SetFloat2(std::string const& name, glm::vec2 const& value) override {}
    void SetMat3(std::string const& name, glm::mat3 const& value) override {}

    const std::string& GetName() const override { return m_Name; }

    void UploadUniformInt(const std::string& name, int value);

    void UploadUniformFloat(const std::string& name, float value);
    void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
    void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
    void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

    void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
    void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

private:
    std::string ReadFile(const std::string& filepath);
    std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
    void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

private:
    uint32_t m_RendererID;
    std::string m_Name;
};

}  // namespace moci