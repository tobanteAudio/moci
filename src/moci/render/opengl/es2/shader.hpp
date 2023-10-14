#pragma once

#include <moci/render/opengl/es2/es2.hpp>
#include <moci/render/shader.hpp>

#include <glm/glm.hpp>

#include <array>
#include <unordered_map>

// TODO: REMOVE!
using GLenum = unsigned int;

namespace moci {

class OpenGLESShader : public Shader
{
public:
    explicit OpenGLESShader(std::string const& filepath);
    OpenGLESShader(
        std::string name,
        std::string const& vertexSrc,
        std::string const& fragmentSrc
    );
    ~OpenGLESShader() override;

    void bind() const override;
    void unbind() const override;

    void setInt(std::string const& name, int value) override;
    void setInts(std::string const& name, int count, int* value) override;
    void setFloat(std::string const& name, float value) override;
    void setFloat2(std::string const& name, glm::vec2 const& value) override;
    void setFloat3(std::string const& name, glm::vec3 const& value) override;
    void setFloat4(std::string const& name, glm::vec4 const& value) override;
    void setMat3(std::string const& name, glm::mat3 const& value) override;
    void setMat4(std::string const& name, glm::mat4 const& value) override;

    [[nodiscard]] auto getName() const -> std::string const& override { return _name; }

private:
    [[nodiscard]] auto getLocation(std::string const& name) const -> std::int32_t;

    void uploadUniformInt(std::string const& name, int value) const;
    void uploadUniformInts(std::string const& name, int count, int* values) const;
    void uploadUniformFloat(std::string const& name, float value) const;
    void uploadUniformFloat2(std::string const& name, glm::vec2 const& value) const;
    void uploadUniformFloat3(std::string const& name, glm::vec3 const& value) const;
    void uploadUniformFloat4(std::string const& name, glm::vec4 const& value) const;

    void uploadUniformMat3(std::string const& name, glm::mat3 const& matrix) const;
    void uploadUniformMat4(std::string const& name, glm::mat4 const& matrix) const;

    static auto createShader(char const* vertexSource, char const* fragmentSource) -> GLint;
    static auto parseShader(std::string const& filepath) -> ShaderProgramSource;

    uint32_t _rendererID;
    std::string _name;
};

}  // namespace moci
