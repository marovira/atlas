#ifndef ATLAS_INCLUDE_ATLAS_GL_VERTEX_ARRAY_OBJECT_HPP
#define ATLAS_INCLUDE_ATLAS_GL_VERTEX_ARRAY_OBJECT_HPP

#pragma once

#include "GL.hpp"
#include "Buffer.hpp"

#include <memory>

namespace atlas
{
    namespace gl
    {
        class VertexArray
        {
        public:
            VertexArray();
            VertexArray(VertexArray const& vao);
            ~VertexArray();

            void bindVertexArray();
            void unBindVertexArray();

            Buffer& addBuffer(GLenum target);
            void deleteBufferAt(int index);

            Buffer& operator[](int index);
            Buffer operator[](int index) const;

            std::vector<Buffer>::iterator begin();
            std::vector<Buffer>::const_iterator begin() const;
            std::vector<Buffer>::iterator end();
            std::vector<Buffer>::const_iterator end() const;

        private:
            struct VertexArrayImpl;
            std::unique_ptr<VertexArrayImpl> mImpl;

        };
    }
}

#endif