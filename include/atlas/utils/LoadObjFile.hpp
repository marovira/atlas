#pragma once

#include <atlas/math/Math.hpp>

#include <tiny_obj_loader.h>

#include <optional>
#include <string>
#include <vector>

namespace atlas::utils
{
    struct Face
    {
        Face() = default;

        Face(std::size_t st, std::size_t sz, int id) :
            start{st},
            size{sz},
            materialId{id}
        {}

        std::size_t start{};
        std::size_t size{};
        int materialId{};
    };

    struct Shape
    {
        Shape() = default;

        Shape(std::size_t start, std::vector<Face> const& f) :
            dataStart{start},
            faces{f}
        {}

        std::size_t dataStart{};
        std::vector<Face> faces;
    };

    struct Mesh
    {
        std::vector<math::Point> vertices;
        std::vector<math::Normal> normals;
        std::vector<math::Point2> texCoords;
        std::vector<std::uint32_t> indices;
        std::vector<Shape> shapes;
        std::vector<tinyobj::material_t> materials;
    };

    std::optional<Mesh> loadObjMesh(std::string const& fileName);
} // namespace atlas::utils
