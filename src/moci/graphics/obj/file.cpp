#include "file.hpp"

#include <moci/core/logging.hpp>
#include <moci/core/scope_guard.hpp>
#include <moci/core/strings.hpp>

#include <fstream>
#include <utility>

namespace moci {

OBJFile::OBJFile(std::string path) : _path(std::move(path)) {}

auto OBJFile::parse() -> bool
{
    std::string line;
    std::ifstream objFile(_path);
    if (!objFile.is_open()) {
        return false;
    }
    auto guard = makeScopeGuard([&]() { objFile.close(); });

    while (getline(objFile, line)) {
        // Remove whitespace
        Strings::trim(line);

        // Ignore empty lines
        if (line.empty()) {
            continue;
        }

        // Ignore comments
        if (line[0] == '#') {
            continue;
        }

        // Ignore g
        if (line[0] == 'g') {
            continue;
        }

        // Vertex position 'v'
        if (line[0] == 'v' && line[1] == ' ') {
            line = line.substr(2);
            _vertices.push_back(parseLineToVec3(line));
            continue;
        }

        // Vertex normal 'vn'
        if (line[0] == 'v' && line[1] == 'n') {
            line = line.substr(3);
            _normals.push_back(parseLineToVec3(line));
            continue;
        }

        // Face 'f'
        if (line[0] == 'f' && line[1] == ' ') {
            line = line.substr(2);
            parseLineToFace(line);
            continue;
        }
    }

    return true;
}

auto OBJFile::parseLineToVec3(std::string& line) -> glm::vec3
{

    auto splits = Strings::split(line, ' ');
    std::for_each(std::begin(splits), std::end(splits), [](auto& split) {
        Strings::trim(split);
    });
    splits.erase(  //
        std::remove_if(
            splits.begin(),  //
            splits.end(),    //
            [](auto const& split) { return split.empty(); }
        ),            //
        splits.end()  //
    );

    MOCI_CORE_ASSERT(splits.size() == 3, "Only vec3 supported for positions");

    float x{};
    float y{};
    float z{};

    try {
        x = std::stof(splits[0]);
        y = std::stof(splits[1]);
        z = std::stof(splits[2]);
    } catch (...) {
        MOCI_CORE_CRITICAL("Exception during string to float conversion");
    }

    return {x, y, z};
}

void OBJFile::parseLineToFace(std::string& line)
{
    auto splits = Strings::split(line, ' ');
    std::for_each(std::begin(splits), std::end(splits), [](auto& split) {
        Strings::trim(split);
    });
    splits.erase(  //
        std::remove_if(
            splits.begin(),  //
            splits.end(),    //
            [](auto const& split) { return split.empty(); }
        ),            //
        splits.end()  //
    );

    MOCI_CORE_ASSERT(splits.size() == 3, "");

    for (auto& split : splits) {
        auto indices = Strings::split(split, '/');
        if (indices.size() == 3) {
            auto const vertexIdx = Strings::toInt(indices[0]);
            // auto const textureIdx = Strings::ToInt(indices[1]);
            auto const normalIdx = Strings::toInt(indices[2]);
            MOCI_CORE_ASSERT(
                vertexIdx.has_value() == true,
                "The vertex position should never be empty"
            );

            auto const vertex  = _vertices.at(vertexIdx.value() - 1);
            auto const normal  = _normals.at(normalIdx.value() - 1);
            auto const texture = glm::vec2{};

            _model.push_back({vertex, normal, texture});
        }

        if (indices.size() == 2) {
            auto const vertexIdx = Strings::toInt(indices[0]);
            // auto const textureIdx = Strings::ToInt(indices[1]);
            MOCI_CORE_ASSERT(
                vertexIdx.has_value() == true,
                "The vertex position should never be empty"
            );

            auto const vertex  = _vertices.at(vertexIdx.value() - 1);
            auto const normal  = glm::vec3{};
            auto const texture = glm::vec2{};

            _model.push_back({vertex, normal, texture});
        }

        if (indices.size() == 1) {
            auto const vertexIdx = Strings::toInt(indices[0]);
            MOCI_CORE_ASSERT(
                vertexIdx.has_value() == true,
                "The vertex position should never be empty"
            );

            auto const vertex  = _vertices.at(vertexIdx.value() - 1);
            auto const normal  = glm::vec3{};
            auto const texture = glm::vec2{};

            _model.push_back({vertex, normal, texture});
        }
    }
}
}  // namespace moci
