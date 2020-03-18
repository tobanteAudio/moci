#include "moci/render/obj/file.hpp"

#include "moci/core/logging.hpp"
#include "moci/core/scope_close.hpp"
#include "moci/core/strings.hpp"

#include <fstream>
#include <utility>

namespace moci
{

OBJFile::OBJFile(std::string path) : path_(std::move(path)) {}

bool OBJFile::Parse()
{
    std::string line;
    std::ifstream objFile(path_);
    if (!objFile.is_open())
    {
        return false;
    }
    auto guard = MakeScopeGuard([&]() { objFile.close(); });

    while (getline(objFile, line))
    {
        // Remove whitespace
        Strings::Trim(line);

        // Ignore empty lines
        if (line.empty())
        {
            continue;
        }

        // Ignore comments
        if (line[0] == '#')
        {
            continue;
        }

        // Ignore g
        if (line[0] == 'g')
        {
            continue;
        }

        // Vertex position 'v'
        if (line[0] == 'v' && line[1] == ' ')
        {
            auto splits = Strings::Split(line.substr(2), ' ');
            std::for_each(std::begin(splits), std::end(splits), [](auto& split) { Strings::Trim(split); });
            splits.erase(                                                         //
                std::remove_if(splits.begin(),                                    //
                               splits.end(),                                      //
                               [](auto const& split) { return split.empty(); }),  //
                splits.end()                                                      //
            );

            MOCI_CORE_ASSERT(splits.size() == 3, "Only vec3 supported for positions");

            float x {};
            float y {};
            float z {};

            try
            {
                x = std::stof(splits[0]);
                y = std::stof(splits[1]);
                z = std::stof(splits[2]);
            }
            catch (...)
            {
                MOCI_CORE_CRITICAL("Exception during string to float conversion");
            }

            vertices_.emplace_back(x, y, z);
            continue;
        }
    }

    for (auto const& vertex : vertices_)
    {
        MOCI_CORE_INFO("{}, {}, {}", vertex.x, vertex.y, vertex.z);
    }

    return true;
}

}  // namespace moci
