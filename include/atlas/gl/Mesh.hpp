#ifndef ATLAS_INCLUDE_ATLAS_GL_MESH_HPP
#define ATLAS_INCLUDE_ATLAS_GL_MESH_HPP

#pragma once

#include "GL.hpp"

#include "atlas/math/Math.hpp"

#include <vector>
#include <memory>

namespace atlas
{
    namespace gl
    {
        class Mesh
        {
        public:
            Mesh();
            Mesh(Mesh&& rhs);

            Mesh& operator=(Mesh&& rhs);
            ~Mesh();

            void updateMesh(std::vector<atlas::math::Point> const& vertices,
                std::vector<atlas::math::Normal> const& normals, 
                std::vector<atlas::math::Point2> const& textures,
                std::vector<GLuint> const& indices);
            void setVertexAttribArrays(std::vector<GLuint> const& arrays);

            void render();

        private:
            struct MeshImpl;
            std::unique_ptr<MeshImpl> mImpl;
        };
    }
}

#endif