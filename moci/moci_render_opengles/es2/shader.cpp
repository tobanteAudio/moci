#include "moci_render_opengles/es2/shader.hpp"

#include "moci/core/strings.hpp"

#include "moci_render_opengl_common/moci_render_opengl_common.hpp"

#include <fstream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>
#include <utility>

namespace moci
{
namespace
{
struct ShaderAttribute
{
    std::uint32_t index;
    std::string name;
    std::string type;
};

auto GetAttributeLayout(const std::string& src) -> Vector<ShaderAttribute>
{
    std::istringstream f(src);
    auto result = Vector<ShaderAttribute>();
    std::string line;
    while (std::getline(f, line))
    {
        auto const found = line.find("attribute");
        if (found != std::string::npos)
        {
            moci::Strings::Trim(line);
            auto splits = moci::Strings::Split(line, ' ');
            assert(splits.size() == 3);

            auto type = splits[1];
            auto name = splits[2];
            name.erase(std::find_if(name.begin(), name.end(), [](auto ch) { return ch == ';'; }));
            result.push_back({static_cast<std::uint32_t>(result.size()), name, type});
        }
    }

    return result;
}
}  // namespace

OpenGLESShader::OpenGLESShader(std::string const& filepath)
{
    auto const program  = parseShader(filepath);
    auto const vertex   = program.shaders[0];
    auto const fragment = program.shaders[1];
    m_RendererID        = createShader(vertex.source.c_str(), fragment.source.c_str());
}

OpenGLESShader::OpenGLESShader(std::string name, std::string const& vertexSrc, std::string const& fragmentSrc)
    : m_RendererID(createShader(vertexSrc.c_str(), fragmentSrc.c_str())), m_Name(std::move(name))
{
}

OpenGLESShader::~OpenGLESShader() { GLCall(glDeleteProgram(m_RendererID)); }

auto OpenGLESShader::parseShader(std::string const& filepath) -> ShaderProgramSource
{
    std::ifstream file(filepath);
    if (file)
    {
        auto ss = std::ostringstream {};
        ss << file.rdbuf();
        auto program = ShaderParser::SplitSource(ss.str());
        return program;
    }

    return {};
}

auto OpenGLESShader::createShader(const char* vertexSource, const char* fragmentSource) -> GLint
{
    constexpr auto INFO_LOG_LENGTH = 512;
    GLchar infoLog[INFO_LOG_LENGTH];
    GLint success = 0;

    // Vertex shader
    GLint vertex_shader = 0;
    GLCall(vertex_shader = glCreateShader(GL_VERTEX_SHADER));
    GLCall(glShaderSource(vertex_shader, 1, &vertexSource, nullptr));
    GLCall(glCompileShader(vertex_shader));
    GLCall(glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success));
    if (success == 0)
    {
        GLCall(glGetShaderInfoLog(vertex_shader, INFO_LOG_LENGTH, nullptr, infoLog));
        MOCI_CORE_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{}\n", infoLog);
    }

    // Fragment shader
    GLint fragment_shader = 0;
    GLCall(fragment_shader = glCreateShader(GL_FRAGMENT_SHADER));
    GLCall(glShaderSource(fragment_shader, 1, &fragmentSource, nullptr));
    GLCall(glCompileShader(fragment_shader));
    GLCall(glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success));
    if (success == 0)
    {
        GLCall(glGetShaderInfoLog(fragment_shader, INFO_LOG_LENGTH, nullptr, infoLog));
        MOCI_CORE_ERROR("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n{}\n", infoLog);
    }

    // Link shaders
    GLint shaderProgram = 0;
    GLCall(shaderProgram = glCreateProgram());
    GLCall(glAttachShader(shaderProgram, vertex_shader));
    GLCall(glAttachShader(shaderProgram, fragment_shader));

    // Bind attributes
    auto const attributes = GetAttributeLayout(vertexSource);
    for (auto const& a : attributes)
    {
        GLCall(glBindAttribLocation(shaderProgram, a.index, a.name.c_str()));
        MOCI_CORE_INFO("Binding attribute {}: \"{}\"", a.index, a.name);
    }

    GLCall(glLinkProgram(shaderProgram));
    GLCall(glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success));
    if (success == 0)
    {
        GLCall(glGetProgramInfoLog(shaderProgram, INFO_LOG_LENGTH, nullptr, infoLog));
        MOCI_CORE_ERROR("ERROR::SHADER::PROGRAM::LINKING_FAILED\n{}\n", infoLog);
    }

    GLCall(glDeleteShader(vertex_shader));
    GLCall(glDeleteShader(fragment_shader));
    return shaderProgram;
}

void OpenGLESShader::Bind() const { GLCall(glUseProgram(m_RendererID)); }

void OpenGLESShader::Unbind() const { GLCall(glUseProgram(0)); }

void OpenGLESShader::SetInt(std::string const& name, int value) { uploadUniformInt(name, value); }

void OpenGLESShader::SetInts(std::string const& name, int count, int* values)
{

    uploadUniformInts(name, count, values);
}

void OpenGLESShader::SetFloat(std::string const& name, float const value) { uploadUniformFloat(name, value); }

void OpenGLESShader::SetFloat2(std::string const& name, const glm::vec2& value) { uploadUniformFloat2(name, value); }

void OpenGLESShader::SetFloat3(std::string const& name, const glm::vec3& value) { uploadUniformFloat3(name, value); }

void OpenGLESShader::SetFloat4(std::string const& name, const glm::vec4& value) { uploadUniformFloat4(name, value); }

void OpenGLESShader::SetMat3(std::string const& name, const glm::mat3& value) { uploadUniformMat3(name, value); }

void OpenGLESShader::SetMat4(std::string const& name, const glm::mat4& value) { uploadUniformMat4(name, value); }

auto OpenGLESShader::getLocation(std::string const& name) const -> std::int32_t
{
    return glGetUniformLocation(m_RendererID, name.c_str());
}

void OpenGLESShader::uploadUniformInt(std::string const& name, int value) const
{
    GLCall(glUniform1i(getLocation(name), value));
}

void OpenGLESShader::uploadUniformInts(std::string const& name, int count, int* values) const
{
    GLCall(glUniform1iv(getLocation(name), count, values));
}

void OpenGLESShader::uploadUniformFloat(std::string const& name, float value) const
{
    GLCall(glUniform1f(getLocation(name), value));
}

void OpenGLESShader::uploadUniformFloat2(std::string const& name, const glm::vec2& value) const
{
    GLCall(glUniform2f(getLocation(name), value.x, value.y));
}

void OpenGLESShader::uploadUniformFloat3(std::string const& name, const glm::vec3& value) const
{
    GLCall(glUniform3f(getLocation(name), value.x, value.y, value.z));
}

void OpenGLESShader::uploadUniformFloat4(std::string const& name, const glm::vec4& value) const
{
    GLCall(glUniform4f(getLocation(name), value.x, value.y, value.z, value.w));
}

void OpenGLESShader::uploadUniformMat3(std::string const& name, const glm::mat3& matrix) const
{
    GLCall(glUniformMatrix3fv(getLocation(name), 1, GL_FALSE, glm::value_ptr(matrix)));
}

void OpenGLESShader::uploadUniformMat4(std::string const& name, const glm::mat4& matrix) const
{
    GLCall(glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, glm::value_ptr(matrix)));
}

}  // namespace moci