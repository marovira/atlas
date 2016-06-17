/**
 * \file QuadMesh.hpp
 * \brief Defines a polygonal mesh shaped like a grid.
 */

#ifndef ATLAS_INCLUDE_ATLAS_PRIMITIVES_QUAD_MESH_HPP
#define ATLAS_INCLUDE_ATLAS_PRIMITIVES_QUAD_MESH_HPP

#pragma once

#include "Primitive.hpp"

namespace atlas
{
    namespace primitives
    {
        /**
         * \class QuadMesh
         * \brief Generates a polygonal grid mesh.
         * 
         * Creates a mesh that is shaped like a generic grid
         * with the dimensions specified. Note that the grid is by
         * default centred on the xy-plane about the origin.
         */
        class QuadMesh : public Primitive
        {
        public:
            /**
             * Empty constructor. Initializes a grid of size 10x10. The 
             * tessellation parameter indicates whether the created buffers
             * are made to allow for tessellation shaders (OpenGL 4.1+ 
             * required) or not.
             * 
             * \param[in] tessellatioOn Whether the buffers support 
             * tessellation or not.
             */
            QuadMesh(bool tessellationOn = false);

            /**
             * Creates a grid of the specified dimensions.
             * 
             * \param[in] width The width of the mesh.
             * \param[in] height The height of the mesh.
             * \param[in] tessellationOn Whether the buffers support
             * tessellation or not.
             */
            QuadMesh(int width, int height, float cellWidth,
                bool tessellationOn = false);

            /**
             * Default copy constructor.
             */
            QuadMesh(QuadMesh const& m);

            /**
             * Standard destructor.
             */
            ~QuadMesh();

            void drawPrimitive(GLenum mode) override;


        private:
            struct QuadMeshImpl;
            std::unique_ptr<QuadMeshImpl> mImpl;
        };
    }
}

#endif