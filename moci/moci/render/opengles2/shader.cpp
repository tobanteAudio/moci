#include "shader.hpp"

#include "moci/core/logging.hpp"
#include "moci/core/strings.hpp"
#include "moci/debug/instrumentor.hpp"

#include <fstream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>
#include <utility>

namespace
{
struct ShaderAttribute
{
    std::uint32_t index;
    std::string name;
    std::string type;
};

auto GetAttributeLayout(std::string src) -> std::vector<ShaderAttribute>
{
    std::istringstream f(src);
    auto result = std::vector<ShaderAttribute>();
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
};
}  // namespace
namespace moci
{

OpenGLESShader::OpenGLESShader(std::string const& filepath)
{
    MOCI_PROFILE_FUNCTION();
    auto [vertexSource, fragmentSource] = parseShader(filepath);
    m_RendererID                        = createShader(vertexSource.c_str(), fragmentSource.c_str());
}

OpenGLESShader::OpenGLESShader(std::string name, std::string const& vertexSrc, std::string const& fragmentSrc)
    : m_Name(std::move(name))
{
    MOCI_PROFILE_FUNCTION();
    m_RendererID = createShader(vertexSrc.c_str(), fragmentSrc.c_str());
}

OpenGLESShader::~OpenGLESShader()
{
    MOCI_PROFILE_FUNCTION();
    GLCall(glDeleteProgram(m_RendererID));
}

auto OpenGLESShader::parseShader(std::string const& filepath) -> ShaderProgramSource
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE     = -1,
        VERTEX   = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return {ss[0].str(), ss[1].str()};
}

auto OpenGLESShader::createShader(const char* vertexSource, const char* fragmentSource) -> GLint
{
    constexpr auto INFO_LOG_LENGTH = 512;
    GLchar infoLog[INFO_LOG_LENGTH];
    GLint success;

    // Vertex shader
    GLint vertex_shader;
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
    GLint fragment_shader;
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
    GLint shaderProgram;
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

void OpenGLESShader::Bind() const
{
    MOCI_PROFILE_FUNCTION();

    GLCall(glUseProgram(m_RendererID));
}

void OpenGLESShader::Unbind() const
{
    MOCI_PROFILE_FUNCTION();

    GLCall(glUseProgram(0));
}

void OpenGLESShader::SetInt(std::string const& name, int value)
{
    MOCI_PROFILE_FUNCTION();

    UploadUniformInt(name, value);
}

void OpenGLESShader::SetInts(std::string const& name, int count, int* values)
{
    MOCI_PROFILE_FUNCTION();

    UploadUniformInts(name, count, values);
}

void OpenGLESShader::SetFloat(std::string const& name, float const value)
{
    MOCI_PROFILE_FUNCTION();

    UploadUniformFloat(name, value);
}

void OpenGLESShader::SetFloat2(std::string const& name, const glm::vec2& value)
{
    MOCI_PROFILE_FUNCTION();

    UploadUniformFloat2(name, value);
}

void OpenGLESShader::SetFloat3(std::string const& name, const glm::vec3& value)
{
    MOCI_PROFILE_FUNCTION();

    UploadUniformFloat3(name, value);
}

void OpenGLESShader::SetFloat4(std::string const& name, const glm::vec4& value)
{
    MOCI_PROFILE_FUNCTION();

    UploadUniformFloat4(name, value);
}

void OpenGLESShader::SetMat3(std::string const& name, const glm::mat3& value)
{
    MOCI_PROFILE_FUNCTION();

    UploadUniformMat3(name, value);
}

void OpenGLESShader::SetMat4(std::string const& name, const glm::mat4& value)
{
    MOCI_PROFILE_FUNCTION();

    UploadUniformMat4(name, value);
}

void OpenGLESShader::UploadUniformInt(std::string const& name, int value)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    GLCall(glUniform1i(location, value));
}

void OpenGLESShader::UploadUniformInts(std::string const& name, int count, int* values)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    GLCall(glUniform1iv(location, count, values));
}

void OpenGLESShader::UploadUniformFloat(std::string const& name, float value)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    GLCall(glUniform1f(location, value));
}

void OpenGLESShader::UploadUniformFloat2(std::string const& name, const glm::vec2& value)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    GLCall(glUniform2f(location, value.x, value.y));
}

void OpenGLESShader::UploadUniformFloat3(std::string const& name, const glm::vec3& value)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    GLCall(glUniform3f(location, value.x, value.y, value.z));
}

void OpenGLESShader::UploadUniformFloat4(std::string const& name, const glm::vec4& value)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    GLCall(glUniform4f(location, value.x, value.y, value.z, value.w));
}

void OpenGLESShader::UploadUniformMat3(std::string const& name, const glm::mat3& matrix)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    GLCall(glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix)));
}

void OpenGLESShader::UploadUniformMat4(std::string const& name, const glm::mat4& matrix)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix)));
}

}  // namespace moci