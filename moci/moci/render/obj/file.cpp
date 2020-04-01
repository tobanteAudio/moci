#include "moci/render/obj/file.hpp"

#include "moci/core/logging.hpp"
#include "moci/core/scope_guard.hpp"
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
            line = line.substr(2);
            vertices_.push_back(parseLineToVec3(line));
            continue;
        }

        // Vertex normal 'vn'
        if (line[0] == 'v' && line[1] == 'n')
        {
            line = line.substr(3);
            normals_.push_back(parseLineToVec3(line));
            continue;
        }

        // Face 'f'
        if (line[0] == 'f' && line[1] == ' ')
        {
            line = line.substr(2);
            parseLineToFace(line);
            continue;
        }
    }

    return true;
}

glm::vec3 OBJFile::parseLineToVec3(std::string& line)
{

    auto splits = Strings::Split(line, ' ');
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

    return {x, y, z};
}

void OBJFile::parseLineToFace(std::string& line)
{
    auto splits = Strings::Split(line, ' ');
    std::for_each(std::begin(splits), std::end(splits), [](auto& split) { Strings::Trim(split); });
    splits.erase(                                                         //
        std::remove_if(splits.begin(),                                    //
                       splits.end(),                                      //
                       [](auto const& split) { return split.empty(); }),  //
        splits.end()                                                      //
    );

    MOCI_CORE_ASSERT(splits.size() == 3, "");

    for (auto& split : splits)
    {
        auto indices = Strings::Split(split, '/');
        if (indices.size() == 3)
        {
            auto const vertexIdx  = Strings::ToInt(indices[0]);
            auto const textureIdx = Strings::ToInt(indices[1]);
            auto const normalIdx  = Strings::ToInt(indices[2]);
            MOCI_CORE_ASSERT(vertexIdx.has_value() == true, "The vertex position should never be empty");

            auto const vertex  = vertices_.at(vertexIdx.value() - 1);
            auto const normal  = normals_.at(normalIdx.value() - 1);
            auto const texture = glm::vec2 {};

            model_.push_back({vertex, normal, texture});
        }

        if (indices.size() == 2)
        {
            auto const vertexIdx  = Strings::ToInt(indices[0]);
            auto const textureIdx = Strings::ToInt(indices[1]);
            MOCI_CORE_ASSERT(vertexIdx.has_value() == true, "The vertex position should never be empty");

            auto const vertex  = vertices_.at(vertexIdx.value() - 1);
            auto const normal  = glm::vec3 {};
            auto const texture = glm::vec2 {};

            model_.push_back({vertex, normal, texture});
        }

        if (indices.size() == 1)
        {
            auto const vertexIdx = Strings::ToInt(indices[0]);
            MOCI_CORE_ASSERT(vertexIdx.has_value() == true, "The vertex position should never be empty");

            auto const vertex  = vertices_.at(vertexIdx.value() - 1);
            auto const normal  = glm::vec3 {};
            auto const texture = glm::vec2 {};

            model_.push_back({vertex, normal, texture});
        }
    }
}
}  // namespace moci
