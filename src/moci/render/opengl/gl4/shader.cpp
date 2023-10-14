#include "shader.hpp"

#if defined(MOCI_API_OPENGL_MODERN)
#include <moci/core/benchmark/profile.hpp>
#include <moci/render/opengl/common/shader_parser.hpp>
#include <moci/render/opengl/gl4/gl4.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <array>
#include <fstream>
#include <utility>

namespace moci
{

namespace
{
auto OpenGLShaderTypeFromShaderType(ShaderType type) -> GLenum
{
    if (type == ShaderType::Vertex) { return GL_VERTEX_SHADER; }
    if (type == ShaderType::Fragment) { return GL_FRAGMENT_SHADER; }

    MOCI_CORE_ASSERT(false, "Unknown shader type!");
    return 0;
}

}  // namespace
OpenGLShader::OpenGLShader(std::string const& filepath)
{
    MOCI_PROFILE_FUNCTION();

    std::string source = ReadFile(filepath);
    auto shaderSources = PreProcess(source);
    Compile(shaderSources);

    // Extract name from filepath
    auto lastSlash = filepath.find_last_of("/\\");
    lastSlash      = lastSlash == std::string::npos ? 0 : lastSlash + 1;
    auto lastDot   = filepath.rfind('.');
    auto count     = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
    _name          = filepath.substr(lastSlash, count);
}

OpenGLShader::OpenGLShader(std::string name, std::string const& vertexSrc, std::string const& fragmentSrc)
    : _name(std::move(name))
{
    MOCI_PROFILE_FUNCTION();

    std::unordered_map<GLenum, std::string> sources;
    sources[GL_VERTEX_SHADER]   = vertexSrc;
    sources[GL_FRAGMENT_SHADER] = fragmentSrc;
    Compile(sources);
}

OpenGLShader::~OpenGLShader()
{
    MOCI_PROFILE_FUNCTION();

    glDeleteProgram(_rendererID);
}

auto OpenGLShader::ReadFile(std::string const& filepath) -> std::string
{
    MOCI_PROFILE_FUNCTION();

    std::string result;
    std::ifstream file(filepath, std::ios::in | std::ios::binary);
    if (file)
    {
        auto ss = std::ostringstream {};
        ss << file.rdbuf();
        return ss.str();
    }

    return {};
}

auto OpenGLShader::PreProcess(std::string const& source) -> std::unordered_map<GLenum, std::string>
{
    MOCI_PROFILE_FUNCTION();

    auto shaderSources = std::unordered_map<GLenum, std::string> {};
    auto const program = ShaderParser::splitSource(source);
    for (auto const& shader : program.shaders)
    {
        auto const type     = OpenGLShaderTypeFromShaderType(shader.type);
        shaderSources[type] = shader.source;
    }

    return shaderSources;
}

void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
{
    MOCI_PROFILE_FUNCTION();

    GLuint program = glCreateProgram();
    MOCI_CORE_ASSERT(shaderSources.size() <= 2, "We only support 2 shaders for now");
    std::array<GLenum, 2> glShaderIDs {};
    int glShaderIDIndex = 0;
    for (const auto& kv : shaderSources)
    {
        GLenum type               = kv.first;
        std::string const& source = kv.second;

        GLuint shader = glCreateShader(type);

        const GLchar* sourceCStr = source.c_str();
        glShaderSource(shader, 1, &sourceCStr, nullptr);

        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog.data());

            glDeleteShader(shader);

            MOCI_CORE_ERROR("{0}", infoLog.data());
            MOCI_CORE_ASSERT(false, "Shader compilation failure!");
            break;
        }

        glAttachShader(program, shader);
        glShaderIDs[glShaderIDIndex++] = shader;
    }

    _rendererID = program;

    // Link our program
    glLinkProgram(program);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());

        // We don't need the program anymore.
        glDeleteProgram(program);

        for (auto id : glShaderIDs) { glDeleteShader(id); }

        MOCI_CORE_ERROR("{0}", infoLog.data());
        MOCI_CORE_ASSERT(false, "Shader link failure!");
        return;
    }

    for (auto id : glShaderIDs)
    {
        glDetachShader(program, id);
        glDeleteShader(id);
    }
}

void OpenGLShader::bind() const
{
    MOCI_PROFILE_FUNCTION();
    glUseProgram(_rendererID);
}

void OpenGLShader::unbind() const
{
    MOCI_PROFILE_FUNCTION();
    glUseProgram(0);
}

void OpenGLShader::setInt(std::string const& name, int value) { uploadUniformInt(name, value); }
void OpenGLShader::setInts(std::string const& name, int count, int* values) { uploadUniformInts(name, count, values); }
void OpenGLShader::setFloat(std::string const& name, float value) { uploadUniformFloat(name, value); }
void OpenGLShader::setFloat2(std::string const& name, glm::vec2 const& value) { uploadUniformFloat2(name, value); }
void OpenGLShader::setFloat3(std::string const& name, const glm::vec3& value) { uploadUniformFloat3(name, value); }
void OpenGLShader::setFloat4(std::string const& name, const glm::vec4& value) { uploadUniformFloat4(name, value); }
void OpenGLShader::setMat3(std::string const& name, glm::mat3 const& value) { uploadUniformMat3(name, value); }
void OpenGLShader::setMat4(std::string const& name, const glm::mat4& value) { uploadUniformMat4(name, value); }

auto OpenGLShader::getLocation(std::string const& name) const -> std::int32_t
{
    return glGetUniformLocation(_rendererID, name.c_str());
}

void OpenGLShader::uploadUniformInt(std::string const& name, int value) { glUniform1i(getLocation(name), value); }

void OpenGLShader::uploadUniformInts(std::string const& name, int count, int* values)
{
    glUniform1iv(getLocation(name), count, values);
}

void OpenGLShader::uploadUniformFloat(std::string const& name, float value) { glUniform1f(getLocation(name), value); }

void OpenGLShader::uploadUniformFloat2(std::string const& name, const glm::vec2& value)
{
    glUniform2f(getLocation(name), value.x, value.y);
}

void OpenGLShader::uploadUniformFloat3(std::string const& name, const glm::vec3& value)
{
    glUniform3f(getLocation(name), value.x, value.y, value.z);
}

void OpenGLShader::uploadUniformFloat4(std::string const& name, const glm::vec4& value)
{
    glUniform4f(getLocation(name), value.x, value.y, value.z, value.w);
}

void OpenGLShader::uploadUniformMat3(std::string const& name, const glm::mat3& matrix)
{
    glUniformMatrix3fv(getLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::uploadUniformMat4(std::string const& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

}  // namespace moci

#endif