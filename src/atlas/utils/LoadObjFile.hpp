#pragma once

#include <atlas/math/Math.hpp>
#include <optional>
#include <string>
#include <tiny_obj_loader.h>
#include <vector>

namespace atlas::utils
{
    struct Vertex
    {
        math::Point position{};
        math::Normal normal{};
        math::Point2 tex_coord{};
        std::size_t index{};
        std::size_t face_id{};
    };

    struct Shape
    {
        bool has_normals{true};
        bool has_texture_coords{true};
        std::vector<Vertex> vertices{};
        std::vector<std::size_t> indices{};
        std::vector<int> material_ids;
        std::vector<unsigned int> smoothing_group_ids;
    };

    struct ObjMesh
    {
        std::vector<Shape> shapes;
        std::vector<tinyobj::material_t> materials;
    };

    std::optional<ObjMesh> load_obj_mesh(std::string const& filename,
                                         std::string const& material_path = {});
} // namespace atlas::utils
