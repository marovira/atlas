#ifndef ATLAS_INCLUDE_ATLAS_GL_CAMERA_HPP
#define ATLAS_INCLUDE_ATLAS_GL_CAMERA_HPP

#pragma once

#include "Matrix.hpp"
#include "Vector.hpp"
#include "Ray.hpp"

namespace atlas
{
    class GLCamera
    {
    public:
        GLCamera();
        GLCamera(GLCamera const& camera) = default;
        virtual ~GLCamera();

        enum class CameraMovements
        {
            IDLE = 0,
            TUMBLE,
            TRACK,
            DOLLY
        };

        virtual void mouseDown(Point2 const& point, CameraMovements movement);
        virtual void mouseDrag(Point2 const& point);
        virtual void mouseUp();

        virtual void resetCamera();

        virtual Matrix4 getCameraMatrix();
    };

}

#endif