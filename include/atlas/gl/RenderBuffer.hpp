/**
 * \file RenderBuffer.hpp
 * \brief Defines a warpper for OpenGL renderbuffer objects.
 */

#ifndef ATLAS_INCLUDE_ATLAS_GL_RENDER_BUFFER_HPP
#define ATLAS_INCLUDE_ATLAS_GL_RENDER_BUFFER_HPP

#pragma once

#include "GL.hpp"

#include <memory>

namespace atlas
{
    namespace gl
    {
        class RenderBuffer : public Object
        {
        public:
            RenderBuffer();

            RenderBuffer(RenderBuffer&& rhs);

            ~RenderBuffer();

            RenderBuffer& operator=(RenderBuffer&& rhs);

            void bindBuffer();

            void unBindBuffer();

        private:
            struct RenderBufferImpl;
            std::unique_ptr<RenderBufferImpl> mImpl;
        };
    }
}

#endif