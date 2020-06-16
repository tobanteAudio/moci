#pragma once

#if defined(MOCI_API_OPENGL_MODERN)

#include "moci_render_base/moci_render_base.hpp"

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

private:
    void uploadUniformInt(std::string const& name, int value);
    void uploadUniformInts(std::string const& name, int count, int* values);
    void uploadUniformFloat(std::string const& name, float value);
    void uploadUniformFloat2(std::string const& name, const glm::vec2& value);
    void uploadUniformFloat3(std::string const& name, const glm::vec3& value);
    void uploadUniformFloat4(std::string const& name, const glm::vec4& value);
    void uploadUniformMat3(std::string const& name, const glm::mat3& matrix);
    void uploadUniformMat4(std::string const& name, const glm::mat4& matrix);

    std::string ReadFile(std::string const& filepath);
    std::unordered_map<GLenum, std::string> PreProcess(std::string const& source);
    void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

private:
    uint32_t m_RendererID;
    std::string m_Name;
};

}  // namespace moci

#endif