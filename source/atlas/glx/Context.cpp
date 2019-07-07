#include "atlas/glx/Context.hpp"

#include <atlas/core/FMT.hpp>
#include <map>

namespace atlas::glx
{
    static std::unordered_map<GLFWwindow*, WindowCallbacks> glfwWindowMap;

    static void mousePressCallback(GLFWwindow* window, int button, int action,
                                   int mods)
    {
        glfwWindowMap.at(window).mousePressCallback(button, action, mods);
    }

    static void mouseMoveCallback(GLFWwindow* window, double xPos, double yPos)
    {
        glfwWindowMap.at(window).mouseMoveCallback(xPos, yPos);
    }

    static void mouseScrollCallback(GLFWwindow* window, double xOffset,
                                    double yOffset)
    {
        glfwWindowMap.at(window).mouseScrollCallback(xOffset, yOffset);
    }

    static void keyPressCallback(GLFWwindow* window, int key, int scancode,
                                 int action, int mods)
    {
        glfwWindowMap.at(window).keyPressCallback(key, scancode, action, mods);
    }

    static void windowSizeCallback(GLFWwindow* window, int width, int height)
    {
        glfwWindowMap.at(window).windowSizeCallback(width, height);
    }

    static void framebufferSizeCallback(GLFWwindow* window, int width,
                                        int height)
    {
        glfwWindowMap.at(window).framebufferSizeCallback(width, height);
    }

    static void windowCloseCallback(GLFWwindow* window)
    {
        glfwWindowMap.at(window).windowCloseCallback();
    }

    bool initializeGLFW(GLFWerrorfun errorCallback)
    {
        glfwSetErrorCallback(errorCallback);
        if (glfwInit() == 0)
        {
            fmt::print(stderr, "error: Could not initialize GLFW.\n");
            return false;
        }

        return true;
    }

    GLFWwindow* createGLFWWindow(WindowSettings const& settings)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, settings.version.major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, settings.version.minor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, settings.profile);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,
                       static_cast<int>(settings.enableDebugContext));
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
                       static_cast<int>(settings.isForwardCompat));

        glfwWindowHint(GLFW_MAXIMIZED, static_cast<int>(settings.isMaximized));
        glfwWindowHint(GLFW_RESIZABLE, static_cast<int>(settings.isResizeable));

        GLFWmonitor* monitor =
            (settings.isFullscreen) ? glfwGetPrimaryMonitor() : nullptr;
        GLFWwindow* window =
            glfwCreateWindow(settings.size.width, settings.size.height,
                             settings.title.c_str(), monitor, nullptr);
        return window;
    }

    bool createGLContext(GLFWwindow* window, ContextVersion const& version)
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
            fmt::print(stderr, "error: OpenGL {}.{} is not supported.\n",
                       version.major, version.minor);
            return false;
        }

        GLint major, minor;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);
        fmt::print(stdout, "Created OpenGL context {}.{}\n", major, minor);
        return true;
    }

    void bindWindowCallbacks(GLFWwindow* window,
                             WindowCallbacks const& callbacks)
    {
        glfwWindowMap.insert({window, callbacks});
        glfwSetKeyCallback(window, keyPressCallback);
        glfwSetMouseButtonCallback(window, mousePressCallback);
        glfwSetScrollCallback(window, mouseScrollCallback);
        glfwSetCursorPosCallback(window, mouseMoveCallback);

        glfwSetWindowSizeCallback(window, windowSizeCallback);
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
        glfwSetWindowCloseCallback(window, windowCloseCallback);
    }
} // namespace atlas::glx
