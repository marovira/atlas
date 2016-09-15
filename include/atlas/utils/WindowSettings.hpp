/**
 * \file WindowSettings.hpp
 * \brief Defines settings for windows.
 */

#ifndef ATLAS_INCLUDE_ATLAS_UTILS_WINDOW_SETTINGS_HPP
#define ATLAS_INCLUDE_ATLAS_UTILS_WINDOW_SETTINGS_HPP

#pragma once

#include "Utils.hpp"
#include "atlas/core/Platform.hpp"
#include "atlas/core/Macros.hpp"
#include "atlas/Atlas.hpp"

#include <string>
#include <tuple>

namespace atlas
{
    namespace utils
    {
        struct WindowSettings
        {
            WindowSettings() :
                title("Made with Atlas Framework" + 
                    std::string(ATLAS_VERSION_STRING)),
                windowSize(std::make_tuple(1080, 720)),
                contextVersion(std::make_tuple(3, 3)),
                isFullscreen(false),
                isMaximized(false)
            {
#ifdef ATLAS_DEBUG
                isDebugContext = true;
#else
                isDebugContext = false;
#endif

#ifdef ATLAS_PLATFORM_APPLE
                isForwardCompat = true;
#else
                isForwardCompat = false;
#endif
            }

            WindowSettings(std::string const& title, int width, int height,
                int major, int minor) :
                title(title),
                windowSize(std::make_tuple(width, height)),
                contextVersion(std::make_tuple(major, minor)),
                isFullscreen(false),
                isMaximized(false)
            {
#ifdef ATLAS_DEBUG
                isDebugContext = true;
#else
                isDebugContext = false;
#endif

#ifdef ATLAS_PLATFORM_APPLE
                isForwardCompat = true;
#else
                isForwardCompat = false;
#endif
            }

            std::string title;
            std::tuple<int, int> windowSize;
            std::tuple<int, int> contextVersion;
            bool isFullscreen;
            bool isMaximized;
            bool isForwardCompat;
            bool isDebugContext;
        };
    }
}

#endif