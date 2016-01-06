#include "atlas/gl/Texture.hpp"
#include "atlas/gl/GL.hpp"
#include "atlas/core/Log.hpp"
#include "atlas/core/GLI.hpp"


namespace atlas
{
    namespace gl
    {
        Texture::Texture() :
            mTextureId(0)
        { }

        Texture::Texture(std::string const& filename) :
            mTextureId(0)
        {
            loadTexture(filename);
        }

        Texture::~Texture()
        {
            deleteTexture();
        }

        void Texture::loadTexture(std::string const& filename)
        {
            USING_GLI_NS;
            USING_GLM_NS;

            texture tex = load(filename);
            if (tex.empty())
            {
                ERROR_LOG("Could not load texture " + filename);
                return;
            }

            gli::gl GL;
            gli::gl::format const format = GL.translate(tex.format());
            GLenum target = GL.translate(tex.target());

            if (mTextureId != 0)
            {
                WARN_LOG(
                    "Texture is not empty. Will destroy previous texture.");
                deleteTexture();
            }

            glGenTextures(1, &mTextureId);
            glBindTexture(target, mTextureId);
            glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
            glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, 0);
            glTexParameteri(target, GL_TEXTURE_SWIZZLE_R, format.Swizzle[0]);
            glTexParameteri(target, GL_TEXTURE_SWIZZLE_G, format.Swizzle[1]);
            glTexParameteri(target, GL_TEXTURE_SWIZZLE_B, format.Swizzle[2]);
            glTexParameteri(target, GL_TEXTURE_SWIZZLE_A, format.Swizzle[3]);

            tvec3<GLsizei> const dimensions(tex.dimensions());
            GLsizei const faceTotal = static_cast<GLsizei>(tex.layers() *
                tex.faces());

            switch (tex.target())
            {
            case TARGET_1D:
                glTexStorage1D(target, static_cast<GLint>(tex.levels()), 
                    format.Internal, dimensions.x);
                break;

            case TARGET_1D_ARRAY:

            case TARGET_2D:

            case TARGET_CUBE:
                glTexStorage2D(target, static_cast<GLint>(tex.levels()),
                    format.Internal, dimensions.x,
                    (tex.target() == TARGET_2D) ? dimensions.y : faceTotal);
                break;

            case TARGET_2D_ARRAY:

            case TARGET_3D:

            case TARGET_CUBE_ARRAY:
                glTexStorage3D(target, static_cast<GLint>(tex.levels()),
                    format.Internal, dimensions.x, dimensions.y,
                    (tex.target() == TARGET_3D) ? dimensions.z : faceTotal);
                break;

            default:
                CRITICAL_LOG("Encountered an unknown target");
                assert(0);
                break;
            }

            for (std::size_t layer = 0; layer < tex.layers(); ++layer)
            {
                for (std::size_t face = 0; face < tex.faces(); ++face)
                {
                    for (std::size_t level = 0; level < tex.levels(); ++level)
                    {
                        GLsizei const layerGL = static_cast<GLsizei>(layer);
                        tvec3<GLsizei> dims(tex.dimensions(level));
                        if (is_target_cube(tex.target()))
                        {
                            target = static_cast<GLenum>(
                                GL_TEXTURE_CUBE_MAP_POSITIVE_X + face);
                        }

                        switch (tex.target())
                        {
                        case TARGET_1D:
                            if (is_compressed(tex.format()))
                            {
                                glCompressedTexSubImage1D(target,
                                    static_cast<GLint>(level), 0, dims.x,
                                    format.Internal, 
                                    static_cast<GLsizei>(tex.size(level)),
                                    tex.data(layer, face, level));
                            }
                            else
                            {
                                glTexSubImage1D(target,
                                    static_cast<GLint>(level), 0, dims.x,
                                    format.External, format.Type,
                                    tex.data(layer, face, level));
                            }

                            break;

                        case TARGET_1D_ARRAY:

                        case TARGET_2D:

                        case TARGET_CUBE:
                            if (is_compressed(tex.format()))
                            {
                                glCompressedTexSubImage2D(target,
                                    static_cast<GLint>(level), 0, 0,
                                    dims.x,
                                    (tex.target() == TARGET_1D_ARRAY) ? 
                                    layerGL : dims.y,
                                    format.Internal, 
                                    static_cast<GLsizei>(tex.size(level)),
                                    tex.data(layer, face, level));
                            }
                            else
                            {
                                glTexSubImage2D(target,
                                    static_cast<GLint>(level), 0, 0,
                                    dims.x,
                                    (tex.target() == TARGET_1D_ARRAY) ?
                                    layerGL : dims.y, 
                                    format.External, format.Type, 
                                    tex.data(layer, face, level));
                            }

                            break;

                        case TARGET_2D_ARRAY:

                        case TARGET_3D:

                        case TARGET_CUBE_ARRAY:
                            if (is_compressed(tex.format()))
                            {
                                glCompressedTexSubImage3D(target,
                                    static_cast<GLint>(level), 0, 0, 0,
                                    dims.x, dims.y,
                                    (tex.target() == TARGET_3D) ? 
                                    dims.z : layerGL, format.Internal,
                                    static_cast<GLsizei>(tex.size(level)),
                                    tex.data(layer, face, level));
                            }
                            else
                            {
                                glTexSubImage3D(target,
                                    static_cast<GLint>(level), 0, 0, 0,
                                    dims.x, dims.y,
                                    (tex.target() == TARGET_3D) ? 
                                    dims.z : layerGL, format.External, 
                                    format.Type, tex.data(layer, face, level));
                            }
                            
                            break;

                        default:
                            CRITICAL_LOG("Encountered an unknown target");
                            assert(0);
                            break;
                        }
                    }
                }
            }
        }

        GLuint Texture::getTexture() const
        {
            return mTextureId;
        }

        void Texture::deleteTexture()
        {
            glDeleteTextures(1, &mTextureId);
        }
    }
}