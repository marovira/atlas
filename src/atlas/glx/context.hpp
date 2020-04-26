#pragma once

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <functional>
#include <string>
#include <zeus/platform.hpp>

namespace atlas::glx
{
    struct WindowCallbacks
    {
        std::function<void(int, int, int, double, double)>
            mouse_press_callback{};
        std::function<void(double, double)> mouse_move_callback{};
        std::function<void(double, double)> mouse_scroll_callback{};
        std::function<void(int, int, int, int)> key_press_callback{};
        std::function<void(int, int)> window_size_callback{};
        std::function<void(int, int)> framebuffer_size_callback{};
        std::function<void(unsigned int)> char_callback{};
        std::function<void()> window_close_callback{};
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
        bool enable_debug_context{zeus::current_build ==
                                  zeus::BuildType::debug};
        bool is_forward_compatible{true};
        bool is_maximized{false};
        bool is_resizeable{true};
        bool is_fullscreen{false};
    };

    bool initialize_glfw(GLFWerrorfun errorCallback);
    GLFWwindow* create_glfw_window(WindowSettings const& settings);
    bool create_gl_context(GLFWwindow* window, ContextVersion const& version);
    void bind_window_callbacks(GLFWwindow* window,
                               WindowCallbacks const& callbacks);
    void destroy_glfw_window(GLFWwindow* window);

    void terminate_glfw();
} // namespace atlas::glx
