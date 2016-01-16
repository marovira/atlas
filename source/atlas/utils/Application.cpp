#include "atlas/utils/Application.hpp"
#include "atlas/utils/Scene.hpp"
#include "atlas/core/Log.hpp"
#include "atlas/core/Platform.hpp"
#include "atlas/gl/GL.hpp"
#include "atlas/core/GLFW.hpp"
#include "atlas/core/Float.hpp"

#include <vector>

namespace atlas
{
    namespace utils
    {

        typedef std::unique_ptr<Scene> ScenePointer;

        struct Application::ApplicationImpl
        {
            ApplicationImpl() :
                currentWindow(nullptr)
            { }

            ~ApplicationImpl()
            { }

            void insertScene(Scene* scene)
            {
                sceneList.push_back(ScenePointer(scene));
                currentScene = sceneList.size() - 1;
                sceneTicks.push_back(-1.0);
            }

            void getNextScene()
            {
                if (sceneList.size() == 1)
                {
                    return;
                }

                sceneTicks[currentScene] = glfwGetTime();
                currentScene = (currentScene + 1) % sceneList.size();

                double newTime =
                    (core::areEqual<double>(-1.0, sceneTicks[currentScene])) ?
                    0 : sceneTicks[currentScene];
                glfwSetTime(newTime);
            }

            void getPreviousScene()
            {
                if (sceneList.size() == 1)
                {
                    return;
                }

                sceneTicks[currentScene] = glfwGetTime();
                currentScene = (currentScene - 1) % sceneList.size();

                double newTime =
                    (core::areEqual<double>(-1.0, sceneTicks[currentScene])) ?
                    0 : sceneTicks[currentScene];
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
            UNUSED(window);
            APPLICATION.getCurrentScene()->mousePressEvent(button, action, 
                mods);
        }

        static void mouseMoveCallback(GLFWwindow* window, double xPos,
            double yPos)
        {
            UNUSED(window);
            APPLICATION.getCurrentScene()->mouseMoveEvent(xPos, yPos);
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
                APPLICATION.nextScene();
            }
            else if (key == GLFW_KEY_TAB && action == GLFW_PRESS &&
                mods == GLFW_KEY_LEFT_SHIFT)
            {
                APPLICATION.previousScene();
            }
            else
            {
                APPLICATION.getCurrentScene()->keyPressEvent(key, scancode, 
                    action, mods);
            }
        }

        static void windowSizeCallback(GLFWwindow* window, int width, 
            int height)
        {
            UNUSED(window);

            width = (width < 1) ? 1 : width;
            height = (height < 1) ? 1 : height;

            APPLICATION.getCurrentScene()->screenResizeEvent(width, height);
        }
        static void errorCallback(int error, const char* message)
        {
            ERROR_LOG(std::to_string(error) + ":" + message);
        }

        static void windowCloseCallback(GLFWwindow* window)
        {
            if (APPLICATION.getCurrentScene()->sceneEnded())
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

            mImpl->currentWindow = glfwCreateWindow(width, height, 
                title.c_str(), NULL, NULL);
            if (!mImpl->currentWindow)
            {
                glfwTerminate();
                CRITICAL_LOG("Could not create window.");
                exit(EXIT_FAILURE);
            }

            glfwMakeContextCurrent(mImpl->currentWindow);

            if (glewInit() != GLEW_OK)
            {
                CRITICAL_LOG("Could not initialize GLEW.");
                glfwDestroyWindow(mImpl->currentWindow);
                glfwTerminate();
                exit(EXIT_FAILURE);
            }

            glfwSetKeyCallback(mImpl->currentWindow, 
                keyPressCallback);
            glfwSetWindowSizeCallback(mImpl->currentWindow, 
                windowSizeCallback);
            glfwSetMouseButtonCallback(mImpl->currentWindow, 
                mousePressCallback);
            glfwSetCursorPosCallback(mImpl->currentWindow, 
                mouseMoveCallback);
            glfwSetWindowCloseCallback(mImpl->currentWindow, 
                windowCloseCallback);
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
            size_t currentScene = mImpl->currentScene;
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

        void Application::getCursorPosition(double* x, double *y)
        {
            glfwGetCursorPos(mImpl->currentWindow, x, y);
        }

        void Application::addScene(Scene* scene)
        {
            mImpl->insertScene(scene);
        }

        void Application::nextScene()
        {
            mImpl->getNextScene();
        }

        void Application::previousScene()
        {
            mImpl->getPreviousScene();
        }

        Scene* Application::getCurrentScene() const
        {
            return mImpl->sceneList[mImpl->currentScene].get();
        }
    }
}