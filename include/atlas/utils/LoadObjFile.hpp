#pragma once

#include <atlas/math/Math.hpp>

#include <tiny_obj_loader.h>

#include <optional>
#include <string>
#include <vector>

namespace atlas::utils
{
    struct ObjMesh
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
    };

    std::optional<ObjMesh> loadObjMesh(std::string const& filename);
} // namespace atlas::utils
