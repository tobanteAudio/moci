#pragma once

#if defined(MOCI_API_OPENGL_MODERN)

    #include <moci/render/shader.hpp>

    #include <glm/glm.hpp>

    #include <string>
    #include <unordered_map>

// TODO: REMOVE!
using GLenum = unsigned int;

namespace moci {

class OpenGLShader : public Shader
{
public:
    explicit OpenGLShader(std::string const& filepath);
    OpenGLShader(
        std::string name,
        std::string const& vertexSrc,
        std::string const& fragmentSrc
    );
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
    void uploadUniformFloat2(std::string const& name, glm::vec2 const& value);
    void uploadUniformFloat3(std::string const& name, glm::vec3 const& value);
    void uploadUniformFloat4(std::string const& name, glm::vec4 const& value);
    void uploadUniformMat3(std::string const& name, glm::mat3 const& matrix);
    void uploadUniformMat4(std::string const& name, glm::mat4 const& matrix);

    static auto readFile(std::string const& filepath) -> std::string;
    static auto preProcess(std::string const& source)
        -> std::unordered_map<GLenum, std::string>;
    void compile(std::unordered_map<GLenum, std::string> const& shaderSources);

    uint32_t _rendererID{};
    std::string _name;
};

}  // namespace moci

#endif
