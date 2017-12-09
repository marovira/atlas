#include "atlas/utils/Application.hpp"
#include "atlas/utils/Scene.hpp"
#include "atlas/utils/WindowSettings.hpp"
#include "atlas/core/Log.hpp"
#include "atlas/core/Platform.hpp"
#include "atlas/core/Float.hpp"
#include "atlas/gl/ErrorCheck.hpp"

#include <vector>

namespace atlas
{
    namespace utils
    {
        struct Application::ApplicationImpl
        {
            ApplicationImpl() :
                currentWindow(nullptr)
            { }

            ~ApplicationImpl()
            { }

            void insertScene(ScenePointer scene)
            {
                sceneList.push_back(std::move(scene));
                currentScene = sceneList.size() - 1;
                sceneTicks.push_back(-1.0);
            }

            double prepareSwappedScene(size_t sceneIndex)
            {
                sceneTicks[currentScene] = glfwGetTime();
                currentScene = sceneIndex;

                int width, height;
                glfwGetFramebufferSize(currentWindow, &width, &height);
                sceneList[sceneIndex]->screenResizeEvent(width, height);

                return
                    (core::areEqual<double>(-1.0, sceneTicks[currentScene])) ?
                    0 : sceneTicks[currentScene];
            }

            void getNextScene()
            {
                if (sceneList.size() == 1)
                {
                    return;
                }

                double newTime = 
                    prepareSwappedScene((currentScene + 1) % sceneList.size());
                glfwSetTime(newTime);
            }

            void getPreviousScene()
            {
                if (sceneList.size() == 1)
                {
                    return;
                }

                double newTime =
                    prepareSwappedScene((currentScene - 1) % sceneList.size());
                glfwSetTime(newTime);

                glfwSetTime(newTime);
            }

            GLFWwindow* currentWindow;
            std::vector<ScenePointer> sceneList;
            std::vector<double> sceneTicks;
            size_t currentScene;
        };

        static void mousePressCallback(GLFWwindow* window, int button,
            int action, int mods)
        {
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            Application::getInstance().getCurrentScene()->mousePressEvent(
                button, action, mods, x, y);
        }

        static void mouseMoveCallback(GLFWwindow* window, double xPos,
            double yPos)
        {
            UNUSED(window);
            Application::getInstance().getCurrentScene()->mouseMoveEvent(
                xPos, yPos);
        }

        static void mouseScrollCallback(GLFWwindow* window, double xOffset,
            double yOffset)
        {
            UNUSED(window);
            Application::getInstance().getCurrentScene()->mouseScrollEvent(
                xOffset, yOffset);
        }

        static void keyPressCallback(GLFWwindow* window, int key,
            int scancode, int action, int mods)
        {
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(window, GL_TRUE);
            }
            else if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
            {
                Application::getInstance().nextScene();
            }
            else if (key == GLFW_KEY_TAB && action == GLFW_PRESS &&
                mods == GLFW_KEY_LEFT_SHIFT)
            {
                Application::getInstance().previousScene();
            }
            else
            {
                Application::getInstance().getCurrentScene()->keyPressEvent(
                    key, scancode, action, mods);
            }
        }

        static void windowSizeCallback(GLFWwindow* window, int width, 
            int height)
        {
            UNUSED(window);
            UNUSED(width);
            UNUSED(height);
        }

        static void frameBufferSizeCallback(GLFWwindow* window, int width,
            int height)
        {
            UNUSED(window);

            width = (width < 1) ? 1 : width;
            height = (height < 1) ? 1 : height;

            Application::getInstance().getCurrentScene()->screenResizeEvent(width, height);
        }

        static void errorCallback(int error, const char* message)
        {
            ERROR_LOG(std::to_string(error) + ":" + message);
        }

