#ifndef APOLLO_INCLUDE_ATLAS_GL_SHADER_INFO_HPP
#define APOLLO_INCLUDE_ATLAS_GL_SHADER_INFO_HPP

#pragma once

#include "GLHeader.hpp"
#include <string>

namespace atlas
{
    struct GLShaderInfo
    {
        GLShaderInfo() :
            shaderType(0),
            shaderHandle(0)
        { }

        GLShaderInfo(GLenum type, std::string const& name) :
            shaderType(type),
            shaderFile(name),
            shaderHandle(0)
        { }

        GLShaderInfo(GLShaderInfo const& shader, GLuint handle) :
            shaderType(shader.shaderType),
            shaderFile(shader.shaderFile),
            shaderHandle(handle)
        { }

        GLShaderInfo(GLShaderInfo const& shader) = default;

        GLenum shaderType;
        std::string shaderFile;
        GLuint shaderHandle;
    };
}

#endif
