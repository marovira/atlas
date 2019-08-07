#include <atlas/hlr/LoadLibrary.hpp>

#include <atlas/core/Platform.hpp>

#include <fmt/printf.h>

#if defined(ATLAS_PLATFORM_WINDOWS)
#    if defined(ATLAS_BUILD_DEBUG)
#        include "TestLibraryDebugPath.hpp"
#    else
#        include "TestLibraryReleasePath.hpp"
#    endif
#else
#    include "TestLibraryPath.hpp"
#endif

#include <catch2/catch.hpp>

using namespace atlas;

TEST_CASE("Checking library loading", "[hlr]")
{
    const std::string libraryPath{TestLibraryPath};
    void* handle = hlr::loadLibraryHandle(libraryPath);

    REQUIRE(handle != nullptr);

    hlr::unloadLibraryHandle(handle);
}

typedef int (*SumFn)(int, int);

TEST_CASE("Checking raw function loading", "[hlr]")
{
    const std::string libraryPath{TestLibraryPath};
    void* handle = hlr::loadLibraryHandle(libraryPath);

    REQUIRE(handle != nullptr);

    void* sumFn = hlr::loadRawFunctionPtr(handle, "add");
    REQUIRE(sumFn != nullptr);

    SumFn fn = reinterpret_cast<SumFn>(sumFn);
    int r    = fn(1, 2);
    REQUIRE(r == 3);

    hlr::unloadLibraryHandle(handle);
}

TEST_CASE("Checking template function pointer loading", "[hlr]")
{
    const std::string libraryPath{TestLibraryPath};
    void* handle = hlr::loadLibraryHandle(libraryPath);

    REQUIRE(handle != nullptr);

    auto fn = hlr::loadFunctionPtr<SumFn>(handle, "add");

    int r = fn(1, 2);
    REQUIRE(r == 3);

    hlr::unloadLibraryHandle(handle);
}

TEST_CASE("Checking template function loading", "[hlr]")
{
    const std::string libraryPath{TestLibraryPath};
    void* handle = hlr::loadLibraryHandle(libraryPath);

    REQUIRE(handle != nullptr);

    auto fn = hlr::loadFunction<SumFn, int(int, int)>(handle, "add");

    int r = fn(1, 2);
    REQUIRE(r == 3);

    hlr::unloadLibraryHandle(handle);
}
