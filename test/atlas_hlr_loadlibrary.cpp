#include <atlas/hlr/LoadLibrary.hpp>

#include <fmt/printf.h>
#include <zeus/platform.hpp>

#if defined(ZEUS_PLATFORM_WINDOWS)
#    if defined(ZEUS_BUILD_DEBUG)
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
    const std::string library_path{TestLibraryPath};
    void* handle = hlr::load_library_handle(library_path);

    REQUIRE(handle != nullptr);

    hlr::unload_library_handle(handle);
}

typedef int (*SumFn)(int, int);

TEST_CASE("Checking raw function loading", "[hlr]")
{
    const std::string library_path{TestLibraryPath};
    void* handle = hlr::load_library_handle(library_path);

    REQUIRE(handle != nullptr);

    void* sumFn = hlr::load_raw_function_ptr(handle, "add");
    REQUIRE(sumFn != nullptr);

    SumFn fn = reinterpret_cast<SumFn>(sumFn);
    int r    = fn(1, 2);
    REQUIRE(r == 3);

    hlr::unload_library_handle(handle);
}

TEST_CASE("Checking template function pointer loading", "[hlr]")
{
    const std::string library_path{TestLibraryPath};
    void* handle = hlr::load_library_handle(library_path);

    REQUIRE(handle != nullptr);

    auto fn = hlr::load_function_ptr<SumFn>(handle, "add");

    int r = fn(1, 2);
    REQUIRE(r == 3);

    hlr::unload_library_handle(handle);
}

TEST_CASE("Checking template function loading", "[hlr]")
{
    const std::string library_path{TestLibraryPath};
    void* handle = hlr::load_library_handle(library_path);

    REQUIRE(handle != nullptr);

    auto fn = hlr::load_function<SumFn, int(int, int)>(handle, "add");

    int r = fn(1, 2);
    REQUIRE(r == 3);

    hlr::unload_library_handle(handle);
}
