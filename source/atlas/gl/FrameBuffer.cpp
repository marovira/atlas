#include "atlas/gl/FrameBuffer.hpp"
#include "atlas/gl/ErrorCheck.hpp"
#include "atlas/core/Log.hpp"

namespace atlas
{
    namespace gl
    {
        struct FrameBuffer::FrameBufferImpl
        {
            FrameBufferImpl() :
                handle(0)
            { }

            FrameBufferImpl(FrameBufferImpl const& impl) = default;
            ~FrameBufferImpl() = default;

            GLuint handle;
        };

        FrameBuffer::FrameBuffer() :
            mImpl(std::make_unique<FrameBufferImpl>())
        {
            glGenFramebuffers(1, &mImpl->handle);
        }

        FrameBuffer::FrameBuffer(FrameBuffer&& rhs) :
            mImpl(std::make_unique<FrameBufferImpl>(*rhs.mImpl))
        {
            rhs.mImpl->handle = 0;
        }

        FrameBuffer& FrameBuffer::operator=(FrameBuffer&& rhs)
        {
            *mImpl = *rhs.mImpl;
            rhs.mImpl->handle = 0;

            return *this;
        }

        FrameBuffer::~FrameBuffer()
        {
            glDeleteFramebuffers(1, &mImpl->handle);
        }

        void FrameBuffer::bindBuffer()
        {
            glBindFramebuffer(GL_FRAMEBUFFER, mImpl->handle);
        }

        void FrameBuffer::unBindBuffer()
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void FrameBuffer::bufferStorage(GLenum internalFormat, GLsizei width,
            GLsizei height)
        {
            glRenderbufferStorage(GL_FRAMEBUFFER, internalFormat, width, height);
        }

        void FrameBuffer::getParameter(GLenum param, GLint* value)
        {
            glGetRenderbufferParameteriv(GL_FRAMEBUFFER, param, value);
        }

        void FrameBuffer::texture2D(GLenum attachment, GLenum textarget,
            GLuint texture, GLint level)
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, textarget, 
                texture, level);
        }

        GLuint FrameBuffer::getHandle() const
        {
            return mImpl->handle;
        }

        bool FrameBuffer::checkStatus() const
        {
            return (glCheckFramebufferStatus(GL_FRAMEBUFFER) ==
                GL_FRAMEBUFFER_COMPLETE);
        }
    }
}