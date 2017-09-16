#ifndef ATLAS_INCLUDE_ATLAS_UTILS_MESH_HPP
#define ATLAS_INCLUDE_ATLAS_UTILS_MESH_HPP

#pragma once

#include "atlas/gl/GL.hpp"
#include "atlas/math/Math.hpp"
#include "atlas/core/TinyObjLoader.hpp"

#include <vector>
#include <memory>

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
        };

        class Mesh
        {
        public:
            Mesh();
            ~Mesh() = default;

            static bool fromTriangleSoup(
                std::vector<atlas::math::Point> const& vertices,
                std::vector<GLuint> const& indices,
                Mesh& mesh,
                std::vector<atlas::math::Normal> const& normals = {},
                std::vector<atlas::math::Point2> const& uvs = {});

            static bool fromFile(std::string const& filename, Mesh& mesh,
                std::string const& mtl = "", bool triangulate = true);

            std::vector<atlas::math::Point>& vertices();
            std::vector<atlas::math::Normal>& normals();
            std::vector<atlas::math::Point2>& texCoords();
            std::vector<GLuint>& indices();

            std::vector<Shape>& shapes();
            std::vector<tinyobj::material_t>& materials();

            void saveToFile(std::string const& filename);

        private:
            std::vector<Shape> mShapes;
            std::vector<tinyobj::material_t> mMaterials;

            std::vector<atlas::math::Point> mVertices;
            std::vector<atlas::math::Normal> mNormals;
            std::vector<atlas::math::Point2> mTexCoords;
            std::vector<GLuint> mIndices;
        };
    }
}

#endif