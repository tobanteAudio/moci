#pragma once

#if defined(MOCI_API_OPENGL_MODERN)

#include <moci/render/shader.hpp>

#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

// TODO: REMOVE!
using GLenum = unsigned int;

namespace moci
{

class OpenGLShader : public Shader
{
public:
    explicit OpenGLShader(std::string const& filepath);
    OpenGLShader(std::string name, std::string const& vertexSrc, std::string const& fragmentSrc);
    ~OpenGLShader() override;

    [[nodiscard]] auto getName() const -> std::string const& override { return _name; }

    void bind() const override;
    void unbind() const override;

    void setInt(std::string const& name, int value) override;
    void setInts(std::string const& name, int count, int* values) override;
    void setFloat(std::string const& name, float value) override;
    void setFloat2(std::string const& name, glm::vec2 const& value) override;
    void setFloat3(std::string const& name, glm::vec3 const& value) override;
    void setFloat4(std::string const& name, glm::vec4 const& value) override;
    void setMat3(std::string const& name, glm::mat3 const& value) override;
    void setMat4(std::string const& name, glm::mat4 const& value) override;

private:
    [[nodiscard]] auto getLocation(std::string const& name) const -> std::int32_t;

    void uploadUniformInt(std::string const& name, int value);
    void uploadUniformInts(std::string const& name, int count, int* values);
    void uploadUniformFloat(std::string const& name, float value);
    void uploadUniformFloat2(std::string const& name, const glm::vec2& value);
    void uploadUniformFloat3(std::string const& name, const glm::vec3& value);
    void uploadUniformFloat4(std::string const& name, const glm::vec4& value);
    void uploadUniformMat3(std::string const& name, const glm::mat3& matrix);
    void uploadUniformMat4(std::string const& name, const glm::mat4& matrix);

    static auto ReadFile(std::string const& filepath) -> std::string;
    static auto PreProcess(std::string const& source) -> std::unordered_map<GLenum, std::string>;
    void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

    uint32_t _rendererID {};
    std::string _name;
};

}  // namespace moci

#endif