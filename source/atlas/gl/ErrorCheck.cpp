#include "atlas/gl/ErrorCheck.hpp"
#include "atlas/gl/GL.hpp"
#include "atlas/core/Log.hpp"

namespace atlas
{
    namespace gl
    {
        void ErrorCheck::checkGLErrors()
        {
            GLenum err = GL_NO_ERROR;
            while ((err = glGetError()) != GL_NO_ERROR)
            {
                std::string message;
                switch (err)
                {
                case GL_INVALID_ENUM:
                    message = "(1280) Invalid enum";
                    break;

                case GL_INVALID_VALUE:
                    message = "(1281) Invalid value";
                    break;

                case GL_INVALID_OPERATION:
                    message = "(1282) Invalid operation";
                    break;

                case GL_OUT_OF_MEMORY:
                    message = "(1285) Out of memory";
                    break;

                case GL_INVALID_FRAMEBUFFER_OPERATION:
                    message = "(1286) Invalid framebuffer operation";
                    break;

                default:
                    message = "(0) No error.";
                    break;
                }

                ERROR_LOG("GL error: " + message);
            }
        }

        void ErrorCheck::clearGLErrors()
        {
            GLenum err = GL_NO_ERROR;
            while ((err = glGetError()) != GL_NO_ERROR)
            { }
        }
    }
}