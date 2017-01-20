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
        /**
         * \class WindowSettings
         * \brief Defines the window settings.
         * 
         * This struct consolidates all of the most common window settings
         * for GLFW.
         */
        struct WindowSettings
        {
            /**
             * Empty constructor. 
             * 
             * \note
             * The flag for debug contexts is set automatically. Also, if
             * building in Apple systems, the forward compatibility flag is 
             * also automatically set.
             */
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
            
            /**
             * Constructor.
             * 
             * \param[in] title The window title.
             * \param[in] width The window width.
             * \param[in] height The window height.
             * \param[in] major The major version number.
             * \param[in] minor The minor version number.
             */
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

            /**
             * \var title
             * The window title.
             */
            std::string title;

            /**
             * \var windowSize
             * The dimensions of the window.
             */
            std::tuple<int, int> windowSize;

            /**
             * \var contextVersion
             * The OpenGL context version.
             */
            ContextVersion contextVersion;

            /**
             * \var isFullscreen
             * Whether the window is fullscren or not.
             */
            bool isFullscreen;

            /**
             * \var isMaximized
             * Whether the window is maximized or not.
             */
            bool isMaximized;

            /**
             * \var isForwardCompat
             * Whether the OpenGL context created is forward compatible or not.
             */
            bool isForwardCompat;

            /**
             * \var isDebugContext
             * Whether the OpenGL context creatd is a debug context.
             */
            bool isDebugContext;
        };
    }
}

#endif