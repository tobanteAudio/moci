#pragma once

#include "glm/glm.hpp"

#include <gsl/gsl>

#include <string>
#include <vector>

namespace moci
{
class OBJFile
{
public:
    OBJFile(std::string path);

    bool Parse();

private:
    std::string path_;
    std::vector<glm::vec3> vertices_;
    std::vector<glm::vec3> normals_;
    std::vector<glm::vec3> faces_;
};
}  // namespace moci
