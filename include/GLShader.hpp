#ifndef APOLLO_INCLUDE_ATLAS_GL_SHADER_HPP
#define APOLLO_INCLUDE_ATLAS_GL_SHADER_HPP

#pragma once

#include "GLHeader.hpp"
#include <vector>
#include <string>
#include <memory>

#include "GLShaderInfo.hpp"

namespace atlas
{
    class GLShader
    {
    public:
        GLShader();
        ~GLShader();

        void compileShaders(std::vector<GLShaderInfo> const& shaders);
        void linkShaders();
        void deleteShader(std::string const& filename);
        void deleteShaders();

        void bindAttribute(GLuint location, std::string const& name) const;

        void enableShaders() const;
        void disableShaders() const;

        GLint getShaderProgram() const;

        GLint getUniformVariable(std::string const& name) const;
        GLint getAttributeVariable(std::string const& name) const;

    private:
        struct GLShaderImpl;
        std::unique_ptr<GLShaderImpl> mImpl;
    };
}

#endif
