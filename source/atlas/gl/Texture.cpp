#include "atlas/gl/Texture.hpp"
#include "atlas/gl/ErrorCheck.hpp"
#include "atlas/core/Log.hpp"
#include "atlas/core/STB.hpp"
#include "atlas/gl/Buffer.hpp"

namespace atlas
{
    namespace gl
    {
        struct Texture::TextureImpl
        {
            TextureImpl() :
                target(0),
                unit(0),
                handle(0)
            { }

            TextureImpl(TextureImpl const& impl) = default;
            ~TextureImpl() = default;

            GLenum target;
            GLenum unit;
            GLuint handle;
        };

        Texture::Texture() :
            mImpl(std::make_unique<TextureImpl>())
        { }

        Texture::Texture(GLenum target, GLenum unit) :
            mImpl(std::make_unique<TextureImpl>())
        {
            glGenTextures(1, &mImpl->handle);
            mImpl->target = target;
            mImpl->unit = unit;
        }

        Texture::Texture(Texture&& rhs) :
            mImpl(std::make_unique<TextureImpl>(*rhs.mImpl))
        {
            rhs.mImpl->handle = 0;
            rhs.mImpl->target = 0;
            rhs.mImpl->unit = 0;
        }

        Texture& Texture::operator=(Texture&& rhs)
        {
            *mImpl = *rhs.mImpl;
            rhs.mImpl->handle = 0;
            rhs.mImpl->target = 0;
            rhs.mImpl->unit = 0;

            return *this;
        }

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

        void Texture::setTextureUnit(GLenum unit)
        {
            mImpl->unit = unit;
        }

        void Texture::bindTexture()
        {
            glActiveTexture(GL_TEXTURE0 + mImpl->unit);
            glBindTexture(mImpl->target, mImpl->handle);
        }

        void Texture::unBindTexture()
        {
            glActiveTexture(GL_TEXTURE0 + mImpl->unit);
            glBindTexture(mImpl->target, 0);
        }

        void Texture::texStorage1D(GLsizei levels, GLenum internalFormat,
            GLsizei width)
        {
            glTexStorage1D(mImpl->target, levels, internalFormat, width);
        }

        void Texture::texStorage2D(GLsizei levels, GLenum internalFormat,
            GLsizei width, GLsizei height)
        {
            glTexStorage2D(mImpl->target, levels, internalFormat, width,
                height);
        }

        void Texture::texStorage3D(GLsizei levels, GLenum internalFormat,
            GLsizei width, GLsizei height, GLsizei depth)
        {
            glTexStorage3D(mImpl->target, levels, internalFormat, width, 
                height, depth);
        }

        void Texture::texBuffer(GLenum internalFormat, Buffer const& buffer)
        {
            glTexBuffer(mImpl->target, internalFormat, buffer.getHandle());
        }

        void Texture::texImage1D(GLint level, GLint internalFormat,
            GLsizei width, GLint border, GLenum type, GLenum format, 
            const GLvoid* data)
        {
            glTexImage1D(mImpl->target, level, internalFormat, width, border,
                type, format, data);
        }

        void Texture::texImage2D(GLint level, GLint internalFormat, 
            GLsizei width, GLsizei height, GLint border, GLenum format, 
            GLenum type, const GLvoid* data)
        {
            glTexImage2D(mImpl->target, level, internalFormat, width, height,
                border, format, type, data);
        }

        void Texture::texImage3D(GLint level, GLint internalFormat,
            GLsizei width, GLsizei height, GLsizei depth, GLint border,
            GLenum format, GLenum type, const GLvoid* data)
        {
            glTexImage3D(mImpl->target, level, internalFormat, width, height,
                depth, border, format, type, data);
        }

        void Texture::texParameteri(GLenum pname, GLenum param)
        {
            glTexParameteri(mImpl->target, pname, param);
        }

        GLuint Texture::getHandle() const
        {
            return mImpl->handle;
        }
    }
}