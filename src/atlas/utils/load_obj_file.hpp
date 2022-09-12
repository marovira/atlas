#pragma once

#include <atlas/math/glm.hpp>
#include <optional>
#include <string>
#include <tiny_obj_loader.h>
#include <vector>

namespace atlas::utils
{
    struct Vertex
    {
        glm::vec3 position{};
        glm::vec3 normal{};
        glm::vec2 tex_coord{};
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
