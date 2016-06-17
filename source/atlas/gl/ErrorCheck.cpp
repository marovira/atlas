#include "atlas/gl/ErrorCheck.hpp"
#include "atlas/gl/GL.hpp"
#include "atlas/core/Log.hpp"

namespace atlas
{
    namespace gl
    {
        void checkGLErrors()
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

        void clearGLErrors()
        {
            GLenum err = GL_NO_ERROR;
            while ((err = glGetError()) != GL_NO_ERROR)
            { }
        }

        void openGLErrorCallback(GLenum source, GLenum type, GLuint id,
            GLenum severity, GLsizei length, const GLchar* message,
            const void* userParam)
        {
            std::string errorOrigin;
            switch (source)
            {
            case GL_DEBUG_SOURCE_API:
                errorOrigin = "OpenGL API";
                break;

            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                errorOrigin = "window system";
                break;

            case GL_DEBUG_SOURCE_SHADER_COMPILER:
                errorOrigin = "shader compiler";
                break;

            case GL_DEBUG_SOURCE_THIRD_PARTY:
                errorOrigin = "third party";
                break;

            case GL_DEBUG_SOURCE_APPLICATION:
                errorOrigin = "user of application";
                break;

            case GL_DEBUG_SOURCE_OTHER:
                errorOrigin = "other";
                break;
            }

            std::string errorType;
            switch (type)
            {
            case GL_DEBUG_TYPE_ERROR:
                errorType = "error";
                break;

            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                errorType = "deprecated behaviour";
                break;

            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                errorType = "undefined behaviour";
                break;

            case GL_DEBUG_TYPE_PORTABILITY:
                errorType = "portability";
                break;

            case GL_DEBUG_TYPE_PERFORMANCE:
                errorType = "performance";
                break;

            case GL_DEBUG_TYPE_MARKER:
                errorType = "marker";
                break;

            case GL_DEBUG_TYPE_PUSH_GROUP:
                errorType = "push group";
                break;

            case GL_DEBUG_TYPE_POP_GROUP:
                errorType = "pop group";
                break;

            case GL_DEBUG_TYPE_OTHER:
                errorType = "other";
                break;
            }

            std::string errorMessage = "[" + errorType + "] : [" +
                errorOrigin + "] : (" + std::to_string(id) + "): " + message;

            switch (severity)
            {
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                INFO_LOG("OpenGL notification: " + errorMessage);
                break;

            case GL_DEBUG_SEVERITY_LOW:
                WARN_LOG("OpenGL warning: " + errorMessage);
                break;

            case GL_DEBUG_SEVERITY_MEDIUM:
                ERROR_LOG("OpenGL error: " + errorMessage);
                break;

            case GL_DEBUG_SEVERITY_HIGH:
                CRITICAL_LOG("OpenGL critical error: " + errorMessage);
                break;
            }
        }
    }
}