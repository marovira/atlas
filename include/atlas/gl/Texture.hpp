#ifndef ATLAS_INCLUDE_ATLAS_GL_TEXTURE_HPP
#define ATLAS_INCLUDE_ATLAS_GL_TEXTURE_HPP

#pragma once

#include "GL.hpp"

#include <string>

namespace atlas
{
    namespace gl
    {
        /**
         *	\class Texture
         *	\brief Provides a way for loading textures into OpenGL.
         *	
         *	This class encapsulates the operations of loading textures and
         *	sending them to OpenGL. If the loading operation is successful,
         *	then the texture handle is stored. Otherwise, an error is sent to 
         *	the Log.
         *	
         *	\note
         *	Please be aware that this class supports only .dds and .vtx file
         *	formats for loading. More are planned, and will be added
         *	eventually.
         */
        class Texture
        {
        public:
            /**
             *	Standard constructor.
             */
            Texture();

            /**
             *	Constructs the Texture and loads the file.
             *	
             *	@param[in] filename The name of the file containing the texture
             *	to load.
             */
            Texture(std::string const& filename);

            /**
             *	Standard destructor. This also destroys the texture handle
             */
            ~Texture();

            /**
             *	Loads the specified texture and sends it to OpenGL. If 
             *	the operation encounters an error, it will be sent to the 
             *	Log.
             *	
             *	@param[in] filename The name of the file containing the texture.
             */
            void loadTexture(std::string const& filename);

            /**
             *	Returns the texture handle.
             *	
             *	@return The texture handle.
             */
            GLuint getTexture() const;

            /**
             *	Deletes the current texture handle. This is invoked by the
             *	destructor.
             */
            void deleteTexture();
            
        private:
            GLuint mTextureId;
        };
    }
}

#endif