#include "atlas/utils/Camera.hpp"
#include "atlas/core/Macros.hpp"

namespace atlas
{
    namespace utils
    {
        Camera::Camera()
        { }

        Camera::~Camera()
        { }

        void Camera::mouseDown(math::Point2 const& point, 
            CameraMovements movement)
        {
            UNUSED(point);
            UNUSED(movement);
        }

        void Camera::mouseDrag(math::Point2 const& point)
        {
            UNUSED(point);
        }

        void Camera::mouseUp()
        { }

        void Camera::resetCamera()
        { }

        math::Matrix4 Camera::getCameraMatrix()
        {
            return math::Matrix4();
        }

    }
}