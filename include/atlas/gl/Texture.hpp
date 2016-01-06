#ifndef ATLAS_INCLUDE_ATLAS_GL_TEXTURE_HPP
#define ATLAS_INCLUDE_ATLAS_GL_TEXTURE_HPP

#pragma once

#include "GL.hpp"

#include <string>

namespace atlas
{
    namespace gl
    {
        class Texture
        {
        public:
            Texture();
            Texture(std::string const& filename);
            ~Texture();

            void loadTexture(std::string const& filename);
            GLuint getTexture() const;
            void deleteTexture();
            
        private:
            GLuint mTextureId;
        };
    }
}

#endif