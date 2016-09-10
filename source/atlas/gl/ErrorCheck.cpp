#include "atlas/gl/ErrorCheck.hpp"
#include "atlas/gl/GL.hpp"
#include "atlas/core/Log.hpp"

namespace atlas
{
    namespace gl
    {
        class DebugCallbackSettings
        {
            DebugCallbackSettings() :
                errorSources(ATLAS_GL_ERROR_SOURCE_ALL),
                errorTypes(ATLAS_GL_ERROR_TYPE_ALL),
                errorSeverity(ATLAS_GL_ERROR_SEVERITY_ALL)
            { }

            ~DebugCallbackSettings()
            { }

            DebugCallbackSettings(DebugCallbackSettings&) = delete;
            void operator=(DebugCallbackSettings&) = delete;

        public:
            static DebugCallbackSettings& getInstance()
            {
                static DebugCallbackSettings instance;
                return instance;
            }

            GLuint errorSources;
            GLuint errorTypes;
            GLuint errorSeverity;
        };

        void initializeGLError()
        {
#ifdef ATLAS_DEBUG
            if (glDebugMessageCallback)
            {
                INFO_LOG("OpenGL debug callback available.");
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
                glDebugMessageCallback(openGLErrorCallback, nullptr);
                GLuint unusedIds = 0;
                glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                    GL_DONT_CARE, 0, &unusedIds, true);
            }
            else
            {
                INFO_LOG("OpenGL debug callback is not available.");
            }
#else
            INFO_LOG("OpenGL debug callback only available in debug builds.");
#endif
            DebugCallbackSettings::getInstance();
        }

        void setGLErrorSources(GLuint sources)
        {
            DebugCallbackSettings::getInstance().errorSources = sources;
        }

        void setGLErrorTypes(GLuint types)
        {
            DebugCallbackSettings::getInstance().errorTypes = types;
        }

        void setGLErrorSeverity(GLuint severity)
        {

            DebugCallbackSettings::getInstance().errorSeverity = severity;
        }

        void APIENTRY openGLErrorCallback(GLenum source, GLenum type, GLuint id,
            GLenum severity, GLsizei length, const GLchar* message,
            const void* userParam)
        {
            UNUSED(userParam);
            UNUSED(length);

            DebugCallbackSettings& settings = 
                DebugCallbackSettings::getInstance();

            std::string errorOrigin;
            switch (source)
            {
            case GL_DEBUG_SOURCE_API:
                if (settings.errorSources & ATLAS_GL_ERROR_SOURCE_API)
                {
                    errorOrigin = "OpenGL API";
                    break;
                }

                return;

            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                if (settings.errorSources & ATLAS_GL_ERROR_SOURCE_WINDOW_SYSTEM)
                {
                    errorOrigin = "window system";
                    break;
                }

                return;

            case GL_DEBUG_SOURCE_SHADER_COMPILER:
                if (settings.errorSources &
                    ATLAS_GL_ERROR_SOURCE_SHADER_COMPILER)
                {
                    errorOrigin = "shader compiler";
                    break;
                }

                return;

            case GL_DEBUG_SOURCE_THIRD_PARTY:
                if (settings.errorSources & ATLAS_GL_ERROR_SOURCE_THIRD_PARTY)
                {
                    errorOrigin = "third party";
                    break;
                }

                return;

            case GL_DEBUG_SOURCE_APPLICATION:
                if (settings.errorSources & ATLAS_GL_ERROR_SOURCE_APPLICATION)
                {
                    errorOrigin = "user of application";
                    break;
                }

                return;

            case GL_DEBUG_SOURCE_OTHER:
                if (settings.errorSources & ATLAS_GL_ERROR_SOURCE_OTHER)
                {
                    errorOrigin = "other";
                    break;
                }

                return;
            }

            std::string errorType;
            switch (type)
            {
            case GL_DEBUG_TYPE_ERROR:
                if (settings.errorTypes & ATLAS_GL_ERROR_TYPE_ERROR)
                {
                    errorType = "error";
                    break;
                }

                return;

            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                if (settings.errorTypes &
                    ATLAS_GL_ERROR_TYPE_DEPRECATED_BEHAVIOUR)
                {
                    errorType = "deprecated behaviour";
                    break;
                }

                return;

            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                if (settings.errorTypes &
                    ATLAS_GL_ERROR_TYPE_UNDEFINED_BEHAVIOUR)
                {
                    errorType = "undefined behaviour";
                    break;
                }

                return;

            case GL_DEBUG_TYPE_PORTABILITY:
                if (settings.errorTypes & ATLAS_GL_ERROR_TYPE_PORTABILITY)
                {
                    errorType = "portability";
                    break;
                }

                return;

            case GL_DEBUG_TYPE_PERFORMANCE:
                if (settings.errorTypes & ATLAS_GL_ERROR_TYPE_PERFORMANCE)
                {
                    errorType = "performance";
                    break;
                }

                return;

            case GL_DEBUG_TYPE_MARKER:
                if (settings.errorTypes & ATLAS_GL_ERROR_TYPE_MARKER)
                {
                    errorType = "marker";
                    break;
                }

                return;

            case GL_DEBUG_TYPE_PUSH_GROUP:
                if (settings.errorTypes & ATLAS_GL_ERROR_TYPE_PUSH_GROUP)
                {
                    errorType = "push group";
                    break;
                }

                return;

            case GL_DEBUG_TYPE_POP_GROUP:
                if (settings.errorTypes & ATLAS_GL_ERROR_TYPE_POP_GROUP)
                {
                    errorType = "pop group";
                    break;
                }

                return;

            case GL_DEBUG_TYPE_OTHER:
                if (settings.errorTypes & ATLAS_GL_ERROR_TYPE_OTHER)
                {
                    errorType = "other";
                    break;
                }

                return;
            }

            std::string errorMessage = "[" + errorType + "] : [" +
                errorOrigin + "] : (" + std::to_string(id) + "): " + message;

            switch (severity)
            {
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                if (settings.errorSeverity &
                    ATLAS_GL_ERROR_SEVERITY_NOTIFICATION)
                {
                    INFO_LOG("OpenGL notification: " + errorMessage);
                }

                break;

            case GL_DEBUG_SEVERITY_LOW:
                if (settings.errorSeverity &
                    ATLAS_GL_ERROR_SEVERITY_LOW)
                {
                    WARN_LOG("OpenGL warning: " + errorMessage);
                }

                break;

            case GL_DEBUG_SEVERITY_MEDIUM:
                if (settings.errorSeverity & ATLAS_GL_ERROR_SEVERITY_MEDIUM)
                {
                    ERROR_LOG("OpenGL error: " + errorMessage);
                }

                break;

            case GL_DEBUG_SEVERITY_HIGH:
                if (settings.errorSeverity & ATLAS_GL_ERROR_SEVERITY_HIGH)
                {
                    CRITICAL_LOG("OpenGL critical error: " + errorMessage);
                }

                break;
            }
        }
    }
}