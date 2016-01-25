#ifndef ATLAS_INCLUDE_ATLAS_PRIMITIVES_PRIMITIVE_HPP
#define ATLAS_INCLUDE_ATLAS_PRIMITIVES_PRIMITIVE_HPP

#pragma once

#include "Primitives.hpp"
#include "atlas/gl/GL.hpp"

namespace atlas
{
    namespace primitives
    {
        class Primitive
        {
        public:
            Primitive();
            virtual ~Primitive();

            virtual void createBuffers();
            virtual void drawBuffers();
        };
    }
}

#endif
