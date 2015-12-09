#include "atlas/Application.hpp"
#include "atlas/Log.hpp"
#include "atlas/Platform.hpp"
#include "atlas/GLHeader.hpp"

#include <GLFW/glfw3.h>
#include <vector>

namespace atlas
{
    typedef std::unique_ptr<Scene> ScenePointer;

    struct Application::ApplicationImpl
    {
        ApplicationImpl()
        { }

        ~ApplicationImpl()
        { }

        GLFWwindow* currentWindow;
        std::vector<ScenePointer> sceneList;
        int currentScene;
    };

    static void mousePressCallback(GLFWwindow* window, int button,
        int action, int mods)
    {
        UNUSED(window);
        APP.getCurrentScene()->mousePressEvent(button, action, mods);
    }

    static void mouseMoveCallback(GLFWwindow* window, double xPos,
        double yPos)
    {
        UNUSED(window);
        APP.getCurrentScene()->mouseMoveEvent(xPos, yPos);
    }

    static void keyPressCallback(GLFWwindow* window, int key,
        int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        else
        {
            APP.getCurrentScene()->keyPressEvent(key, scancode, action, mods);
        }
    }

    static void windowSizeCallback(GLFWwindow* window, int width, int height)
    {
        UNUSED(window);

        width = (width < 1) ? 1 : width;
        height = (height < 1) ? 1 : height;

        APP.getCurrentScene()->screenResizeEvent(width, height);
    }
    static void errorCallback(int error, const char* message)
    {
        ERROR_LOG(std::to_string(error) + ":" + message);
    }

    Application::Application() :
        mImpl(new ApplicationImpl)
    {
        glfwSetErrorCallback(errorCallback);

        if (!glfwInit())
        {
            CRITICAL_LOG("Could not initialize GLFW");
            exit(EXIT_FAILURE);
        }
    }

    Application::~Application()
    {
        if (mImpl->currentWindow)
        {
            glfwDestroyWindow(mImpl->currentWindow);
        }

        glfwTerminate();
    }

    Application& Application::getInstance()
    {
        static Application instance;
        return instance;
    }

    void Application::createWindow(int width, int height,
        std::string const& title)
    {
        if (mImpl->currentWindow != nullptr)
        {
            ERROR_LOG("Multiple windows are currently not supported.");
            return;
        }

#if defined(ATLAS_PLATFORM_APPLE)
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
#endif

        mImpl->currentWindow = glfwCreateWindow(width, height, title.c_str(),
            NULL, NULL);
        if (!mImpl->currentWindow)
        {
            glfwTerminate();
            CRITICAL_LOG("Could not create window.");
            exit(EXIT_FAILURE);
        }

        glfwMakeContextCurrent(mImpl->currentWindow);

#if !defined(ATLAS_PLATFORM_APPLE)
        if (glewInit() != GLEW_OK)
        {
            CRITICAL_LOG("Could not initialize GLEW.");
            glfwDestroyWindow(mImpl->currentWindow);
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
#endif
        glfwSetKeyCallback(mImpl->currentWindow, keyPressCallback);
        glfwSetWindowSizeCallback(mImpl->currentWindow, windowSizeCallback);
        glfwSetMouseButtonCallback(mImpl->currentWindow, mousePressCallback);
        glfwSetCursorPosCallback(mImpl->currentWindow, mouseMoveCallback);
    }

    void Application::runApplication()
    {
        if (mImpl->currentWindow == nullptr)
        {
            WARN_LOG("Cannot run application without a window.");
            return;
        }

        if (mImpl->sceneList.empty())
        {
            WARN_LOG("Cannot run application without a scene.");
            return;
        }

        int width, height;
        int currentScene = mImpl->currentScene;
        glfwGetWindowSize(mImpl->currentWindow, &width, &height);
        mImpl->sceneList[currentScene]->screenResizeEvent(width, height);

        glfwSetTime(0.0);
        double currentTime;

        while (!glfwWindowShouldClose(mImpl->currentWindow))
        {
            currentTime = glfwGetTime();
            mImpl->sceneList[currentScene]->updateScene(currentTime);
            mImpl->sceneList[currentScene]->renderScene();

            glfwSwapBuffers(mImpl->currentWindow);
            glfwPollEvents();
        }
    }

    void Application::addScene(Scene* scene)
    {
        mImpl->sceneList.push_back(ScenePointer(scene));
        scene = nullptr;
        mImpl->currentScene = mImpl->sceneList.size() - 1;
    }

    Scene* Application::getCurrentScene() const
    {
        return mImpl->sceneList[mImpl->currentScene].get();
    }
}