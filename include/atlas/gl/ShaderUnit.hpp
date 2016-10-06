/**
 *	\file ShaderUnit.hpp
 *	\brief Defines the files and units used for shaders.
 */
#ifndef ATLAS_INCLUDE_ATLAS_GL_SHADER_UNIT_HPP
#define ATLAS_INCLUDE_ATLAS_GL_SHADER_UNIT_HPP

#pragma once

#include "GL.hpp"

#include <vector>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>

namespace atlas
{
    namespace gl
    {
        /**
         * \class ShaderFile
         * \brief Contains the information regarding the shader file.
         *
         * This struct is a container for all the necessary information for
         * the shader system to function properly. In particular, it holds the
         * parent of the file (the file that included it, if any) and the 
         * timestamp to support live reloading.
         */
        struct ShaderFile
        {
            /**
             * Empty constructor.
             */
            ShaderFile() :
                parent(-1)
            { }

            /**
             * Constructor for list initialization.
             * 
             * \param[in] n The name of the file.
             * \param[in] p The parent of the file.
             * \param[in] t The timestamp of the file.
             */
            ShaderFile(std::string const& n, int p, time_t t) :
                name(n),
                parent(p),
                timeStamp(t)
            { }

            /**
             * \var name 
             * The name of the file.
             */
            std::string name;

            /**
             * \var parent 
             * The index of the file that includes this one.
             */
            int parent;

            /**
             * \var timeStamp 
             * The timestamp of the file.
             */
            time_t timeStamp;
        };

        /**
         * \class ShaderUnit
         * \brief The equivalent of a shader in OpenGL.
         * 
         * This struct functions as a wrapper for the shader object. It holds
         * the handle, type of shader, and file. In addition, it also contains
         * the list of all the files that are included.
         */
        struct ShaderUnit
        {
            /**
             * Empty constructor.
             */
            ShaderUnit() :
                type(0),
                handle(0),
                isSourceString(false)
            { }

            /**
             * Constructor for list initialization.
             */
            ShaderUnit(std::string const& file, GLenum t) :
                filename(file),
                type(t),
                handle(0),
                isSourceString(false)
            { }

            ShaderUnit(std::string const& source, GLenum t, 
                bool sourceString) :
                filename(source),
                type(t),
                handle(0),
                isSourceString(sourceString)
            { }

            /**
             * \var filename 
             * The name of the shader file.
             */
            std::string filename;

            /**
             * \var type 
             * The type of shader.
             */
            GLenum type;

            /**
             * \var handle 
             * The shader handle.
             */
            GLuint handle;

            /**
             * \var includedFiles 
             * The list of files this shader includes.
             */
            std::vector<ShaderFile> includedFiles;

            bool isSourceString;
        };
    }
}

#endif