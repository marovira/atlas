#ifndef ATLAS_INCLUDE_ATLAS_UTILS_MESH_HPP
#define ATLAS_INCLUDE_ATLAS_UTILS_MESH_HPP

#pragma once

#include "atlas/gl/GL.hpp"
#include "atlas/math/Math.hpp"

#include <vector>
#include <memory>

namespace atlas
{
    namespace utils
    {
        class Mesh
        {
        public:
            Mesh();
            Mesh(Mesh&& rhs);

            Mesh& operator=(Mesh&& rhs);
            ~Mesh();

            static Mesh& fromTriangleSoup(
                std::vector<atlas::math::Point> const& vertices,
                std::vector<GLuint> const& indices,
                std::vector<atlas::math::Normal> const& normals = {},
                std::vector<atlas::math::Point2> const& uvs = {});

            std::vector<atlas::math::Point>& vertices();
            std::vector<atlas::math::Normal>& normals();
            std::vector<atlas::math::Point2>& uvs();
            std::vector<GLuint>& indices();

            bool isValid() const;

            void setVertexAttribArrays(std::vector<GLuint> const& arrays);

            void updateMesh();
            void updateIndices();

            void renderMesh();

        private:
            struct MeshImpl;
            std::unique_ptr<MeshImpl> mImpl;

            std::vector<atlas::math::Point> mVertices;
            std::vector<atlas::math::Normal> mNormals;
            std::vector<atlas::math::Point2> mUvs;
            std::vector<GLuint> mIndices;
        };
    }
}

#endif