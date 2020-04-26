#pragma once

#include <GL/gl3w.h>

namespace atlas::glx
{
    enum class ErrorSource : unsigned short
    {
        api             = 0x01,
        window_system   = 0x02,
        shader_compiler = 0x04,
        third_party     = 0x08,
        application     = 0x10,
        other           = 0x20,
        all             = 0x3F,
        none            = 0x00
    };

    enum class ErrorType : unsigned short
    {
        error                = 0x001,
        deprecated_behaviour = 0x002,
        UndefinedBehaviour   = 0x004,
        portability          = 0x008,
        performance          = 0x010,
        marker               = 0x020,
        push_group           = 0x040,
        pop_group            = 0x080,
        other                = 0x100,
        all                  = 0x1FF,
        none                 = 0x000
    };

    enum class ErrorSeverity : unsigned short
    {
        notification = 0x1,
        low          = 0x2,
        medium       = 0x4,
        high         = 0x8,
        all          = 0xF,
        none         = 0x0
    };

    void
    initialize_gl_error_callback(ErrorSource source     = ErrorSource::all,
                                 ErrorType type         = ErrorType::all,
                                 ErrorSeverity severity = ErrorSeverity::all);

} // namespace atlas::glx
