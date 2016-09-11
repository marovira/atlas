/**
 * \file WindowSettings.hpp
 * \brief Defines settings for windows.
 */

#ifndef ATLAS_INCLUDE_ATLAS_UTILS_WINDOW_SETTINGS_HPP
#define ATLAS_INCLUDE_ATLAS_UTILS_WINDOW_SETTINGS_HPP

#pragma once

#include "Utils.hpp"

#include <string>
#include <tuple>

namespace atlas
{
    namespace utils
    {
        class WindowSettings
        {
        public:
            WindowSettings();
            WindowSettings(std::string const& title, int width, int height,
                int major, int minor);
            ~WindowSettings();

            void setTitle(std::string const& title);
            void setWindowSize(int width, int height);
            void setContextVersion(int major, int minor);
            void setFullscreen(bool fullscreen);
            void setMaximized(bool maximized);
            void setForwardCompat(bool compat);
            void setDebugContext(bool debug);

            std::string getTitle() const;
            std::tuple<int, int> getWindowSize() const;
            std::tuple<int, int> getContextVersion() const;
            int isFullscreen() const;
            int isMaximized() const;
            int forwardCompatEnabled() const;
            int debugContextEnabled() const;

        private:
            std::string mTitle;
            int mWidth, mHeight;
            int mMajor, mMinor;
            bool mIsFullscreen, mIsMaximized;
            bool mIsForwardCompat;
            bool mIsDebugContext;
        };
    }
}

#endif