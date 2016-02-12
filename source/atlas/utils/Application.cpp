#include "atlas/utils/Application.hpp"
#include "atlas/utils/Scene.hpp"
#include "atlas/core/Log.hpp"
#include "atlas/core/Platform.hpp"
#include "atlas/gl/GL.hpp"
#include "atlas/core/GLFW.hpp"
#include "atlas/core/Float.hpp"
#include "atlas/gl/ErrorCheck.hpp"

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
            APPLICATION.getCurrentScene()->mousePressEvent(button, action,
                mods, x, y);
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
            UNUSED(width);
            UNUSED(height);
        }

        static void frameBufferSizeCallback(GLFWwindow* window, int width,
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

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef ATLAS_PLATFORM_APPLE
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
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

            GLint major, minor;
            glGetIntegerv(GL_MAJOR_VERSION, &major);
            glGetIntegerv(GL_MINOR_VERSION, &minor);

            INFO_LOG("Created OpenGL context " + std::to_string(major) +
                "." + std::to_string(minor));

            glewExperimental = GL_TRUE;
            GLenum err = glewInit();
            if (err != GLEW_OK)
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
            glfwSetFramebufferSizeCallback(mImpl->currentWindow,
                frameBufferSizeCallback);
            glfwSetMouseButtonCallback(mImpl->currentWindow, 
                mousePressCallback);
            glfwSetCursorPosCallback(mImpl->currentWindow, 
                mouseMoveCallback);
            glfwSetWindowCloseCallback(mImpl->currentWindow, 
                windowCloseCallback);

            // HACK: This clears out the errors generated by glew.
            // We need to find a better way of doing this. Maybe replace
            // GLEW in its entirety?
            GL_CLEAR_ERRORS();
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

            while (!glfwWindowShouldClose(mImpl->currentWindow))
            {
                currentTime = glfwGetTime();
                mImpl->sceneList[mImpl->currentScene]->updateScene(currentTime);
                mImpl->sceneList[mImpl->currentScene]->renderScene();

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