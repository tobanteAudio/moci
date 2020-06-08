#pragma once

#include <fstream>
#include <vector>
namespace th
{
inline auto ReadFile(std::string path) -> std::vector<char>
{
    auto file = std::ifstream {path, std::ios::binary | std::ios::ate};
    if (file)
    {
        auto const size = static_cast<size_t>(file.tellg());
        auto content    = std::vector<char> {};
        content.resize(size);
        file.seekg(0);
        file.read(content.data(), size);
        file.close();
        return content;
    }

    return {};
}
}  // namespace th
