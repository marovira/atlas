#ifndef ATLAS_INCLUDE_ATLAS_TEXTURE_HPP
#define ATLAS_INCLUDE_ATLAS_TEXTURE_HPP

#pragma once

#include "GLHeader.hpp"

#include <string>

namespace atlas
{
    class Texture
    {
    public:
        Texture();
        Texture(std::string const& filename);
        ~Texture();

        void loadTexture(std::string const& filename);
        void renderTexture();
        void destroyTexture();

    private:
        GLuint mTextureId;
    };
}

#endif
