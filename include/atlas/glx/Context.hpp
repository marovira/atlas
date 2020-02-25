#pragma once

#include <atlas/core/Platform.hpp>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <functional>
#include <string>

namespace atlas::glx
{
    struct WindowCallbacks
    {
        std::function<void(int, int, int, double, double)> mousePressCallback{};
        std::function<void(double, double)> mouseMoveCallback{};
        std::function<void(double, double)> mouseScrollCallback{};
        std::function<void(int, int, int, int)> keyPressCallback{};
        std::function<void(int, int)> windowSizeCallback{};
        std::function<void(int, int)> framebufferSizeCallback{};
        std::function<void(unsigned int)> charCallback{};
        std::function<void()> windowCloseCallback{};
    };

    struct ContextVersion
    {
        int major{4};
        int minor{5};
    };

    struct WindowSize
    {
        int width{300};
        int height{300};
    };

    struct WindowSettings
    {
        ContextVersion version{};
        WindowSize size{};
        std::string title;
        int profile{GLFW_OPENGL_CORE_PROFILE};
        bool enableDebugContext{core::CurrentBuild == core::BuildType::Debug};
        bool isForwardCompat{true};
        bool isMaximized{false};
        bool isResizeable{true};
        bool isFullscreen{false};
    };

    bool initializeGLFW(GLFWerrorfun errorCallback);
    GLFWwindow* createGLFWWindow(WindowSettings const& settings);
    bool createGLContext(GLFWwindow* window, ContextVersion const& version);
    void bindWindowCallbacks(GLFWwindow* window,
                             WindowCallbacks const& callbacks);
    void destroyGLFWWindow(GLFWwindow* window);

    void terminateGLFW();
} // namespace atlas::glx
