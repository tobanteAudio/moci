#include "moci_render_opengles/es2/shader.hpp"

#include "moci_core/benchmark/profile.hpp"
#include "moci_core/core/logging.hpp"
#include "moci_core/core/strings.hpp"
#include "moci_core/core/vector.hpp"

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

auto GetAttributeLayout(std::string src) -> Vector<ShaderAttribute>
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
    auto const vertex   = program.sources[0];
    auto const fragment = program.sources[1];
    m_RendererID        = createShader(vertex.source.c_str(), fragment.source.c_str());
}

OpenGLESShader::OpenGLESShader(std::string name, std::string const& vertexSrc, std::string const& fragmentSrc)
    : m_Name(std::move(name))
{
    m_RendererID = createShader(vertexSrc.c_str(), fragmentSrc.c_str());
}

OpenGLESShader::~OpenGLESShader() { GLCall(glDeleteProgram(m_RendererID)); }

auto OpenGLESShader::parseShader(std::string const& filepath) -> ShaderProgramSource
{
    std::ifstream stream(filepath);

    enum class GLShaderType
    {
        NONE     = -1,
        VERTEX   = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    GLShaderType type = GLShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = GLShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = GLShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    auto const vertex   = ShaderSource {ShaderType::Vertex, ss[0].str()};
    auto const fragment = ShaderSource {ShaderType::Fragment, ss[1].str()};
    return {{vertex, fragment}};
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

void OpenGLESShader::uploadUniformInt(std::string const& name, int value) const
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    GLCall(glUniform1i(location, value));
}

void OpenGLESShader::uploadUniformInts(std::string const& name, int count, int* values) const
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    GLCall(glUniform1iv(location, count, values));
}

void OpenGLESShader::uploadUniformFloat(std::string const& name, float value) const
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    GLCall(glUniform1f(location, value));
}

void OpenGLESShader::uploadUniformFloat2(std::string const& name, const glm::vec2& value) const
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    GLCall(glUniform2f(location, value.x, value.y));
}

void OpenGLESShader::uploadUniformFloat3(std::string const& name, const glm::vec3& value) const
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    GLCall(glUniform3f(location, value.x, value.y, value.z));
}

void OpenGLESShader::uploadUniformFloat4(std::string const& name, const glm::vec4& value) const
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    GLCall(glUniform4f(location, value.x, value.y, value.z, value.w));
}

void OpenGLESShader::uploadUniformMat3(std::string const& name, const glm::mat3& matrix) const
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    GLCall(glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix)));
}

void OpenGLESShader::uploadUniformMat4(std::string const& name, const glm::mat4& matrix) const
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix)));
}

}  // namespace moci