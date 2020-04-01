#pragma once

#if defined(MOCI_API_OPENGL_MODERN)

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
    OpenGLShader(std::string const& filepath);
    OpenGLShader(std::string const& name, std::string const& vertexSrc, std::string const& fragmentSrc);
    ~OpenGLShader() override;

    std::string const& GetName() const override { return m_Name; }

    void Bind() const override;
    void Unbind() const override;

    void SetInt(std::string const& name, int value) override;
    void SetInts(std::string const& name, int count, int* values) override;
    void SetFloat(std::string const& name, float value) override;
    void SetFloat2(std::string const& name, glm::vec2 const& value) override;
    void SetFloat3(std::string const& name, glm::vec3 const& value) override;
    void SetFloat4(std::string const& name, glm::vec4 const& value) override;
    void SetMat3(std::string const& name, glm::mat3 const& value) override;
    void SetMat4(std::string const& name, glm::mat4 const& value) override;

    void UploadUniformInt(std::string const& name, int value);
    void UploadUniformInts(std::string const& name, int count, int* values);
    void UploadUniformFloat(std::string const& name, float value);
    void UploadUniformFloat2(std::string const& name, const glm::vec2& value);
    void UploadUniformFloat3(std::string const& name, const glm::vec3& value);
    void UploadUniformFloat4(std::string const& name, const glm::vec4& value);
    void UploadUniformMat3(std::string const& name, const glm::mat3& matrix);
    void UploadUniformMat4(std::string const& name, const glm::mat4& matrix);

private:
    std::string ReadFile(std::string const& filepath);
    std::unordered_map<GLenum, std::string> PreProcess(std::string const& source);
    void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

private:
    uint32_t m_RendererID;
    std::string m_Name;
};

}  // namespace moci

#endif