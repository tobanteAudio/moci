#include "shader.hpp"

#include <moci/core/strings.hpp>
#include <moci/render/opengl/common/shader_parser.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
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

auto getAttributeLayout(const std::string& src) -> std::vector<ShaderAttribute>
{
    std::istringstream f(src);
    auto result = std::vector<ShaderAttribute>();
    std::string line;
    while (std::getline(f, line))
    {
        auto const found = line.find("attribute");
        if (found != std::string::npos)
        {
            moci::Strings::trim(line);
            auto splits = moci::Strings::split(line, ' ');
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
    _rendererID         = createShader(vertex.source.c_str(), fragment.source.c_str());
}

OpenGLESShader::OpenGLESShader(std::string name, std::string const& vertexSrc, std::string const& fragmentSrc)
    : _rendererID(createShader(vertexSrc.c_str(), fragmentSrc.c_str())), _name(std::move(name))
{
}

OpenGLESShader::~OpenGLESShader() { GLCall(glDeleteProgram(_rendererID)); }

auto OpenGLESShader::parseShader(std::string const& filepath) -> ShaderProgramSource
{
    std::ifstream file(filepath);
    if (file)
    {
        auto ss = std::ostringstream {};
        ss << file.rdbuf();
        auto program = ShaderParser::splitSource(ss.str());
        return program;
    }

    return {};
}

auto OpenGLESShader::createShader(const char* vertexSource, const char* fragmentSource) -> GLint
{
    constexpr auto infoLogLength = 512;
    GLchar infoLog[infoLogLength];
    GLint success = 0;

    // Vertex shader
    GLint vertexShader = 0;
    GLCall(vertexShader = glCreateShader(GL_VERTEX_SHADER));
    GLCall(glShaderSource(vertexShader, 1, &vertexSource, nullptr));
    GLCall(glCompileShader(vertexShader));
    GLCall(glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success));
    if (success == 0)
    {
        GLCall(glGetShaderInfoLog(vertexShader, infoLogLength, nullptr, infoLog));
        MOCI_CORE_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{}\n", infoLog);
    }

    // Fragment shader
    GLint fragmentShader = 0;
    GLCall(fragmentShader = glCreateShader(GL_FRAGMENT_SHADER));
    GLCall(glShaderSource(fragmentShader, 1, &fragmentSource, nullptr));
    GLCall(glCompileShader(fragmentShader));
    GLCall(glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success));
    if (success == 0)
    {
        GLCall(glGetShaderInfoLog(fragmentShader, infoLogLength, nullptr, infoLog));
        MOCI_CORE_ERROR("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n{}\n", infoLog);
    }

    // Link shaders
    GLint shaderProgram = 0;
    GLCall(shaderProgram = glCreateProgram());
    GLCall(glAttachShader(shaderProgram, vertexShader));
    GLCall(glAttachShader(shaderProgram, fragmentShader));

    // Bind attributes
    auto const attributes = getAttributeLayout(vertexSource);
    for (auto const& a : attributes)
    {
        GLCall(glBindAttribLocation(shaderProgram, a.index, a.name.c_str()));
        MOCI_CORE_INFO("Binding attribute {}: \"{}\"", a.index, a.name);
    }

    GLCall(glLinkProgram(shaderProgram));
    GLCall(glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success));
    if (success == 0)
    {
        GLCall(glGetProgramInfoLog(shaderProgram, infoLogLength, nullptr, infoLog));
        MOCI_CORE_ERROR("ERROR::SHADER::PROGRAM::LINKING_FAILED\n{}\n", infoLog);
    }

    GLCall(glDeleteShader(vertexShader));
    GLCall(glDeleteShader(fragmentShader));
    return shaderProgram;
}

void OpenGLESShader::bind() const { GLCall(glUseProgram(_rendererID)); }

void OpenGLESShader::unbind() const { GLCall(glUseProgram(0)); }

void OpenGLESShader::setInt(std::string const& name, int value) { uploadUniformInt(name, value); }

void OpenGLESShader::setInts(std::string const& name, int count, int* values)
{

    uploadUniformInts(name, count, values);
}

void OpenGLESShader::setFloat(std::string const& name, float const value) { uploadUniformFloat(name, value); }

void OpenGLESShader::setFloat2(std::string const& name, const glm::vec2& value) { uploadUniformFloat2(name, value); }

void OpenGLESShader::setFloat3(std::string const& name, const glm::vec3& value) { uploadUniformFloat3(name, value); }

void OpenGLESShader::setFloat4(std::string const& name, const glm::vec4& value) { uploadUniformFloat4(name, value); }

void OpenGLESShader::setMat3(std::string const& name, const glm::mat3& value) { uploadUniformMat3(name, value); }

void OpenGLESShader::setMat4(std::string const& name, const glm::mat4& value) { uploadUniformMat4(name, value); }

auto OpenGLESShader::getLocation(std::string const& name) const -> std::int32_t
{
    return glGetUniformLocation(_rendererID, name.c_str());
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
