#include "atlas/gl/Buffer.hpp"
#include "atlas/gl/ErrorCheck.hpp"
#include "atlas/core/Log.hpp"

namespace atlas
{
    namespace gl
    {
        struct Buffer::BufferImpl
        {
            BufferImpl() :
                target(0),
                handle(0)
            { }

            BufferImpl(BufferImpl const& impl) = default;

            BufferImpl* clone()
            {
                return new BufferImpl(*this);
            }

            ~BufferImpl()
            { }

            GLenum target;
            GLuint handle;
        };

        Buffer::Buffer() :
            mImpl(new BufferImpl)
        { }

        Buffer::Buffer(GLenum target) :
            mImpl(new BufferImpl)
        {
            glGenBuffers(1, &mImpl->handle);
            mImpl->target = target;
        }

        Buffer::Buffer(Buffer const& b) :
            mImpl(b.mImpl->clone())
        { }

        Buffer::~Buffer()
        {
            glDeleteBuffers(1, &mImpl->handle);
            mImpl->handle = 0;
            mImpl->target = 0;
        }

        void Buffer::genBuffer(GLenum target)
        {
            glGenBuffers(1, &mImpl->handle);
            mImpl->target = target;
        }

        void Buffer::bindBuffer() const
        {
            glBindBuffer(mImpl->target, mImpl->handle);
            GL_ERROR_CHECK();
        }

        void Buffer::unBindBuffer() const
        {
            glBindBuffer(mImpl->target, 0);
        }

        void Buffer::bufferData(GLsizeiptr size, const GLvoid* data,
            GLenum range) const
        {
            glBufferData(mImpl->target, size, data, range);
            GL_ERROR_CHECK();
        }

        void Buffer::bufferSubData(GLintptr offset, GLsizeiptr size,
            const GLvoid* data) const
        {
            glBufferSubData(mImpl->target, offset, size, data);
            GL_ERROR_CHECK();
        }

        void Buffer::clearBufferData(GLenum internalFormat, GLenum format, 
            GLenum type, const void* data) const
        {
            glClearBufferData(mImpl->target, internalFormat, format, 
                type, data);
            GL_ERROR_CHECK();
        }

        void Buffer::clearBufferSubData(GLenum internalFormat, GLintptr offset,
            GLintptr size, GLenum format, GLenum type, const void* data) const
        {
            glClearBufferSubData(mImpl->target, internalFormat, offset, size,
                format, type, data);
            GL_ERROR_CHECK();
        }

        void Buffer::getBufferSubData(GLintptr offset, GLsizeiptr size, 
            GLvoid* data) const
        {
            glGetBufferSubData(mImpl->target, offset, size, data);
            GL_ERROR_CHECK();
        }

        void* Buffer::mapBuffer(GLenum access) const
        {
            void* ret = glMapBuffer(mImpl->target, access);
            GL_ERROR_CHECK();
            return ret;
        }

        GLboolean Buffer::unMapBuffer() const
        {
            GLboolean ret;
            if ((ret = glUnmapBuffer(mImpl->target)) != GL_TRUE)
            {
                ERROR_LOG("Unable to unmap buffer. Data store contents are" +
                    std::string(" undefined"));
            }

            return ret;
        }

        void Buffer::vertexAttribPointer(GLuint index, GLint size,
            GLenum type, GLboolean normalized, GLsizei stride,
            const GLvoid* pointer) const
        {
            glVertexAttribPointer(index, size, type, normalized, stride, 
                pointer);
            GL_ERROR_CHECK();
        }

        void Buffer::bindBufferRange(GLuint index, GLintptr offset, 
            GLsizeiptr size) const
        {
            glBindBufferRange(mImpl->target, index, mImpl->handle, offset,
                size);
            GL_ERROR_CHECK();
        }

        void Buffer::bindBufferBase(GLuint index) const
        {
            glBindBufferBase(mImpl->target, index, mImpl->handle);
            GL_ERROR_CHECK();
        }
    }
}