#ifndef ATLAS_INCLUDE_ATLAS_GL_SHADER_HPP
#define ATLAS_INCLUDE_ATLAS_GL_SHADER_HPP

#pragma once

#include "GL.hpp"

#include <vector>
#include <string>
#include <memory>

namespace atlas
{
    namespace gl
    {
        struct ShaderInfo
        {
            ShaderInfo() :
                shaderType(0),
                shaderHandle(0)
            { }

            ShaderInfo(GLenum type, std::string const& name) :
                shaderType(type),
                shaderFile(name),
                shaderHandle(0)
            { }

            ShaderInfo(ShaderInfo const& shader, GLuint handle) :
                shaderType(shader.shaderType),
                shaderFile(shader.shaderFile),
                shaderHandle(handle)
            { }

            ShaderInfo(ShaderInfo const& shader) = default;

            GLenum shaderType;
            std::string shaderFile;
            GLuint shaderHandle;
        };

        class GLShader
        {
        public:
            GLShader();
            GLShader(GLShader const& shader);
            ~GLShader();

            void compileShaders(std::vector<ShaderInfo> const& shaders);
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
}

#endif
