#include "atlas/utils/Camera.hpp"
#include "atlas/core/Macros.hpp"
#include "atlas/core/Time.hpp"

namespace atlas
{
    namespace utils
    {
        Camera::Camera() 
        { }

        Camera::~Camera()
        { }

        void Camera::mouseDown(math::Point2 const& point)
        {
            UNUSED(point);
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

        void Camera::updateCamera(atlas::core::Time<> const& time)
        {
            UNUSED(time);
        }

        void Camera::updateCameraBounds(atlas::math::Point2 const& size)
        {
            UNUSED(size);
        }

        void Camera::resetCamera()
        { }

        math::Matrix4 Camera::getCameraMatrix() const
        {
            return math::Matrix4();
        }

        atlas::math::Point Camera::getCameraPosition() const
        {
            return math::Point();
        }

        float Camera::getCameraFOV() const
        {
            return 45.0f;
        }

    }
}