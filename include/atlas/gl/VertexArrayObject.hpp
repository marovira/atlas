#ifndef ATLAS_INCLUDE_ATLAS_GL_VERTEX_ARRAY_OBJECT_HPP
#define ATLAS_INCLUDE_ATLAS_GL_VERTEX_ARRAY_OBJECT_HPP

#pragma once

#include "GL.hpp"
#include "Buffer.hpp"

#include <memory>
#include <vector>

namespace atlas
{
    namespace gl
    {
        class VertexArrayObject
        {
        public:
            VertexArrayObject();
            VertexArrayObject(VertexArrayObject const& vao);
            ~VertexArrayObject();

            void bindVertexArray();
            void unBindVertexArray();

            void enableVertexAttribArray(GLuint index);
            void disableVertexAttribArray(GLuint index);

        private:
            struct VertexArrayImpl;
            std::unique_ptr<VertexArrayImpl> mImpl;

        };
    }
}

#endif