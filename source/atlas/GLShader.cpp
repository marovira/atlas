#include "atlas/GLShader.hpp"
#include "atlas/Log.hpp"
#include "atlas/Platform.hpp"

#include <iostream>

namespace atlas
{
    struct GLShader::GLShaderImpl
    {
        GLShaderImpl() :
            shaderProgram(0)
        { }

        ~GLShaderImpl()
        { }

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
                ERROR_LOG("Cannot open file: " + filename);
                return NULL;
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

        GLint shaderProgram;
        std::vector<GLShaderInfo> shaders;
    };

    GLShader::GLShader() :
        mImpl(new GLShaderImpl)
    { }

    GLShader::~GLShader()
    {
        deleteShaders();
    }

    void GLShader::compileShaders(std::vector<GLShaderInfo> const& shaders)
    {
        if (shaders.empty())
        {
            WARN_LOG("Received empty shader list.");
            return;
        }

        mImpl->shaderProgram = glCreateProgram();

        for (auto shader : shaders)
        {
            auto handle = glCreateShader(shader.shaderType);

            const GLchar* source = mImpl->readShaderSource(shader.shaderFile);
            if (!source)
            {
                ERROR_LOG("Cannot compile empty shader.");
                for (auto& delShader : mImpl->shaders)
                {
                    glDeleteShader(delShader.shaderHandle);
                }

                mImpl->shaders.clear();
                glDeleteProgram(mImpl->shaderProgram);
                mImpl->shaderProgram = 0;

                return;
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

                ERROR_LOG("Shader compilation failed: " + std::string(log));
                
                delete[] log;
                return;
            }

            glAttachShader(mImpl->shaderProgram, handle);
            mImpl->shaders.push_back(GLShaderInfo(shader, handle));
        }
    }

    void GLShader::linkShaders()
    {
        if (!mImpl->shaderProgram)
        {
            ERROR_LOG("Cannot link with an empty program.");
            return;
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
            delete[] log;

            for (auto& shader : mImpl->shaders)
            {
                glDeleteShader(shader.shaderHandle);
                shader.shaderHandle = 0;
            }

            mImpl->shaders.clear();
            return;
        }
    }

    void GLShader::deleteShader(std::string const& filename)
    {
        int index = 0;
        for (auto& shader : mImpl->shaders)
        {
            if (filename.compare(shader.shaderFile) == 0)
            {
                glDetachShader(mImpl->shaderProgram, shader.shaderHandle);
                glDeleteShader(shader.shaderHandle);
                mImpl->shaders.erase(mImpl->shaders.begin() + index);
            }
            else
            {
                WARN_LOG("Could not find shader with filename " + filename);
                break;
            }
        }

        // If all shaders have been deleted, then delete the program as well.
        if (mImpl->shaders.empty() && mImpl->shaderProgram)
        {
            glDeleteProgram(mImpl->shaderProgram);
            mImpl->shaderProgram = 0;
        }
    }

    void GLShader::deleteShaders()
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

    void GLShader::bindAttribute(GLuint location, 
        std::string const& name) const
    {
        if (mImpl->shaderProgram)
        {
            glBindAttribLocation(mImpl->shaderProgram, location, name.c_str());
        }
        else
        {
            WARN_LOG(std::string("Cannot bind attribute without a shader") +
                std::string("program. Have you called compileShaders()?"));
        }
    }

    void GLShader::enableShaders() const
    {
        if (mImpl->shaderProgram)
        {
            glUseProgram(mImpl->shaderProgram);
        }
        else
        {
            WARN_LOG(std::string("Cannot enable shaders without a shader") +
                std::string("program. Have you called compileShaders()?"));
        }
    }

    void GLShader::disableShaders() const
    {
        glUseProgram(0);
    }

    GLint GLShader::getShaderProgram() const
    {
        return mImpl->shaderProgram;
    }

    GLint GLShader::getUniformVariable(std::string const& name) const
    {
        if (mImpl->shaderProgram)
        {
            return glGetUniformLocation(mImpl->shaderProgram, name.c_str());
        }
        else
        {
            WARN_LOG(std::string("Cannot access uniform variables without ") +
                std::string("a shader program. Have you called ") +
                std::string("compileShaders()?"));
            return -1;
        }
    }

    GLint GLShader::getAttributeVariable(std::string const& name) const
    {
        if (mImpl->shaderProgram)
        {
            return glGetAttribLocation(mImpl->shaderProgram, name.c_str());
        }
        else
        {
            WARN_LOG(std::string("Cannot access attribute location without ") +
                std::string("a shader program. Have you called ") +
                std::string("compileShaders()?"));
            return -1;
        }
    }
}
