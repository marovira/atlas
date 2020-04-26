#include <atlas/hlr/load_library.hpp>

#include <fmt/printf.h>
#include <zeus/platform.hpp>

#if defined(ZEUS_PLATFORM_WINDOWS)
#    if defined(ZEUS_BUILD_DEBUG)
#        include "test_data/hlr/test_library_debug_path.hpp"
#    else
#        include "test_data/hlr/test_library_release_path.hpp"
#    endif
#else
#    include "test_data/hlr/test_library_path.hpp"
#endif

#include <catch2/catch.hpp>

using namespace atlas;

TEST_CASE("[hlr] - load_library_handle")
{
    const std::string library_path{test_library_path};
    void* handle = hlr::load_library_handle(library_path);

    REQUIRE(handle != nullptr);

    hlr::unload_library_handle(handle);
}

typedef int (*SumFn)(int, int);

TEST_CASE("[hlr] - load_raw_function_ptr")
{
    const std::string library_path{test_library_path};
    void* handle = hlr::load_library_handle(library_path);

    REQUIRE(handle != nullptr);

    void* sumFn = hlr::load_raw_function_ptr(handle, "add");
    REQUIRE(sumFn != nullptr);

    SumFn fn = reinterpret_cast<SumFn>(sumFn);
    int r    = fn(1, 2);
    REQUIRE(r == 3);

    hlr::unload_library_handle(handle);
}

TEST_CASE("[hlr] - load_function_ptr")
{
    const std::string library_path{test_library_path};
    void* handle = hlr::load_library_handle(library_path);

    REQUIRE(handle != nullptr);

    auto fn = hlr::load_function_ptr<SumFn>(handle, "add");

    int r = fn(1, 2);
    REQUIRE(r == 3);

    hlr::unload_library_handle(handle);
}

TEST_CASE("[hlr] - load_function")
{
    const std::string library_path{test_library_path};
    void* handle = hlr::load_library_handle(library_path);

    REQUIRE(handle != nullptr);

    auto fn = hlr::load_function<SumFn, int(int, int)>(handle, "add");

    int r = fn(1, 2);
    REQUIRE(r == 3);

    hlr::unload_library_handle(handle);
}
