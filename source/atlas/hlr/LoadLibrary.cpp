#include "atlas/hlr/LoadLibrary.hpp"

#include <atlas/core/Platform.hpp>
#include <fmt/printf.h>

#if defined(ATLAS_PLATFORM_WINDOWS)
#    define WIN32_LEAN_AND_MEAN
#    include <windows.h>
#else
#    include <dlfcn.h>
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
            fmt::print(stderr,
                       "error: LoadLibrary returned error code {}\n",
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

        auto h = reinterpret_cast<HMODULE>(handle);

        FARPROC procAddress = GetProcAddress(h, functionName.c_str());
        if (procAddress == nullptr)
        {
            auto errorCode = GetLastError();
            fmt::print(stderr,
                       "error: GetProcAddress returned error code {}\n",
                       errorCode);
        }

        return procAddress;
    }

    void unloadLibraryHandle(void* handle)
    {
        auto h = reinterpret_cast<HMODULE>(handle);
        FreeLibrary(h);
    }
#else
    void* loadLibraryHandle(std::string const& libraryName)
    {
        void* handle = dlopen(libraryName.c_str(), RTLD_LAZY);
        if (handle == nullptr)
        {
            auto errorMsg = dlerror();
            fmt::print(stderr, "error: in dlopen: {}\n", errorMsg);
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
        void* procAddress = dlsym(handle, functionName.c_str());
        if (procAddress == nullptr)
        {
            auto errorMsg = dlerror();
            fmt::print(stderr, "error: in dlsym: {}\n", errorMsg);
        }

        return procAddress;
    }

    void unloadLibraryHandle(void* handle)
    {
        dlclose(handle);
    }
#endif
} // namespace atlas::hlr
