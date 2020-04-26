#include "Context.hpp"

#include <fmt/printf.h>
#include <map>

namespace atlas::glx
{
    static void
    mousePressCallback(GLFWwindow* window, int button, int action, int mods)
    {
        auto callbacks =
            static_cast<WindowCallbacks*>(glfwGetWindowUserPointer(window));
        if (callbacks == nullptr)
        {
            return;
        }

        if (callbacks->mouse_press_callback)
        {
            double x;
            double y;
            glfwGetCursorPos(window, &x, &y);
            callbacks->mouse_press_callback(button, action, mods, x, y);
        }
    }

    static void mouseMoveCallback(GLFWwindow* window, double xPos, double yPos)
    {
        auto callbacks =
            static_cast<WindowCallbacks*>(glfwGetWindowUserPointer(window));
        if (callbacks == nullptr)
        {
            return;
        }

        if (callbacks->mouse_move_callback)
        {
            callbacks->mouse_move_callback(xPos, yPos);
        }
    }

    static void
    mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
    {
        auto callbacks =
            static_cast<WindowCallbacks*>(glfwGetWindowUserPointer(window));
        if (callbacks == nullptr)
        {
            return;
        }

        if (callbacks->mouse_scroll_callback)
        {
            callbacks->mouse_scroll_callback(xOffset, yOffset);
        }
    }

    static void keyPressCallback(
        GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
            return;
        }

        auto callbacks =
            static_cast<WindowCallbacks*>(glfwGetWindowUserPointer(window));
        if (callbacks == nullptr)
        {
            return;
        }

        if (callbacks->key_press_callback)
        {
            callbacks->key_press_callback(key, scancode, action, mods);
        }
    }

    static void windowSizeCallback(GLFWwindow* window, int width, int height)
    {
        auto callbacks =
            static_cast<WindowCallbacks*>(glfwGetWindowUserPointer(window));
        if (callbacks == nullptr)
        {
            return;
        }

        if (callbacks->window_size_callback)
        {
            callbacks->window_size_callback(width, height);
        }
    }

    static void
    framebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        auto callbacks =
            static_cast<WindowCallbacks*>(glfwGetWindowUserPointer(window));
        if (callbacks == nullptr)
        {
            return;
        }

        if (callbacks->framebuffer_size_callback)
        {
            callbacks->framebuffer_size_callback(width, height);
        }
    }

    static void charCallback(GLFWwindow* window, unsigned int codepoint)
    {
        auto callbacks =
            static_cast<WindowCallbacks*>(glfwGetWindowUserPointer(window));
        if (callbacks == nullptr)
        {
            return;
        }

        if (callbacks->char_callback)
        {
            callbacks->char_callback(codepoint);
        }
    }

    static void windowCloseCallback(GLFWwindow* window)
    {
        auto callbacks =
            static_cast<WindowCallbacks*>(glfwGetWindowUserPointer(window));
        if (callbacks == nullptr)
        {
            return;
        }

        if (callbacks->window_close_callback)
        {
            callbacks->window_close_callback();
        }
    }

    bool initialize_glfw(GLFWerrorfun errorCallback)
    {
        glfwSetErrorCallback(errorCallback);
        if (glfwInit() == 0)
        {
            fmt::print(stderr, "error: Could not initialize GLFW.\n");
            return false;
        }

        return true;
    }

    GLFWwindow* create_glfw_window(WindowSettings const& settings)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, settings.version.major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, settings.version.minor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, settings.profile);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,
                       static_cast<int>(settings.enable_debug_context));
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
                       static_cast<int>(settings.is_forward_compatible));

        glfwWindowHint(GLFW_MAXIMIZED, static_cast<int>(settings.is_maximized));
        glfwWindowHint(GLFW_RESIZABLE,
                       static_cast<int>(settings.is_resizeable));

        GLFWmonitor* monitor =
            (settings.is_fullscreen) ? glfwGetPrimaryMonitor() : nullptr;
        GLFWwindow* window = glfwCreateWindow(settings.size.width,
                                              settings.size.height,
                                              settings.title.c_str(),
                                              monitor,
                                              nullptr);
        return window;
    }

    bool create_gl_context(GLFWwindow* window, ContextVersion const& version)
    {
        if (window == nullptr)
        {
            return false;
        }

        if (gl3wInit() != 0)
        {
            fmt::print(stderr, "error: Could not initialize OpenGL.\n");
            return false;
        }

        if (gl3wIsSupported(version.major, version.minor) == 0)
        {
            fmt::print(stderr,
                       "error: OpenGL {}.{} is not supported.\n",
                       version.major,
                       version.minor);
            return false;
        }

        GLint major;
        GLint minor;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);
        fmt::print(stdout, "Created OpenGL context {}.{}\n", major, minor);
        return true;
    }

    void bind_window_callbacks(GLFWwindow* window,
                               WindowCallbacks const& callbacks)
    {
        if (window == nullptr)
        {
            return;
        }

        auto calls = new WindowCallbacks(callbacks);
        glfwSetWindowUserPointer(window, calls);

        glfwSetKeyCallback(window, keyPressCallback);
        glfwSetMouseButtonCallback(window, mousePressCallback);
        glfwSetScrollCallback(window, mouseScrollCallback);
        glfwSetCursorPosCallback(window, mouseMoveCallback);

        glfwSetWindowSizeCallback(window, windowSizeCallback);
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
        glfwSetWindowCloseCallback(window, windowCloseCallback);
        glfwSetCharCallback(window, charCallback);
    }

    void destroy_glfw_window(GLFWwindow* window)
    {
        if (window == nullptr)
        {
            return;
        }

        auto callbacks =
            static_cast<WindowCallbacks*>(glfwGetWindowUserPointer(window));
        delete callbacks;
        glfwSetWindowUserPointer(window, nullptr);

        glfwDestroyWindow(window);
    }

    void terminate_glfw()
    {
        glfwTerminate();
    }
} // namespace atlas::glx
