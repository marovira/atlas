#ifndef ATLAS_INCLUDE_ATLAS_UTILS_CAMERA_HPP
#define ATLAS_INCLUDE_ATLAS_UTILS_CAMERA_HPP

#pragma once

#include "atlas/math/Matrix.hpp"
#include "atlas/math/Vector.hpp"
#include "atlas/math/Ray.hpp"

namespace atlas
{
    namespace utils
    {
        class Camera
        {
        public:
            Camera();
            Camera(Camera const& camera) = default;
            virtual ~Camera();

            enum class CameraMovements
            {
                IDLE = 0,
                TUMBLE,
                TRACK,
                DOLLY
            };

            virtual void mouseDown(math::Point2 const& point, 
                CameraMovements movement);
            virtual void mouseDrag(math::Point2 const& point);
            virtual void mouseUp();

            virtual void resetCamera();

            virtual math::Matrix4 getCameraMatrix();
        };
    }
}

#endif