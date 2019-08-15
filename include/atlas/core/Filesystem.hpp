#pragma once

#include "Platform.hpp"

#include <fmt/printf.h>

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
            fmt::print(stderr, "warning: ({}): {}\n", code.value(),
                       code.message());
        }
        return decltype(ftime)::clock::to_time_t(ftime);
    }
} // namespace atlas::core
