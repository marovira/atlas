/**
 *	\file ModellingScene.hpp
 *	\brief Defines a simple scene for modeling projects.
 */

#ifndef ATLAS_INCLUDE_ATLAS_TOOLS_MODELLING_SCENE_HPP
#define ATLAS_INCLUDE_ATLAS_TOOLS_MODELLING_SCENE_HPP

#pragma once

#include "Tools.hpp"
#include "MayaCamera.hpp"
#include "Grid.hpp"
#include "atlas/utils/Scene.hpp"
#include "atlas/gl/Buffer.hpp"

namespace atlas
{
    namespace tools
    {
        class ModellingScene : public utils::Scene
        {
        public:
            ModellingScene();

            void mousePressEvent(int button, int action, int modifiers,
                double xPos, double yPos) override;
            void mouseMoveEvent(double xPos, double yPos) override;
            void mouseScrollEvent(double xOffset, double yOffset) override;
            void keyPressEvent(int key, int scancode, int action,
                int mods) override;
            void screenResizeEvent(int width, int height) override;

            void onSceneEnter() override;
            void onSceneExit() override;

            void updateScene(double time) override;
            void renderScene() override;

        protected:
            bool mShowGrid;
            double mLastTime;
            float mWidth, mHeight;

            atlas::gl::Buffer mUniformMatrixBuffer;

             Grid mGrid;
             MayaCamera mCamera;

        };
    }
}


#endif
