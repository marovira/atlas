#pragma once

#include "Platform.hpp"

#include <fmt/printf.h>

#if defined(ATLAS_PLATFORM_WINDOWS)
#    define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#endif
#include <experimental/filesystem>
#include <string>

namespace atlas::core
{
    inline std::time_t getFileLastWrite(std::string const& filename)
    {
        namespace fs = std::experimental::filesystem;

        fs::path filePath{filename};
        [[maybe_unused]] std::error_code code;
        auto ftime = fs::last_write_time(filePath, code);
        if (code && CurrentBuild == BuildType::Debug)
        {
            fmt::print(
                stderr, "warning: ({}): {}\n", code.value(), code.message());
        }
        return decltype(ftime)::clock::to_time_t(ftime);
    }

    inline std::string getFileDirectory(std::string const& filename)
    {
        namespace fs = std::experimental::filesystem;

        fs::path filePath{filename};
        std::string rootDir;
        if (filePath.has_parent_path())
        {
            rootDir = filePath.parent_path().string();
        }

        return rootDir;
    }
} // namespace atlas::core
