#include "shader_parser.hpp"

#include <moci/core/logging.hpp>

enum
{
    GL_FRAGMENT_SHADER = 0x8B30,
    GL_VERTEX_SHADER   = 0x8B31
};

namespace moci {
namespace {
auto shaderTypeFromString(std::string_view type) -> ShaderType
{
    if (type == "vertex") {
        return ShaderType::Vertex;
    }
    if (type == "fragment" || type == "pixel") {
        return ShaderType::Fragment;
    }

    MOCI_CORE_ASSERT(false, "Unknown shader type!");
    return ShaderType::Unknown;
}
}  // namespace

auto ShaderParser::splitSource(std::string const& sources) -> ShaderProgramSource
{
    constexpr char const* typeToken = "#type";
    auto const typeTokenLength      = strlen(typeToken);

    auto shaderSources = ShaderProgramSource{};

    // Start of shader type declaration line
    auto pos = sources.find(typeToken, 0);
    while (pos != std::string::npos) {
        // End of shader type declaration line
        auto const eol = sources.find_first_of("\r\n", pos);
        MOCI_CORE_ASSERT(eol != std::string::npos, "Syntax error");

        // Start of shader type name (after "#type " keyword)
        auto const begin       = pos + typeTokenLength + 1;
        std::string const type = sources.substr(begin, eol - begin);
        MOCI_CORE_ASSERT(
            shaderTypeFromString(type) != ShaderType::Unknown,
            "Invalid shader type specified"
        );

        // Start of shader code after shader type declaration line
        auto nextLinePos = sources.find_first_not_of("\r\n", eol);
        MOCI_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
        // Start of next shader type declaration line
        pos = sources.find(typeToken, nextLinePos);

        auto const source = (pos == std::string::npos)
                              ? sources.substr(nextLinePos)  //
                              : sources.substr(nextLinePos, pos - nextLinePos);

        shaderSources.shaders.push_back(
            ShaderStageSource{shaderTypeFromString(type), source}
        );
    }

    return shaderSources;
}

}  // namespace moci
