/**
 * \file Texture.hpp
 * \brief Defines a wrapper for OpenGL texture objects.
 *
 */

#ifndef ATLAS_INCLUDE_ATLAS_GL_TEXTURE_HPP
#define ATLAS_INCLUDE_ATLAS_GL_TEXTURE_HPP

#pragma once

#include "GL.hpp"

#include <memory>

namespace atlas
{
    namespace gl
    {
        /**
         * \class Texture
         * \brief A container for OpenGL texture objects.
         * 
         * This class encapsulates the handle for the texture object,
         * thereby providing simplified access to the most common texture
         * functions.
         */
        class Texture : public Object
        {
        public:
            /**
             * Empty constructor. Noe that this does not generate a texture.
             */
            Texture();

            /**
             * Creates a texture with the specified target. A texture unit
             * can also be provided as an optional parameter. It is set by
             * default to 0.
             * 
             * \param[in] target The type of texture.
             * \param[in] unit The texture unit number to use for binding.
             */
            Texture(GLenum target, GLenum unit = 0);

            /**
             * Move constructor.
             * In order to prevent textures from being accidentally deleted,
             * only move semantics are enabled.
             * 
             * \param[in] rhs The texture to move.
             */
            Texture(Texture&& rhs);

            /**
             * This destroys the texture handle that was created and set it
             * back to 0.
             */
            ~Texture();

            /**
             * Move assignment operator.
             * 
             * \param[in] rhs The texture to move.
             */
            Texture& operator=(Texture&& rhs);

            /**
             * Used in tandem with the \c Texture() constructor.
             * 
             * \param[in] target The type of texture.
             */
            void genTexture(GLenum target);

            /**
             * Sets the texture unit that will be used to bind the texture.
             * 
             * \param[in] unit The texture unit to use.
             */
            void setTextureUnit(GLenum unit);

            /**
             * Binds the texture handle contained by the Texture object to
             * the target specified upon construction or \c genTexture. This
             * also sets the current texture unit to the one specified on
             * construction or \c setTextureUnit.
             */
            void bindTexture();

            /**
             * Unbinds the textur handle contained by the Texture object by
             * binding a null (0) texture to the specified target and unit.
             */
            void unBindTexture();

            /**
             * Simultaneously specify storage for all levels of a 
             * one-dimensional texture.
             * 
             * \param[in] levels The number of texture levels.
             * \param[in] internalFormat Specifies the sized internal format 
             * to be used to store texture image data.
             * \param[in] width Specifies the width of the texture, in texels.
             */
            void texStorage1D(GLsizei levels, GLenum internalFormat, 
                GLsizei width);

            /**
             * Simultaneously specify storage for all levels of a 
             * two-dimensional texture or a one-dimensional array texture.
             * 
             * \param[in] levels The number of texture levels.
             * \param[in] internalFormat Specifies the sized internal format 
             * to be used to store texture image data.
             * \param[in] width Specifies the width of the texture, in texels.
             * \param[in] height Specifies the height of the texture, in texels.
             */
            void texStorage2D(GLsizei levels, GLenum internalFormat,
                GLsizei width, GLsizei height);

            /**
             * Simultaneously specify storage for all levels of a 
             * three-dimensional, two-dimensional array or cube-map array
             * texture.
             * 
             * \param[in] levels The number of texture levels.
             * \param[in] internalFormat Specifies the sized internal format 
             * to be used to store texture image data.
             * \param[in] width Specifies the width of the texture, in texels.
             * \param[in] height Specifies the height of the texture, in texels.
             * \param[in] depth Specifies the depth of the texture, in texels.
             */
            void texStorage3D(GLsizei levels, GLenum internalFormat,
                GLsizei width, GLsizei height, GLsizei depth);

            /**
             * Attach a buffer object's data store to a buffer texture object.
             * 
             * \param[in] internalFormat Specifies the internal format of the 
             * data in the store belonging to buffer.
             * \param[in] buffer Specifies the buffer object whose storage to
             * attach to the active buffer texture.
             * 
             * \note
             * To use this function, the target of the texture must be 
             * \c GL_TEXTURE_BUFFER.
             */
            void texBuffer(GLenum internalFormat, Buffer const& buffer);

            /**
             * Specify a one-dimensional texture image.
             * 
             * \param[in] level Specifies the level-of-detail number. Level 0 
             * is the base image level. Level n is the nth mimmap reduction
             * image.
             * \param[in] internalFormat Specifies the number of colour 
             * components in the texture. Must be one of base internal formats,
             * one of the sized internal formats, or one of the compressed 
             * internal formats.
             * \param[in] width Specifies the width of the texture image. 
             * \param[in] border This value must be 0.
             * \param[in] format Specifies the format of the pixel data.
             * \param[in] type Specifies the data type of pixel data.
             * \param[in] data Specifies a pointer to the image data in memory.
             */
            void texImage1D(GLint level, GLint internalFormat,
                GLsizei width, GLint border, GLenum format, 
                GLenum type, const GLvoid* data);

            /**
             * Specify a two-dimensional texture image.
             * 
             * \param[in] level Specifies the level-of-detail number. Level 0 
             * is the base image level. Level n is the nth mimmap reduction
             * image.
             * \param[in] internalFormat Specifies the number of colour 
             * components in the texture. Must be one of base internal formats,
             * one of the sized internal formats, or one of the compressed 
             * internal formats.
             * \param[in] width Specifies the width of the texture image. 
             * \param[in] height Specifies the height of the texture image, or
             * the number of layers in a texture array.
             * \param[in] border This value must be 0.
             * \param[in] format Specifies the format of the pixel data.
             * \param[in] type Specifies the data type of pixel data.
             * \param[in] data Specifies a pointer to the image data in memory.
             */
            void texImage2D(GLint level, GLint internalFormat, 
                GLsizei width, GLsizei height, GLint border, GLenum format,
                GLenum type, const GLvoid* data);

            /**
             * Specify a three-dimensional texture image.
             * 
             * \param[in] level Specifies the level-of-detail number. Level 0 
             * is the base image level. Level n is the nth mimmap reduction
             * image.
             * \param[in] internalFormat Specifies the number of colour 
             * components in the texture. Must be one of base internal formats,
             * one of the sized internal formats, or one of the compressed 
             * internal formats.
             * \param[in] width Specifies the width of the texture image. 
             * \param[in] height Specifies the height of the texture image, or
             * the number of layers in a texture array.
             * \param[in] depth Specifies the depth of the texture image, or
             * the number of layers in a texture array.
             * \param[in] border This value must be 0.
             * \param[in] format Specifies the format of the pixel data.
             * \param[in] type Specifies the data type of pixel data.
             * \param[in] data Specifies a pointer to the image data in memory.
             */
            void texImage3D(GLint level, GLint internalFormat, GLsizei width,
                GLsizei height, GLsizei depth, GLint border, GLenum format,
                GLenum type, const GLvoid* data);

            /**
             * Set texture parameters.
             * 
             * \param[in] pname Specifies the symbolic name of a single-valued
             * texture parameter.
             * \param[in] param Specifies the value.
             */
            void texParameteri(GLenum pname, GLenum param);

            /**
             * Returns the texture handle.
             * 
             * \return The texture handle.
             */
            GLuint getHandle() const override;

        private:
            struct TextureImpl;
            std::unique_ptr<TextureImpl> mImpl;
        };
    }
}

#endif