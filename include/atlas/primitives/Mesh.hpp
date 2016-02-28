#ifndef ATLAS_INCLUDE_ATLAS_PRIMITIVES_MESH_HPP
#define ATLAS_INCLUDE_ATLAS_PRIMITIVES_MESH_HPP

#pragma once

#include "Primitive.hpp"

#include <string>

namespace atlas
{
    namespace primitives
    {
        class Mesh : public Primitive
        {
        public:
            Mesh();
            Mesh(Mesh const& m) = default;
            ~Mesh();

            bool loadFromFile(std::string const& filename);
        };
    }
}

#endif