#include "atlas/utils/WindowSettings.hpp"

#include "atlas/core/Platform.hpp"
#include "atlas/core/Macros.hpp"

namespace atlas
{
    namespace utils
    {
        WindowSettings::WindowSettings() :
            mTitle("Made with Atlas Framework"),
            mWidth(1080),
            mHeight(720),
            mMajor(3),
            mMinor(3),
            mIsFullscreen(false),
            mIsMaximized(false)
        {
#ifdef ATLAS_DEBUG
            mIsDebugContext = true;
#else
            mIsDebugContext = false;
#endif

#ifdef ATLAS_PLATFORM_APPLE
            mIsForwardCompat = true;
#else
            mIsForwardCompat = false;
#endif
        }

        WindowSettings::WindowSettings(std::string const& title, int width,
            int height, int major, int minor) :
            mTitle(title),
            mWidth(width),
            mHeight(height),
            mMajor(major),
            mMinor(minor),
            mIsFullscreen(false),
            mIsMaximized(false)
        {
#ifdef ATLAS_DEBUG
            mIsDebugContext = true;
#else
            mIsDebugContext = false;
#endif

#ifdef ATLAS_PLATFORM_APPLE
            mIsForwardCompat = true;
#else
            mIsForwardCompat = false;
#endif
        }

        WindowSettings::~WindowSettings()
        { }

        void WindowSettings::setTitle(std::string const& title)
        {
            mTitle = title;
        }

        void WindowSettings::setWindowSize(int width, int height)
        {
            mWidth = width;
            mHeight = height;
        }

        void WindowSettings::setContextVersion(int major, int minor)
        {
            mMajor = major;
            mMinor = minor;
        }

        void WindowSettings::setFullscreen(bool fullscreen)
        {
            mIsFullscreen = fullscreen;
            mIsMaximized = (fullscreen) ? false : mIsMaximized;
        }

        void WindowSettings::setMaximized(bool maximized)
        {
            mIsMaximized = maximized;
            mIsFullscreen = (maximized) ? false : mIsFullscreen;
        }

        void WindowSettings::setForwardCompat(bool compat)
        {
            mIsForwardCompat = compat;
        }

        void WindowSettings::setDebugContext(bool debug)
        {
            mIsDebugContext = debug;
        }

        std::string WindowSettings::getTitle() const
        {
            return mTitle;
        }

        std::tuple<int, int> WindowSettings::getWindowSize() const
        {
            return std::tuple<int, int>(mWidth, mHeight);
        }

        std::tuple<int, int> WindowSettings::getContextVersion() const
        {
            return std::tuple<int, int>(mMajor, mMinor);
        }

        int WindowSettings::isFullscreen() const
        {
            return (mIsFullscreen) ? 1 : 0;
        }

        int WindowSettings::isMaximized() const
        {
            return (mIsMaximized) ? 1 : 0;
        }

        int WindowSettings::forwardCompatEnabled() const
        {
            return (mIsForwardCompat) ? 1 : 0;
        }

        int WindowSettings::debugContextEnabled() const
        {
            return (mIsDebugContext) ? 1 : 0;
        }
    }
}