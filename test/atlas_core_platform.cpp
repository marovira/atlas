#include <atlas/core/Platform.hpp>

#include <catch2/catch.hpp>

#include <string>

template <atlas::core::Platform P>
constexpr std::string getPlatformName()
{
    if constexpr (P == atlas::core::Platform::Windows)
    {
        return "Windows";
    }
    else
    {
        return "Linux";
    }
}

template <atlas::core::BuildType B>
constexpr std::string getBuildType()
{
    if constexpr (B == atlas::core::BuildType::Debug)
    {
        return "Debug";
    }
    else
    {
        return "Release";
    }
}

#if defined(ATLAS_PLATFORM_WINDOWS)
TEST_CASE("Checking Windows Template Flags", "[core]")
{
    using atlas::core::CurrentPlatform;
    REQUIRE(getPlatformName<CurrentPlatform>() == "Windows");
}
#else
TEST_CASE("Checking Linux Template Flags", "[core]")
{
    using atlas::core::CurrentPlatform;
    REQUIRE(getPlatformName<CurrentPlatform>() == "Linux");
}
#endif

#if defined(ATLAS_BUILD_DEBUG)
TEST_CASE("Checking Debug Flag", "[core]")
{
    using atlas::core::CurrentBuild;
    REQUIRE(getBuildType<CurrentBuild>() == "Debug");
}
#else
TEST_CASE("Checking Debug Flag", "[core]")
{
    using atlas::core::CurrentBuild;
    REQUIRE(getBuildType<CurrentBuild>() == "Release");
}
#endif
