#include <sstream>

#include "basic_gl4.hpp"

int main()
{
    auto const shaderSource = std::string {moci::resources::basic_gl4, moci::resources::basic_gl4_len};
    auto line               = std::string {};
    auto input              = std::istringstream {shaderSource};

    while (getline(input, line)) { std::printf("%s\n", line.c_str()); }

    return 0;
}
