/**
 * \file Mesh.hpp
 * \brief Defines a polygonal mesh.
 */

#ifndef ATLAS_INCLUDE_ATLAS_PRIMITIVES_MESH_HPP
#define ATLAS_INCLUDE_ATLAS_PRIMITIVES_MESH_HPP

#pragma once

#include "Primitive.hpp"

#include <string>

namespace atlas
{
    namespace primitives
    {
        /**
         * \class Mesh
         * \brief Generates a polygonal mesh.
         * 
         * Creates a mesh that can be constructed from a file using 
         * loadFromFile. Alternatively the mesh can be manually constructed
         * by modifying the appropriate buffers using the appropriate functions
         * from the Primitive base class.
         */
        class Mesh : public Primitive
        {
        public:
            /**
             * Empty constructor. Note that this does not initialize any 
             * data.
             */
            Mesh();

            /**
             * Standard copy constructor.
             */
            Mesh(Mesh const& m) = default;

            /**
             * Standard destructor.
             */
            ~Mesh();

            /**
             * Loads a mesh from the provided file. This automatically reads
             * in the obj file as well as populates the vertex, normal, and
             * index arrays.
             * 
             * \param[in] filename The name of the file containing the mesh.
             * 
             * \return True if the file was successfully loaded, false 
             * otherwise.
             */
            bool loadFromFile(std::string const& filename);
        };
    }
}

#endif