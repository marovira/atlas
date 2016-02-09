#ifndef ATLAS_INCLUDE_ATLAS_GL_BUFFER_HPP
#define ATLAS_INCLUDE_ATLAS_GL_BUFFER_HPP

#pragma once

#include "GL.hpp"
#include "Buffer.hpp"

#include <memory>

#define BUFFER_OFFSET(offset) (void*)(offset)

namespace atlas
{
    namespace gl
    {
        class Buffer
        {
        public:
            Buffer();
            Buffer(GLenum target);
            Buffer(Buffer const& b);
            ~Buffer();

            void genBuffer(GLenum target);

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

            void vertexAttribPointer(GLuint index, GLint size,
                GLenum type, GLboolean normalized, GLsizei stride,
                const GLvoid* pointer);

        private:
            struct BufferImpl;
            std::unique_ptr<BufferImpl> mImpl;

        };
    }
}

#endif