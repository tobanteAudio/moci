#pragma once

#include "moci_render_base/moci_render_base.hpp"

namespace moci
{
class ShaderParser
{

public:
    static auto SplitSource(std::string const& source) -> ShaderProgramSource;
};

}  // namespace moci