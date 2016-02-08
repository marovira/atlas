#ifndef ATLAS_INCLUDE_ATLAS_GL_BUFFER_HPP
#define ATLAS_INCLUDE_ATLAS_GL_BUFFER_HPP

#pragma once

#include "GL.hpp"
#include "Buffer.hpp"

#include <memory>

namespace atlas
{
    namespace gl
    {
        class Buffer
        {
        public:
            Buffer(GLenum target);
            Buffer(Buffer const& b);
            ~Buffer();

            void bindBuffer();
            void unBindBuffer();

            void bufferData(GLsizeiptr size, const GLvoid* data, 
                GLenum range);
            void bufferSubData(GLintptr offset, GLsizeiptr size, 
                const GLvoid* data);
            void clearBufferData(GLenum internalFormat, GLenum format,
                GLenum type, const void* data);
            void clearBufferSubData(GLenum internalFormat, GLintptr ofset,
                GLintptr size, GLenum format, GLenum type, const void* data);

            void getBufferSubData(GLintptr offset, GLsizeiptr size, 
                GLvoid* data);
            void* mapBuffer(GLenum access);
            GLboolean unMapBuffer();

        private:
            struct BufferImpl;
            std::unique_ptr<BufferImpl> mImpl;

        };
    }
}

#endif