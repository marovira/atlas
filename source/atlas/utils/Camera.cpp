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

        void Camera::mouseMove(math::Point2 const& point)
        {
            UNUSED(point);
        }

        void Camera::mouseUp()
        { }

        void Camera::mouseScroll(atlas::math::Point2 const& offset)
        {
            UNUSED(offset);
        }

        void Camera::keyDown(int key)
        {
            UNUSED(key);
        }

        void Camera::keyUp(int key)
        { 
            UNUSED(key);
        }

        void Camera::updateCamera(atlas::utils::Time const& time)
        {
            UNUSED(time);
        }

        void Camera::updateCameraBounds(atlas::math::Point2 const& size)
        {
            UNUSED(size);
        }

        void Camera::resetCamera()
        { }

        math::Matrix4 Camera::getCameraMatrix()
        {
            return math::Matrix4();
        }

        float Camera::getCameraFOV()
        {
            return 45.0f;
        }

    }
}