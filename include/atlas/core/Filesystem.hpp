#pragma once

#include "Platform.hpp"

#include <experimental/filesystem>
#include <string>

namespace atlas::core
{
    std::time_t getFileLastWrite(std::string const& filename)
    {
        namespace fs = std::experimental::filesystem;

        fs::path filePath{filename};
        auto ftime = fs::last_write_time(filePath);
        return decltype(ftime)::clock::to_time_t(ftime);
    }
} // namespace atlas::core
