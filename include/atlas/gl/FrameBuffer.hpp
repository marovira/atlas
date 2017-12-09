/**
 * \file FrameBuffer.hpp
 * \brief Defines a wrapper for OpenGL framebuffer objects.
 */

#ifndef ATLAS_INCLUDE_ATLAS_GL_FRAME_BUFFER_HPP
#define ATLAS_INCLUDE_ATLAS_GL_FRAME_BUFFER_HPP

#pragma once

#include "GL.hpp"

#include <memory>

namespace atlas
{
    namespace gl
    {
        class FrameBuffer : public Object
        {
        public:
            FrameBuffer();

            FrameBuffer(FrameBuffer&& rhs);

            ~FrameBuffer();

            FrameBuffer& operator=(FrameBuffer&& rhs);

            void bindBuffer();

            void unBindBuffer();

            void bufferStorage(GLenum internalFormat, GLsizei width, 
                GLsizei height);

            void getParameter(GLenum param, GLint* value);

            void texture2D(GLenum attachment, GLenum textarget, 
                GLuint texture, GLint level);

            bool checkStatus() const;

            GLuint getHandle() const override;

        private:
            struct FrameBufferImpl;
            std::unique_ptr<FrameBufferImpl> mImpl;
        };
    }
}

#endif