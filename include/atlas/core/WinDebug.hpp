#pragma once

#if defined(_WIN32) || defined(_WIN64)

#include "FMT.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include <cstring>

namespace atlas::core
{
    template<typename... Args>
    void winDebugPrint(std::string const& format, Args const&... args)
    {
        auto message = fmt::sprintf(format, std::forward(args)...);
        static constexpr auto maxLogLength = 16 * 1024;

        char buffer[maxLogLength];
        std::memcpy(buffer, message.c_str(), message.size() + 1);
        strncat_s(buffer, "\n", 3);

        WCHAR wszBuffer[maxLogLength] = {0};
        MultiByteToWideChar(CP_UTF8, 0, buffer, -1, wszBuffer,
                            sizeof(wszBuffer));
        OutputDebugStringW(wszBuffer);
    }
} // namespace atlas::core

#endif
