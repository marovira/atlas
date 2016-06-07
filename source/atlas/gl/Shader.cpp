#include "atlas/gl/Shader.hpp"
#include "atlas/core/Log.hpp"
#include "atlas/core/Platform.hpp"
#include "atlas/core/Macros.hpp"
#include "atlas/core/Exception.hpp"

#include <iostream>

namespace atlas
{
    namespace gl
    {
        struct Shader::GLShaderImpl
        {
            GLShaderImpl() :
                shaderProgram(0)
            { }

            GLShaderImpl(GLShaderImpl const& impl) = default;

            ~GLShaderImpl()
            { }

            GLShaderImpl* clone() const
            {
                return new GLShaderImpl(*this);
            }

            const GLchar* readShaderSource(std::string const& filename)
            {
                FILE* infile;
#if defined(ATLAS_PLATFORM_WINDOWS)
                fopen_s(&infile, filename.c_str(), "rb");
#else
                infile = fopen(filename.c_str(), "rb");
#endif
                if (!infile)
                {
                    throw core::RuntimeException("Could not open file: " 
                        + filename);
                }

                fseek(infile, 0, SEEK_END);
                int length = ftell(infile);
                fseek(infile, 0, SEEK_SET);

                if (length == 0)
                {
                    fclose(infile);
                    return nullptr;
                }

                GLchar* source = new GLchar[length + 1];
                fread(source, 1, length, infile);
                fclose(infile);
                source[length] = '\0';
                return const_cast<const GLchar*>(source);
            }

            bool compileShader(ShaderInfo& shader)
            {
                if (shader.shaderHandle)
                {
                    glDeleteShader(shader.shaderHandle);
                }

                auto handle = glCreateShader(shader.shaderType);

                const GLchar* source = readShaderSource(shader.shaderFile);
                if (!source)
                {
                    ERROR_LOG_V("File: \"%s\" is empty or cannot be read.",
                        shader.shaderFile.c_str());
                    glDeleteShader(handle);

                    return false;
                }

                glShaderSource(handle, 1, &source, NULL);
                delete[] source;

                glCompileShader(handle);

                GLint compiled;
                glGetShaderiv(handle, GL_COMPILE_STATUS, &compiled);
                if (!compiled)
                {
                    GLsizei len;
                    glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &len);

                    GLchar* log = new GLchar[len + 1];
                    glGetShaderInfoLog(handle, len, &len, log);

                    ERROR_LOG_V(
                        "In file \"%s\": shader compilation failed: %s.",
                        shader.shaderFile.c_str(), log);

                    glDeleteShader(handle);
                    delete[] log;
                    return false;
                }

                glAttachShader(shaderProgram, handle);
                shader.shaderHandle = handle;
                return true;
            }


            bool checkShaderProgram() const
            {
                if (shaderProgram == 0)
                {
                    return false;
                }

#ifdef ATLAS_DEBUG
                if (!glIsProgram(shaderProgram))
                {
                    return false;
                }
#endif

                return true;
            }

            GLint shaderProgram;
            std::vector<ShaderInfo> shaders;
        };

        Shader::Shader() :
            mImpl(new GLShaderImpl)
        { }

        Shader::Shader(std::vector<ShaderInfo> const& shaders) :
            mImpl(new GLShaderImpl)
        {
            mImpl->shaders = shaders;
        }

        Shader::Shader(Shader const& shader) :
            mImpl(shader.mImpl->clone())
        { }

        Shader::~Shader()
        {
            deleteShaders();
        }

        void Shader::setShaders(std::vector<ShaderInfo> const& shaders)
        {
            mImpl->shaders = shaders;
        }

        std::vector<ShaderInfo>& Shader::getShaders() const
        {
            return mImpl->shaders;
        }

        bool Shader::compileShaders(int idx)
        {
            if (mImpl->shaders.empty())
            {
                WARN_LOG("Received empty shader list.");
                return false;
            }

            if (!mImpl->checkShaderProgram())
            {
                mImpl->shaderProgram = glCreateProgram();
            }

            bool ret = true;
            if (idx == -1)
            {
                for (auto shader : mImpl->shaders)
                {
                    ret = mImpl->compileShader(shader);
                }
            }
            else
            {
                if (0 <= idx && idx < mImpl->shaders.size())
                {
                    ret = mImpl->compileShader(mImpl->shaders[idx]);
                }
            }

            return true;
        }

        bool Shader::linkShaders()
        {
            if (!mImpl->shaderProgram)
            {
                ERROR_LOG("Cannot link with an empty program.");
                return false;
            }

            glLinkProgram(mImpl->shaderProgram);

            GLint linked;
            glGetProgramiv(mImpl->shaderProgram, GL_LINK_STATUS, &linked);
            if (!linked)
            {
                GLsizei len;
                glGetShaderiv(mImpl->shaderProgram, GL_INFO_LOG_LENGTH, &len);

                GLchar* log = new GLchar[len + 1];
                glGetShaderInfoLog(mImpl->shaderProgram, len, &len, log);

                ERROR_LOG("Shader linking failed: " + std::string(log));

                deleteShaders();
                delete[] log;
                return false;
            }

            return true;
        }

        void Shader::deleteShaders()
        {
            for (auto& shader : mImpl->shaders)
            {
                glDetachShader(mImpl->shaderProgram, shader.shaderHandle);
                glDeleteShader(shader.shaderHandle);
            }

            mImpl->shaders.clear();

            if (mImpl->shaderProgram)
            {
                glDeleteProgram(mImpl->shaderProgram);
                mImpl->shaderProgram = 0;
            }
        }

        bool Shader::reloadShaders(int idx)
        {
            bool ret = true;
            ret = compileShaders(idx);
            ret = linkShaders();
            return ret;
        }

        void Shader::bindAttribute(GLuint location, 
            std::string const& name) const
        {
            if (mImpl->checkShaderProgram())
            {
                glBindAttribLocation(mImpl->shaderProgram, location,
                    name.c_str());
            }
        }

        void Shader::enableShaders() const
        {
            if (mImpl->checkShaderProgram())
            {
                glUseProgram(mImpl->shaderProgram);
            }
        }

        void Shader::disableShaders() const
        {
            glUseProgram(0);
        }

        GLint Shader::getShaderProgram() const
        {
            return mImpl->shaderProgram;
        }

        GLint Shader::getUniformVariable(std::string const& name) const
        {
            GLint ret = -1;
            if (mImpl->checkShaderProgram())
            {
                ret = glGetUniformLocation(mImpl->shaderProgram, 
                    name.c_str());
                if (ret == -1)
                {
                    ERROR_LOG(std::string("The uniform location \"") + name +
                        std::string("\" is invalid."));
                }
            }

            return ret;
        }

        GLint Shader::getAttributeVariable(std::string const& name) const
        {
            GLint ret = -1;
            if (mImpl->checkShaderProgram())
            {
                ret = glGetAttribLocation(mImpl->shaderProgram,
                    name.c_str());
                if (ret == -1)
                {
                    ERROR_LOG(std::string("The attribute location \"") + name +
                        std::string("\" is invalid."));
                }
            }

            return ret;
        }
    }
}