#ifndef ATLAS_INCLUDE_ATLAS_GL_QUADRICS_QUADRIC_HPP
#define ATLAS_INCLUDE_ATLAS_GL_QUADRICS_QUADRIC_HPP

#pragma once

#include "Primitives.hpp"

namespace atlas
{
    namespace primitives
    {
        class Primitive
        {
        public:
            Primitive();
            virtual ~Primitive();

            virtual void bindBuffers();
            virtual void unBindBuffers();
        };
    }
}

#endif
