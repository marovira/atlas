#include "atlas/core/Assert.hpp"
#include "atlas/core/Log.hpp"
#include "atlas/core/Platform.hpp"

#if defined (ATLAS_PLATFORM_WINDOWS) && defined(ATLAS_DEBUG)
#include <intrin.h>
#endif


#include <cstdlib>
#include <string>

namespace atlas
{
    namespace core
    {
        namespace Assert
        {
            void assertion(const char* exprString, bool expr, const char* file,
                int line, const char* message)
            {
                if (!expr)
                {
                    ERROR_LOG_V("In file %s:%d", file, line);
                    ERROR_LOG_V("Assert failed: %s", message);
                    ERROR_LOG_V("Expected:    %s", exprString);
#if defined (ATLAS_DEBUG) && defined(ATLAS_PLATFORM_WINDOWS)
                    __debugbreak();
#else
                    abort();
#endif
                }
            }
        }
    }
}