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
        struct ShaderFile
        {
            ShaderFile() :
                parent(-1)
            { }

            ShaderFile(std::string const& n, int p, time_t t) :
                name(n),
                parent(p),
                timeStamp(t)
            { }

            std::string name;
            int parent;
            time_t timeStamp;
        };

        struct ShaderUnit
        {
            ShaderUnit() :
                type(0),
                handle(0)
            { }

            ShaderUnit(std::string const& file, GLenum t) :
                filename(file),
                type(t),
                handle(0)
            { }

            std::string filename;
            GLenum type;
            GLuint handle;

            std::vector<ShaderFile> includedFiles;
        };
    }
}

#endif