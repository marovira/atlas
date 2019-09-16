#pragma once

#include <atlas/math/Math.hpp>

#include <tiny_obj_loader.h>

#include <optional>
#include <string>
#include <vector>

namespace atlas::utils
{
    struct Vertex
    {
        math::Point position{};
        math::Normal normal{};
        math::Point2 texCoord{};
        std::size_t index{};
        std::size_t faceId{};
    };

    struct Shape
    {
        bool hasNormals{true};
        bool hasTextureCoords{true};
        std::vector<Vertex> vertices{};
        std::vector<std::size_t> indices{};
        std::vector<int> materialIds;
        std::vector<unsigned int> smoothingGroupIds;
    };

    struct ObjMesh
    {
        std::vector<Shape> shapes;
        std::vector<tinyobj::material_t> materials;
    };

    std::optional<ObjMesh> loadObjMesh(std::string const& filename,
                                       std::string const& materialPath = {});
} // namespace atlas::utils
