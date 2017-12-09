#include "atlas/gl/RenderBuffer.hpp"
#include "atlas/gl/ErrorCheck.hpp"
#include "atlas/core/Log.hpp"

namespace atlas
{
    namespace gl
    {
        struct RenderBuffer::RenderBufferImpl
        {
            RenderBufferImpl() :
                handle(0)
            { }

            RenderBufferImpl(RenderBufferImpl const& impl) = default;
            ~RenderBufferImpl() = default;

            GLuint handle;
        };

        RenderBuffer::RenderBuffer() :
            mImpl(std::make_unique<RenderBufferImpl>())
        {
            glGenRenderbuffers(1, &mImpl->handle);
        }

        RenderBuffer::RenderBuffer(RenderBuffer&& rhs) :
            mImpl(std::make_unique<RenderBufferImpl>(*rhs.mImpl))
        {
            rhs.mImpl->handle = 0;
        }

        RenderBuffer& RenderBuffer::operator=(RenderBuffer&& rhs)
        {
            *mImpl = *rhs.mImpl;
            rhs.mImpl->handle = 0;

            return *this;
        }

        RenderBuffer::~RenderBuffer()
        {
            glDeleteFramebuffers(1, &mImpl->handle);
        }

        void RenderBuffer::bindBuffer()
        {
            glBindRenderbuffer(GL_RENDERBUFFER, mImpl->handle);
        }

        void RenderBuffer::unBindBuffer()
        {
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
        }
    }
}