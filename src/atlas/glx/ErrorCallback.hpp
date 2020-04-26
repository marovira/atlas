#pragma once

#include <GL/gl3w.h>

namespace atlas::glx
{
    enum class ErrorSource : unsigned short
    {
        API            = 0x01,
        WindowSystem   = 0x02,
        ShaderCompiler = 0x04,
        ThirdParty     = 0x08,
        Application    = 0x10,
        Other          = 0x20,
        All            = 0x3F,
        None           = 0x00
    };

    enum class ErrorType : unsigned short
    {
        Error               = 0x001,
        DeprecatedBehaviour = 0x002,
        UndefinedBehaviour  = 0x004,
        Portability         = 0x008,
        Performance         = 0x010,
        Marker              = 0x020,
        PushGroup           = 0x040,
        PopGroup            = 0x080,
        Other               = 0x100,
        All                 = 0x1FF,
        None                = 0x000
    };

    enum class ErrorSeverity : unsigned short
    {
        Notification = 0x1,
        Low          = 0x2,
        Medium       = 0x4,
        High         = 0x8,
        All          = 0xF,
        None         = 0x0
    };

    void initializeGLCallback(ErrorSource source     = ErrorSource::All,
                              ErrorType type         = ErrorType::All,
                              ErrorSeverity severity = ErrorSeverity::All);

} // namespace atlas::glx
