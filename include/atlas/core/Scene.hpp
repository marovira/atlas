#ifndef ATLAS_INCLUDE_ATLAS_CORE_SCENE_HPP
#define ATLAS_INCLUDE_ATLAS_CORE_SCENE_HPP

#pragma once

#include "Core.hpp"
#include "atlas/math/Matrix.hpp"
#include "atlas/utils/Time.hpp"

namespace atlas
{
    namespace core
    {
        class Scene
        {
        public:
            Scene();
            ~Scene();

            virtual void mousePressEvent(int button, int action, int modifiers);
            virtual void mouseMoveEvent(double xPos, double yPos);
            virtual void keyPressEvent(int key, int scancode, int action,
                int mods);
            virtual void screenResizeEvent(int width, int height);
            virtual void updateScene(double time);
            virtual void renderScene();
            virtual bool sceneEnded();

        protected:
            math::Matrix4 mProjection;
            math::Matrix4 mView;
            utils::Time mTime;
        };
    }
}

#endif