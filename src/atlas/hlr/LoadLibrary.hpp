#pragma once

#include <functional>
#include <string>

namespace atlas::hlr
{
    void* load_library_handle(std::string const& library_name);
    void* load_raw_function_ptr(void* handle, std::string const& function_name);
    void unload_library_handle(void* handle);

    template<typename FnType>
    FnType load_function_ptr(void* handle, std::string const& function_name)
    {
        return reinterpret_cast<FnType>(
            load_raw_function_ptr(handle, function_name));
    }

    template<typename FnPtrType, typename FnType>
    std::function<FnType> load_function(void* handle,
                                        std::string const& function_name)
    {
        auto fnPtr = loadFunctionPtr<FnPtrType>(handle, function_name);
        return std::function<FnType>{fnPtr};
    }
} // namespace atlas::hlr
