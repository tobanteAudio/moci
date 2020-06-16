#include "shader.hpp"

#if defined(MOCI_API_OPENGL_MODERN)
#include "gl4.hpp"
#include "moci_core/moci_core.hpp"

#include <array>
#include <fstream>

#include <glm/gtc/type_ptr.hpp>

namespace moci
{

static GLenum OpenGLShaderTypeFromString(std::string const& type)
{
    if (type == "vertex") return GL_VERTEX_SHADER;
    if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

    MOCI_CORE_ASSERT(false, "Unknown shader type!");
    return 0;
}

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
    m_Name         = filepath.substr(lastSlash, count);
}

OpenGLShader::OpenGLShader(std::string const& name, std::string const& vertexSrc, std::string const& fragmentSrc)
    : m_Name(name)
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

    glDeleteProgram(m_RendererID);
}

std::string OpenGLShader::ReadFile(std::string const& filepath)
{
    MOCI_PROFILE_FUNCTION();

    std::string result;
    std::ifstream in(filepath, std::ios::in | std::ios::binary);
    if (in)
    {
        in.seekg(0, std::ios::end);
        size_t size = in.tellg();
        if (size != -1)
        {
            result.resize(size);
            in.seekg(0, std::ios::beg);
            in.read(&result[0], size);
            in.close();
        }
        else
        {
            MOCI_CORE_ERROR("Could not read from file '{0}'", filepath);
        }
    }
    else
    {
        MOCI_CORE_ERROR("Could not open file '{0}'", filepath);
    }

    return result;
}

std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(std::string const& source)
{
    MOCI_PROFILE_FUNCTION();

    std::unordered_map<GLenum, std::string> shaderSources;

    const char* typeToken  = "#type";
    size_t typeTokenLength = strlen(typeToken);
    size_t pos             = source.find(typeToken, 0);  // Start of shader type declaration line
    while (pos != std::string::npos)
    {
        size_t eol = source.find_first_of("\r\n", pos);  // End of shader type declaration line
        MOCI_CORE_ASSERT(eol != std::string::npos, "Syntax error");
        size_t begin     = pos + typeTokenLength + 1;  // Start of shader type name (after "#type " keyword)
        std::string type = source.substr(begin, eol - begin);
        MOCI_CORE_ASSERT(OpenGLShaderTypeFromString(type), "Invalid shader type specified");

        size_t nextLinePos
            = source.find_first_not_of("\r\n", eol);  // Start of shader code after shader type declaration line
        MOCI_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
        pos = source.find(typeToken, nextLinePos);  // Start of next shader type declaration line

        shaderSources[OpenGLShaderTypeFromString(type)]
            = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
    }

    return shaderSources;
}

void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
{
    MOCI_PROFILE_FUNCTION();

    GLuint program = glCreateProgram();
    MOCI_CORE_ASSERT(shaderSources.size() <= 2, "We only support 2 shaders for now");
    std::array<GLenum, 2> glShaderIDs;
    int glShaderIDIndex = 0;
    for (auto& kv : shaderSources)
    {
        GLenum type               = kv.first;
        std::string const& source = kv.second;

        GLuint shader = glCreateShader(type);

        const GLchar* sourceCStr = source.c_str();
        glShaderSource(shader, 1, &sourceCStr, 0);

        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            Vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(shader);

            MOCI_CORE_ERROR("{0}", infoLog.data());
            MOCI_CORE_ASSERT(false, "Shader compilation failure!");
            break;
        }

        glAttachShader(program, shader);
        glShaderIDs[glShaderIDIndex++] = shader;
    }

    m_RendererID = program;

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
        Vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        // We don't need the program anymore.
        glDeleteProgram(program);

        for (auto id : glShaderIDs) glDeleteShader(id);

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

void OpenGLShader::Bind() const
{
    MOCI_PROFILE_FUNCTION();
    glUseProgram(m_RendererID);
}

void OpenGLShader::Unbind() const
{
    MOCI_PROFILE_FUNCTION();
    glUseProgram(0);
}

void OpenGLShader::SetInt(std::string const& name, int value) { uploadUniformInt(name, value); }
void OpenGLShader::SetInts(std::string const& name, int count, int* values) { uploadUniformInts(name, count, values); }
void OpenGLShader::SetFloat(std::string const& name, float value) { uploadUniformFloat(name, value); }
void OpenGLShader::SetFloat2(std::string const& name, glm::vec2 const& value) { uploadUniformFloat2(name, value); }
void OpenGLShader::SetFloat3(std::string const& name, const glm::vec3& value) { uploadUniformFloat3(name, value); }
void OpenGLShader::SetFloat4(std::string const& name, const glm::vec4& value) { uploadUniformFloat4(name, value); }
void OpenGLShader::SetMat3(std::string const& name, glm::mat3 const& value) { uploadUniformMat3(name, value); }
void OpenGLShader::SetMat4(std::string const& name, const glm::mat4& value) { uploadUniformMat4(name, value); }

void OpenGLShader::uploadUniformInt(std::string const& name, int value)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1i(location, value);
}

void OpenGLShader::uploadUniformInts(std::string const& name, int count, int* values)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1iv(location, count, values);
}

void OpenGLShader::uploadUniformFloat(std::string const& name, float value)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1f(location, value);
}

void OpenGLShader::uploadUniformFloat2(std::string const& name, const glm::vec2& value)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform2f(location, value.x, value.y);
}

void OpenGLShader::uploadUniformFloat3(std::string const& name, const glm::vec3& value)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform3f(location, value.x, value.y, value.z);
}

void OpenGLShader::uploadUniformFloat4(std::string const& name, const glm::vec4& value)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void OpenGLShader::uploadUniformMat3(std::string const& name, const glm::mat3& matrix)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::uploadUniformMat4(std::string const& name, const glm::mat4& matrix)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

}  // namespace moci

#endif