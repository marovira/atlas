/**
 * \file ObjMesh.hpp
 * \brief Defines a polygonal mesh from an object file.
 */

#ifndef ATLAS_INCLUDE_ATLAS_PRIMITIVES_OBJ_MESH_HPP
#define ATLAS_INCLUDE_ATLAS_PRIMITIVES_OBJ_MESH_HPP

#pragma once

#include "Primitive.hpp"

#include "atlas/gl/Shader.hpp"

#include <string>

namespace atlas
{
    namespace primitives
    {
        class ObjMesh
        {
        public:
            ObjMesh();
            ObjMesh(ObjMesh const& m);

            ~ObjMesh();

            bool loadFromFile(std::string const& file, 
                std::string const& mtl = "", bool triangulate = true);
            void setMaterialBufferLocation(GLuint index = 1);

            //bool loadTextures(std::string const& path, int textureUnit);
            void drawMesh();

        private:
            struct ObjMeshImpl;
            std::unique_ptr<ObjMeshImpl> mImpl;
        };
    }
}

#endif