#include "ErrorCallback.hpp"

#include <fmt/printf.h>
#include <magic_enum.hpp>
#include <zeus/platform.hpp>

namespace atlas::glx
{
    struct ErrorFilters
    {
        ErrorSource source;
        ErrorType type;
        ErrorSeverity severity;
    };

    static ErrorFilters error_filters;

    void APIENTRY openGLErrorCallback(GLenum source,
                                      GLenum type,
                                      GLuint id,
                                      GLenum severity,
                                      GLsizei length,
                                      GLchar const* message,
                                      void const* userParam);

    void initialize_gl_error_callback([[maybe_unused]] ErrorSource source,
                                      [[maybe_unused]] ErrorType type,
                                      [[maybe_unused]] ErrorSeverity severity)
    {
#if defined(ZEUS_BUILD_DEBUG)
        if (glDebugMessageCallback)
        {
            fmt::print("OpenGL debug callback available\n");
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(openGLErrorCallback, nullptr);
            GLuint unused_ids{0};
            glDebugMessageControl(
                GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unused_ids, true);

            error_filters.source   = source;
            error_filters.type     = type;
            error_filters.severity = severity;
        }
        else
        {
            fmt::print("OpenGL debug callback not available\n");
        }
#else
        fmt::print("OpenGL debug callback is only available in debug builds\n");
#endif
    }

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
        std::string error_origin;
        switch (source)
        {
        case GL_DEBUG_SOURCE_API:
            if (check(error_filters.source, ErrorSource::api))
            {
                error_origin = "OpenGL API";
                break;
            }

            return;

        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            if (check(error_filters.source, ErrorSource::window_system))
            {
                error_origin = "window system";
                break;
            }

            return;

        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            if (check(error_filters.source, ErrorSource::shader_compiler))
            {
                error_origin = "shader compiler";
                break;
            }

            return;

        case GL_DEBUG_SOURCE_THIRD_PARTY:
            if (check(error_filters.source, ErrorSource::third_party))
            {
                error_origin = "third party";
                break;
            }

            return;

        case GL_DEBUG_SOURCE_APPLICATION:
            if (check(error_filters.source, ErrorSource::application))
            {
                error_origin = "user of application";
                break;
            }

            return;

        case GL_DEBUG_SOURCE_OTHER:
            if (check(error_filters.source, ErrorSource::other))
            {
                error_origin = "other";
                break;
            }

            return;
        }

        std::string error_type;
        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR:
            if (check(error_filters.type, ErrorType::error))
            {
                error_type = "error";
                break;
            }

            return;

        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            if (check(error_filters.type, ErrorType::deprecated_behaviour))
            {
                error_type = "deprecated behaviour";
                break;
            }

            return;

        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            if (check(error_filters.type, ErrorType::UndefinedBehaviour))
            {
                error_type = "undefined behaviour";
                break;
            }

            return;

        case GL_DEBUG_TYPE_PORTABILITY:
            if (check(error_filters.type, ErrorType::portability))
            {
                error_type = "portability";
                break;
            }

            return;

        case GL_DEBUG_TYPE_PERFORMANCE:
            if (check(error_filters.type, ErrorType::performance))
            {
                error_type = "performance";
                break;
            }

            return;

        case GL_DEBUG_TYPE_MARKER:
            if (check(error_filters.type, ErrorType::marker))
            {
                error_type = "marker";
                break;
            }

            return;

        case GL_DEBUG_TYPE_PUSH_GROUP:
            if (check(error_filters.type, ErrorType::push_group))
            {
                error_type = "push group";
                break;
            }

            return;

        case GL_DEBUG_TYPE_POP_GROUP:
            if (check(error_filters.type, ErrorType::pop_group))
            {
                error_type = "pop group";
                break;
            }

            return;

        case GL_DEBUG_TYPE_OTHER:
            if (check(error_filters.type, ErrorType::other))
            {
                error_type = "other";
                break;
            }

            return;
        }

        std::string error_message = fmt::format(
            "{}({}):({}): {}", error_type, error_origin, id, message);

        switch (severity)
        {
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            if (check(error_filters.severity, ErrorSeverity::notification))
            {
                fmt::print(stdout, "OpenGL notification: {}\n", error_message);
            }

            break;

        case GL_DEBUG_SEVERITY_LOW:
            if (check(error_filters.severity, ErrorSeverity::low))
            {
                fmt::print(stdout, "OpenGL warning: {}\n", error_message);
            }

            break;

        case GL_DEBUG_SEVERITY_MEDIUM:
            if (check(error_filters.severity, ErrorSeverity::medium))
            {
                fmt::print(stderr, "OpenGL error: {}\n", error_message);
            }

            break;

        case GL_DEBUG_SEVERITY_HIGH:
            if (check(error_filters.severity, ErrorSeverity::high))
            {
                fmt::print(
                    stderr, "OpenGL critical error: {}\n", error_message);
            }
            break;
        }
    }
} // namespace atlas::glx
