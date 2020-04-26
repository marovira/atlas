#pragma once

namespace atlas::core
{
    enum class Platform
    {
        Windows = 0,
        Linux
    };

    enum class BuildType
    {
        Debug = 0,
        Release
    };

#if defined(_WIN32) || defined(_WIN64)
#    define ATLAS_PLATFORM_WINDOWS
    static constexpr Platform CurrentPlatform{Platform::Windows};
#elif defined(__APPLE__)
#    error "Support for Apple is deprecated"
#elif defined(__linux__)
#    define ATLAS_PLATFORM_LINUX
    static constexpr Platform CurrentPlatform{Platform::Linux};
#endif

#if defined(_DEBUG) || defined(DEBUG) || defined(__DEBUG__)
#    define ATLAS_BUILD_DEBUG
    static constexpr BuildType CurrentBuild{BuildType::Debug};
#else
#    define ATLAS_BUILD_RELEASE
    static constexpr BuildType CurrentBuild{BuildType::Release};
#endif
} // namespace atlas::core
