#include "atlas/gl/Texture.hpp"
#include "atlas/gl/ErrorCheck.hpp"
#include "atlas/core/Log.hpp"
#include "atlas/core/STBImage.hpp"
#include "atlas/gl/Buffer.hpp"

namespace atlas
{
    namespace gl
    {
        struct Texture::TextureImpl
        {
            TextureImpl() :
                target(0),
                handle(0)
            { }

            TextureImpl(TextureImpl const& impl) = default;

            ~TextureImpl()
            { }

            TextureImpl* clone()
            {
                return new TextureImpl(*this);
            }

            GLenum target;
            GLuint handle;
        };

        Texture::Texture() :
            mImpl(new TextureImpl)
        { }

        Texture::Texture(GLenum target) :
            mImpl(new TextureImpl)
        {
            glGenTextures(1, &mImpl->handle);
            mImpl->target = target;
        }

        Texture::Texture(Texture const& tex) :
            mImpl(tex.mImpl->clone())
        { }

        Texture::~Texture()
        {
            glDeleteTextures(1, &mImpl->handle);
            mImpl->handle = 0;
            mImpl->target = 0;
        }

        void Texture::genTexture(GLenum target)
        {
            glGenTextures(1, &mImpl->handle);
            mImpl->target = target;
        }

        void Texture::bindTexture()
        {
            glBindTexture(mImpl->target, mImpl->handle);
            GL_ERROR_CHECK();
        }

        void Texture::unBindTexture()
        {
            glBindTexture(mImpl->target, 0);
            GL_ERROR_CHECK();
        }

        void Texture::texStorage1D(GLsizei levels, GLenum internalFormat,
            GLsizei width)
        {
            glTexStorage1D(mImpl->target, levels, internalFormat, width);
            GL_ERROR_CHECK();
        }

        void Texture::texStorage2D(GLsizei levels, GLenum internalFormat,
            GLsizei width, GLsizei height)
        {
            glTexStorage2D(mImpl->target, levels, internalFormat, width,
                height);
            GL_ERROR_CHECK();
        }

        void Texture::texStorage3D(GLsizei levels, GLenum internalFormat,
            GLsizei width, GLsizei height, GLsizei depth)
        {
            glTexStorage3D(mImpl->target, levels, internalFormat, width, 
                height, depth);
            GL_ERROR_CHECK();
        }

        void Texture::texBuffer(GLenum internalFormat, Buffer const& buffer)
        {
            glTexBuffer(mImpl->target, internalFormat, buffer.getHandle());
            GL_ERROR_CHECK();
        }

        GLuint Texture::getHandle() const
        {
            return mImpl->handle;
        }
    }
}