#pragma once

#include <moci/render/shader.hpp>

namespace moci
{
class ShaderParser
{

public:
    static auto splitSource(std::string const& source) -> ShaderProgramSource;
};

}  // namespace moci