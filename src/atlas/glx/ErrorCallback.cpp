#include "ErrorCallback.hpp"

#include <atlas/core/Platform.hpp>

#include <fmt/printf.h>
#include <magic_enum.hpp>

namespace atlas::glx
{
    struct ErrorFilters
    {
        ErrorSource source;
        ErrorType type;
        ErrorSeverity severity;
    };

    static ErrorFilters gErrorFilters;

    void APIENTRY openGLErrorCallback(GLenum source,
                                      GLenum type,
                                      GLuint id,
                                      GLenum severity,
                                      GLsizei length,
                                      GLchar const* message,
                                      void const* userParam);

    void initializeGLCallback([[maybe_unused]] ErrorSource source,
                              [[maybe_unused]] ErrorType type,
                              [[maybe_unused]] ErrorSeverity severity)
    {
#if defined(ATLAS_BUILD_DEBUG)
        if (glDebugMessageCallback)
        {
            fmt::print("OpenGL debug callback available\n");
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(openGLErrorCallback, nullptr);
            GLuint unusedIds{0};
            glDebugMessageControl(
                GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, true);

            gErrorFilters.source   = source;
            gErrorFilters.type     = type;
            gErrorFilters.severity = severity;
        }
        else
        {
            fmt::print("OpenGL debug callback not available\n");
        }
#else
        fmt::print("OpenGL debug callback is only available in debug builds\n");
#endif
    }

    using namespace atlas::core;

    template<typename T>
    bool check(T a, T b)
    {
        using namespace magic_enum::bitwise_operators;
        T c      = a & b;
        auto res = magic_enum::enum_integer(c);
        return (res != 0);
    }

    void APIENTRY openGLErrorCallback(GLenum source,
                                      GLenum type,
                                      GLuint id,
                                      GLenum severity,
                                      [[maybe_unused]] GLsizei length,
                                      GLchar const* message,
                                      [[maybe_unused]] void const* userParam)
    {
        std::string errorOrigin;
        switch (source)
        {
        case GL_DEBUG_SOURCE_API:
            if (check(gErrorFilters.source, ErrorSource::API))
            {
                errorOrigin = "OpenGL API";
                break;
            }

            return;

        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            if (check(gErrorFilters.source, ErrorSource::WindowSystem))
            {
                errorOrigin = "window system";
                break;
            }

            return;

        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            if (check(gErrorFilters.source, ErrorSource::ShaderCompiler))
            {
                errorOrigin = "shader compiler";
                break;
            }

            return;

        case GL_DEBUG_SOURCE_THIRD_PARTY:
            if (check(gErrorFilters.source, ErrorSource::ThirdParty))
            {
                errorOrigin = "third party";
                break;
            }

            return;

        case GL_DEBUG_SOURCE_APPLICATION:
            if (check(gErrorFilters.source, ErrorSource::Application))
            {
                errorOrigin = "user of application";
                break;
            }

            return;

        case GL_DEBUG_SOURCE_OTHER:
            if (check(gErrorFilters.source, ErrorSource::Other))
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
            if (check(gErrorFilters.type, ErrorType::Error))
            {
                errorType = "error";
                break;
            }

            return;

        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            if (check(gErrorFilters.type, ErrorType::DeprecatedBehaviour))
            {
                errorType = "deprecated behaviour";
                break;
            }

            return;

        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            if (check(gErrorFilters.type, ErrorType::UndefinedBehaviour))
            {
                errorType = "undefined behaviour";
                break;
            }

            return;

        case GL_DEBUG_TYPE_PORTABILITY:
            if (check(gErrorFilters.type, ErrorType::Portability))
            {
                errorType = "portability";
                break;
            }

            return;

        case GL_DEBUG_TYPE_PERFORMANCE:
            if (check(gErrorFilters.type, ErrorType::Performance))
            {
                errorType = "performance";
                break;
            }

            return;

        case GL_DEBUG_TYPE_MARKER:
            if (check(gErrorFilters.type, ErrorType::Marker))
            {
                errorType = "marker";
                break;
            }

            return;

        case GL_DEBUG_TYPE_PUSH_GROUP:
            if (check(gErrorFilters.type, ErrorType::PushGroup))
            {
                errorType = "push group";
                break;
            }

            return;

        case GL_DEBUG_TYPE_POP_GROUP:
            if (check(gErrorFilters.type, ErrorType::PopGroup))
            {
                errorType = "pop group";
                break;
            }

            return;

        case GL_DEBUG_TYPE_OTHER:
            if (check(gErrorFilters.type, ErrorType::Other))
            {
                errorType = "other";
                break;
            }

            return;
        }

        std::string errorMessage =
            fmt::format("{}({}):({}): {}", errorType, errorOrigin, id, message);

        switch (severity)
        {
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            if (check(gErrorFilters.severity, ErrorSeverity::Notification))
            {
                fmt::print(stdout, "OpenGL notification: {}\n", errorMessage);
            }

            break;

        case GL_DEBUG_SEVERITY_LOW:
            if (check(gErrorFilters.severity, ErrorSeverity::Low))
            {
                fmt::print(stdout, "OpenGL warning: {}\n", errorMessage);
            }

            break;

        case GL_DEBUG_SEVERITY_MEDIUM:
            if (check(gErrorFilters.severity, ErrorSeverity::Medium))
            {
                fmt::print(stderr, "OpenGL error: {}\n", errorMessage);
            }

            break;

        case GL_DEBUG_SEVERITY_HIGH:
            if (check(gErrorFilters.severity, ErrorSeverity::High))
            {
                fmt::print(stderr, "OpenGL critical error: {}\n", errorMessage);
            }
            break;
        }
    }
} // namespace atlas::glx
