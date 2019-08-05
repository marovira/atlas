#pragma once

#include <functional>
#include <string>

namespace atlas::hlr
{
    void* loadLibraryHandle(std::string const& libraryName);
    void* loadRawFunctionPtr(void* handle, std::string const& functionName);
    void unloadLibraryHandle(void* handle);

    template<typename FnType>
    FnType loadFunctionPtr(void* handle, std::string const& functionName)
    {
        return reinterpret_cast<FnType>(
            loadRawFunctionPtr(handle, functionName));
    }

    template<typename FnPtrType, typename FnType>
    std::function<FnType> loadFunction(void* handle,
                                       std::string const& functionName)
    {
        auto fnPtr = loadFunctionPtr<FnPtrType>(handle, functionName);
        return std::function<FnType>{fnPtr};
    }
} // namespace atlas::hlr
