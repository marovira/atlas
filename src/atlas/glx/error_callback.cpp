#include "error_callback.hpp"

#include <fmt/printf.h>
#include <magic_enum.hpp>
#include <zeus/platform.hpp>

namespace atlas::glx
{
    struct ErrorFilters
    {
        zeus::EnumBitfield<ErrorSource> source;
        zeus::EnumBitfield<ErrorType> type;
        zeus::EnumBitfield<ErrorSeverity> severity;
    };

    static ErrorFilters error_filters;

    void APIENTRY openGLErrorCallback(GLenum source,
                                      GLenum type,
                                      GLuint id,
                                      GLenum severity,
                                      GLsizei length,
                                      GLchar const* message,
                                      void const* userParam);

    void initialize_gl_error_callback(
        [[maybe_unused]] InitializeGLErrorCallbackParams const& params)
    {
#if defined(ZEUS_BUILD_DEBUG)
        auto [source, type, severity] = params;

        if (glDebugMessageCallback)
        {
            fmt::print("OpenGL debug callback available\n");
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(openGLErrorCallback, nullptr);
            GLuint unused_ids{0};
            glDebugMessageControl(GL_DONT_CARE,
                                  GL_DONT_CARE,
                                  GL_DONT_CARE,
                                  0,
                                  &unused_ids,
                                  true);

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
            if (error_filters.source & ErrorSource::api)
            {
                error_origin = "OpenGL API";
                break;
            }

            return;

        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            if (error_filters.source & ErrorSource::window_system)
            {
                error_origin = "window system";
                break;
            }

            return;

        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            if (error_filters.source & ErrorSource::shader_compiler)
            {
                error_origin = "shader compiler";
                break;
            }

            return;

        case GL_DEBUG_SOURCE_THIRD_PARTY:
            if (error_filters.source & ErrorSource::third_party)
            {
                error_origin = "third party";
                break;
            }

            return;

        case GL_DEBUG_SOURCE_APPLICATION:
            if (error_filters.source & ErrorSource::application)
            {
                error_origin = "user of application";
                break;
            }

            return;

        case GL_DEBUG_SOURCE_OTHER:
            if (error_filters.source & ErrorSource::other)
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
            if (error_filters.type & ErrorType::error)
            {
                error_type = "error";
                break;
            }

            return;

        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            if (error_filters.type & ErrorType::deprecated_behaviour)
            {
                error_type = "deprecated behaviour";
                break;
            }

            return;

        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            if (error_filters.type & ErrorType::undefined_behaviour)
            {
                error_type = "undefined behaviour";
                break;
            }

            return;

        case GL_DEBUG_TYPE_PORTABILITY:
            if (error_filters.type & ErrorType::portability)
            {
                error_type = "portability";
                break;
            }

            return;

        case GL_DEBUG_TYPE_PERFORMANCE:
            if (error_filters.type & ErrorType::performance)
            {
                error_type = "performance";
                break;
            }

            return;

        case GL_DEBUG_TYPE_MARKER:
            if (error_filters.type & ErrorType::marker)
            {
                error_type = "marker";
                break;
            }

            return;

        case GL_DEBUG_TYPE_PUSH_GROUP:
            if (error_filters.type & ErrorType::push_group)
            {
                error_type = "push group";
                break;
            }

            return;

        case GL_DEBUG_TYPE_POP_GROUP:
            if (error_filters.type & ErrorType::pop_group)
            {
                error_type = "pop group";
                break;
            }

            return;

        case GL_DEBUG_TYPE_OTHER:
            if (error_filters.type & ErrorType::other)
            {
                error_type = "other";
                break;
            }

            return;
        }

        std::string error_message =
            fmt::format("{}({}):({}): {}", error_type, error_origin, id, message);

        switch (severity)
        {
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            if (error_filters.severity & ErrorSeverity::notification)
            {
                fmt::print(stdout, "OpenGL notification: {}\n", error_message);
            }

            break;

        case GL_DEBUG_SEVERITY_LOW:
            if (error_filters.severity & ErrorSeverity::low)
            {
                fmt::print(stdout, "OpenGL warning: {}\n", error_message);
            }

            break;

        case GL_DEBUG_SEVERITY_MEDIUM:
            if (error_filters.severity & ErrorSeverity::medium)
            {
                fmt::print(stderr, "OpenGL error: {}\n", error_message);
            }

            break;

        case GL_DEBUG_SEVERITY_HIGH:
            if (error_filters.severity & ErrorSeverity::high)
            {
                fmt::print(stderr, "OpenGL critical error: {}\n", error_message);
            }
            break;
        }
    }
} // namespace atlas::glx
