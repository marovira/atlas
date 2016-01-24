#ifndef ATLAS_INCLUDE_ATLAS_GL_QUADRICS_QUADRIC_HPP
#define ATLAS_INCLUDE_ATLAS_GL_QUADRICS_QUADRIC_HPP

#pragma once

#include "Quadrics.hpp"

namespace atlas
{
    namespace gl
    {
        namespace quadrics
        {
            class Quadric
            {
            public:
                Quadric();
                virtual ~Quadric();

                virtual void bindBuffers();
                virtual void unBindBuffers();
            };
        }
    }
}

#endif
