#ifndef ATLAS_INCLUDE_ATLAS_PRIMITIVES_CUBE_HPP
#define ATLAS_INCLUDE_ATLAS_PRIMITIVES_CUBE_HPP

#pragma once

#include "Primitive.hpp"

namespace atlas
{
    namespace primitives
    {
        class Cube : public Primitive
        {
        public:
            Cube(float width = 1.0f, float height = 1.0f, 
                float depth = 1.0f);
            Cube(Cube const& c);
            ~Cube();

        private:
            struct CubeImpl;
            std::unique_ptr<CubeImpl> mImpl;
        };
    }
}

#endif