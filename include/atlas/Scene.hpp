#ifndef ATLAS_INCLUDE_ATLAS_SCENE_HPP
#define ATLAS_INCLUDE_ATLAS_SCENE_HPP

#pragma once

#include "Atlas.hpp"
#include "Matrix.hpp"

namespace atlas
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
        Matrix4 mProjection;
        Matrix4 mView;
    };
}

#endif