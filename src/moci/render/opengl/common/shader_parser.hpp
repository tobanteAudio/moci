#pragma once

#include "moci/render/shader.hpp"

namespace moci
{
class ShaderParser
{

public:
    static auto SplitSource(std::string const& source) -> ShaderProgramSource;
};

}  // namespace moci