#pragma once

#include "Platform.hpp"

#include <experimental/filesystem>
#include <string>

namespace atlas::core
{
    std::time_t getFileLastWrite(std::string const& filename)
    {
#if defined(ATLAS_PLATFORM_WINDOWS)
        namespace fs = std::experimental::filesystem;
#else
        namespace fs = std::filesystem;
#endif

        fs::path filePath{filename};
        auto ftime = fs::last_write_time(filePath);
        return decltype(ftime)::clock::to_time_t(ftime);
    }
} // namespace atlas::core