#include "atlas/hlr/LoadLibrary.hpp"

#include <atlas/core/Platform.hpp>
#include <fmt/printf.h>

#if defined(ATLAS_PLATFORM_WINDOWS)
#    define WIN32_LEAN_AND_MEAN
#    include <windows.h>
#endif

namespace atlas::hlr
{
#if defined(ATLAS_PLATFORM_WINDOWS)
    void* loadLibraryHandle(std::string const& libraryName)
    {
        void* handle = LoadLibrary(libraryName.c_str());
        if (handle == nullptr)
        {
            auto errorCode = GetLastError();
            fmt::print(stderr, "error: LoadLibrary returned error code {}\n",
                       errorCode);
        }

        return handle;
    }

    void* loadRawFunctionPtr(void* handle, std::string const& functionName)
    {
        if (handle == nullptr)
        {
            fmt::print(stderr, "error: null library handle\n");
            return nullptr;
        }

        HMODULE h = reinterpret_cast<HMODULE>(handle);

        FARPROC procAddress = GetProcAddress(h, functionName.c_str());
        if (procAddress == nullptr)
        {
            auto errorCode = GetLastError();
            fmt::print(stderr, "error: GetProcAddress returned error code {}\n",
                       errorCode);
        }

        return procAddress;
    }

    void unloadLibraryHandle(void* handle)
    {
        HMODULE h = reinterpret_cast<HMODULE>(handle);
        FreeLibrary(h);
    }
#else
#endif
} // namespace atlas::hlr
