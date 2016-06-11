#ifndef ATLAS_INCLUDE_ATLAS_GL_TEXTURE_HPP
#define ATLAS_INCLUDE_ATLAS_GL_TEXTURE_HPP

#pragma once

#include "GL.hpp"

#include <memory>

namespace atlas
{
    namespace gl
    {
        class Texture
        {
        public:
            Texture();
            Texture(GLenum target);
            Texture(Texture const& tex);
            ~Texture();

            void genTexture(GLenum target);

            void bindTexture();
            void unBindTexture();

        private:
            struct TextureImpl;
            std::unique_ptr<TextureImpl> mImpl;
        };
    }
}

#endif