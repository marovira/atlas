#include "atlas/utils/Scene.hpp"
#include "atlas/math/Math.hpp"
#include "atlas/core/Macros.hpp"
#include "atlas/gl/GL.hpp"

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

        void Scene::keyPressEvent(int key, int scancode, int action, int mods)
        {
            UNUSED(key);
            UNUSED(scancode);
            UNUSED(action);
            UNUSED(mods);
        }

        void Scene::screenResizeEvent(int width, int height)
        {
            USING_GLM_NS;

            glViewport(0, 0, width, height);
            mProjection = perspective(radians(45.0),
                (double)width / height, 1.0, 1000.0);
        }

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
    }
}