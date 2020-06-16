#include "moci_render_base/moci_render_base.hpp"

namespace moci
{
class ShaderParser
{

public:
    static ShaderProgramSource SplitSource(std::string source);
};

}  // namespace moci