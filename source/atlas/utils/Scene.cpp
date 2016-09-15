#include "atlas/utils/Scene.hpp"
#include "atlas/math/Math.hpp"
#include "atlas/core/Macros.hpp"
#include "atlas/gl/GL.hpp"
#include "atlas/utils/Application.hpp"

namespace atlas
{
    namespace utils
    {
        Scene::Scene()
        { }

        Scene::~Scene()
        { }

        void Scene::mousePressEvent(int button, int action, int modifiers, 
            double xPos, double yPos)
        {
            UNUSED(button);
            UNUSED(action);
            UNUSED(modifiers);
            UNUSED(xPos);
            UNUSED(yPos);
        }

        void Scene::mouseMoveEvent(double xPos, double yPos)
        {
            UNUSED(xPos);
            UNUSED(yPos);
        }

        void Scene::mouseScrollEvent(double xOffset, double yOffset)
        {
            UNUSED(xOffset);
            UNUSED(yOffset);
        }

        void Scene::keyPressEvent(int key, int scancode, int action, int mods)
        {
            UNUSED(key);
            UNUSED(scancode);
            UNUSED(action);
            UNUSED(mods);
        }

        void Scene::screenResizeEvent(int width, int height)
        {
            glViewport(0, 0, width, height);
            mProjection = glm::perspective(glm::radians(45.0),
                (double)width / height, 1.0, 1000.0);
        }

        void Scene::onSceneEnter()
        { }

        void Scene::onSceneExit()
        { }

        void Scene::updateScene(double time)
        {
            UNUSED(time);
        }

        void Scene::renderScene()
        {
            float grey = 161.0f / 255.0f;
            glClearColor(grey, grey, grey, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
        }

        bool Scene::sceneEnded()
        {
            return true;
        }

        void Scene::setCursorEnabled(bool enabled)
        {
            int en = (enabled) ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
            glfwSetInputMode(Application::getInstance().getCurrentWindow(),
                GLFW_CURSOR, en);
        }
    }
}