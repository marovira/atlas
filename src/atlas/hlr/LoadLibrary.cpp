#include "LoadLibrary.hpp"

#include <fmt/printf.h>
#include <zeus/platform.hpp>

#if defined(ZEUS_PLATFORM_WINDOWS)
#    define WIN32_LEAN_AND_MEAN
#    include <windows.h>
#else
#    include <dlfcn.h>
#endif

namespace atlas::hlr
{
#if defined(ZEUS_PLATFORM_WINDOWS)
    void* load_library_handle(std::string const& library_name)
    {
        void* handle = LoadLibrary(library_name.c_str());
        if (handle == nullptr)
        {
            auto error_code = GetLastError();
            fmt::print(stderr,
                       "error: LoadLibrary returned error code {}\n",
                       error_code);
        }

        return handle;
    }

    void* load_raw_function_ptr(void* handle, std::string const& function_name)
    {
        if (handle == nullptr)
        {
            fmt::print(stderr, "error: null library handle\n");
            return nullptr;
        }

        auto hmodule = reinterpret_cast<HMODULE>(handle);

        FARPROC prog_address = GetProcAddress(hmodule, function_name.c_str());
        if (prog_address == nullptr)
        {
            auto error_code = GetLastError();
            fmt::print(stderr,
                       "error: GetProcAddress returned error code {}\n",
                       error_code);
        }

        return prog_address;
    }

    void unload_library_handle(void* handle)
    {
        auto h = reinterpret_cast<HMODULE>(handle);
        FreeLibrary(h);
    }
#else
    void* load_library_handle(std::string const& library_name)
    {
        void* handle = dlopen(library_name.c_str(), RTLD_LAZY);
        if (handle == nullptr)
        {
            auto error_message = dlerror();
            fmt::print(stderr, "error: in dlopen: {}\n", error_message);
        }

        return handle;
    }

    void* load_raw_function_ptr(void* handle, std::string const& function_name)
    {
        if (handle == nullptr)
        {
            fmt::print(stderr, "error: null library handle\n");
            return nullptr;
        }
        void* prog_address = dlsym(handle, function_name.c_str());
        if (prog_address == nullptr)
        {
            auto error_message = dlerror();
            fmt::print(stderr, "error: in dlsym: {}\n", error_message);
        }

        return prog_address;
    }

    void unload_library_handle(void* handle)
    {
        dlclose(handle);
    }
#endif
} // namespace atlas::hlr
