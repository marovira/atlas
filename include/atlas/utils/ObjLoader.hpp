/**
 * \file ObjLoader.hpp
 * \brief Defines a way of loading obj files.
 */

#ifndef ATLAS_INCLUDE_ATLAS_UTILS_OBJ_LOADER_HPP
#define ATLAS_INCLUDE_ATLAS_UTILS_OBJ_LOADER_HPP

#pragma once

#include "atlas/core/TinyObjLoader.hpp"
#include "atlas/math/Math.hpp"

#include <string>
#include <vector>

namespace atlas
{
    namespace utils
    {
        struct Face
        {
            Face() :
                start(0),
                size(0),
                materialId(0)
            { }

            Face(std::size_t st, std::size_t sz, int idx) :
                start(st),
                size(sz),
                materialId(idx)
            { }

            std::size_t start;
            std::size_t size;
            int materialId;
        };

        struct Shape
        {
            Shape() :
                dataStart(0)
            { }

            Shape(std::size_t start, std::vector<Face> const& f) :
                dataStart(start),
                faces(f)
            { }

            std::size_t dataStart;
            std::vector<Face> faces;

            std::vector<atlas::math::Point> vertices;
            std::vector<atlas::math::Normal> normals;
            std::vector<atlas::math::Point2> texCoords;
        };

        struct Mesh
        {
            Mesh()
            { }

            std::vector<Shape> shapes;
            std::vector<tinyobj::material_t> materials;
        };

        bool loadMeshFromFile(std::string const& file, Mesh& mesh,
            std::string const& mtl = "", bool triangulate = true);
    }
}

#endif