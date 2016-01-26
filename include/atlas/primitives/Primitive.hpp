#ifndef ATLAS_INCLUDE_ATLAS_PRIMITIVES_PRIMITIVE_HPP
#define ATLAS_INCLUDE_ATLAS_PRIMITIVES_PRIMITIVE_HPP

#pragma once

#include "Primitives.hpp"
#include "atlas/gl/GL.hpp"
#include "atlas/math/Vector.hpp"

#include <vector>

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
            virtual void drawPrimitive();

        protected:
            virtual void bindBuffers();
            virtual void drawBuffers();
            virtual void unBindBuffers();

            GLuint mVao;
            GLuint mVertexBuffer;
            GLuint mNormalBuffer;
            GLuint mIndexBuffer;

            std::vector<atlas::math::Point> mVertices;
            std::vector<atlas::math::Normal> mNormals;
            std::vector<GLuint> mIndices;
        };
    }
}

#endif