        static void windowCloseCallback(GLFWwindow* window)
        {
            if (Application::getInstance().getCurrentScene()->sceneEnded())
            {
                glfwSetWindowShouldClose(window, GL_TRUE);
            }
            else
            {
                glfwSetWindowShouldClose(window, GL_FALSE);
                glfwHideWindow(window);
            }
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

        void Application::createWindow(WindowSettings const& settings)
        {
            if (mImpl->currentWindow != nullptr)
            {
                ERROR_LOG("Multiple windows are not suppored.");
                return;
            }

            auto context = settings.contextVersion;

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, std::get<0>(context));
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, std::get<1>(context));
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 
                settings.isDebugContext);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
                settings.isForwardCompat);

            glfwWindowHint(GLFW_MAXIMIZED, settings.isMaximized);
            glfwWindowHint(GLFW_RESIZABLE, settings.isResizeable);

            GLFWmonitor* monitor = (settings.isFullscreen) ?
                glfwGetPrimaryMonitor() : NULL;

            auto size = settings.windowSize;
            mImpl->currentWindow = glfwCreateWindow(std::get<0>(size),
                std::get<1>(size), settings.title.c_str(), monitor, NULL);

            if (!mImpl->currentWindow)
            {
                glfwTerminate();
                CRITICAL_LOG("Could not create window.");
                exit(EXIT_FAILURE);
            }

            glfwMakeContextCurrent(mImpl->currentWindow);

            if (gl3wInit())
            {
                CRITICAL_LOG("Could not initialize OpenGL.");
                glfwDestroyWindow(mImpl->currentWindow);
                glfwTerminate();
                exit(EXIT_FAILURE);
            }

            if (!gl3wIsSupported(std::get<0>(context), std::get<1>(context)))
            {
                CRITICAL_LOG_V("OpenGL %d.%d is not supported.",
                    std::get<0>(context), std::get<1>(context));
                glfwDestroyWindow(mImpl->currentWindow);
                glfwTerminate();
                exit(EXIT_FAILURE);
            }

            GLint major, minor;
            glGetIntegerv(GL_MAJOR_VERSION, &major);
            glGetIntegerv(GL_MINOR_VERSION, &minor);

            INFO_LOG("Created OpenGL context " + std::to_string(major) +
                "." + std::to_string(minor));

            glfwSetKeyCallback(mImpl->currentWindow, 
                keyPressCallback);
            glfwSetMouseButtonCallback(mImpl->currentWindow, 
                mousePressCallback);
            glfwSetScrollCallback(mImpl->currentWindow,
                mouseScrollCallback);
            glfwSetCursorPosCallback(mImpl->currentWindow, 
                mouseMoveCallback);

            glfwSetWindowSizeCallback(mImpl->currentWindow, 
                windowSizeCallback);
            glfwSetFramebufferSizeCallback(mImpl->currentWindow,
                frameBufferSizeCallback);
            glfwSetWindowCloseCallback(mImpl->currentWindow, 
                windowCloseCallback);

            gl::initializeGLError();

        }

        void Application::createWindow(int width, int height,
            std::string const& title, int contextVersionMajor, 
            int contextVersionMinor)
        {
            createWindow(WindowSettings(title, width, height, 
                contextVersionMajor, contextVersionMinor));
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

            glfwShowWindow(mImpl->currentWindow);

            int width, height;
            glfwGetFramebufferSize(mImpl->currentWindow, &width, &height);
            mImpl->sceneList[mImpl->currentScene]->screenResizeEvent(
                width, height);

            glfwSetTime(0.0);
            double currentTime;

            mImpl->sceneList[mImpl->currentScene]->onSceneEnter();

            while (!glfwWindowShouldClose(mImpl->currentWindow))
            {
                currentTime = glfwGetTime();
                mImpl->sceneList[mImpl->currentScene]->updateScene(currentTime);
                mImpl->sceneList[mImpl->currentScene]->renderScene();

                glfwSwapBuffers(mImpl->currentWindow);
                glfwPollEvents();
            }

            mImpl->sceneList[mImpl->currentScene]->onSceneExit();
        }

        void Application::getCursorPosition(double* x, double *y)
        {
            glfwGetCursorPos(mImpl->currentWindow, x, y);
        }

        void Application::addScene(ScenePointer scene)
        {
            mImpl->insertScene(std::move(scene));
        }

        void Application::nextScene()
        {
            mImpl->sceneList[mImpl->currentScene]->onSceneExit();
            mImpl->getNextScene();
            mImpl->sceneList[mImpl->currentScene]->onSceneEnter();
        }

        void Application::previousScene()
        {
            mImpl->sceneList[mImpl->currentScene]->onSceneExit();
            mImpl->getPreviousScene();
            mImpl->sceneList[mImpl->currentScene]->onSceneEnter();
        }

        Scene* Application::getCurrentScene() const
        {
            return mImpl->sceneList[mImpl->currentScene].get();
        }

        GLFWwindow* Application::getCurrentWindow() const
        {
            return mImpl->currentWindow;
        }
    }
}