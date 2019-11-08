#include "atlas/utils/Cameras.hpp"
#include "atlas/core/Float.hpp"

namespace atlas::utils
{
    using namespace atlas::math;

    namespace MayaCamera
    {
        void mouseDown(CameraData& data, math::Point2 const& point)
        {
            data.lastPos = point;
        }

        void mouseMove(CameraData& data, math::Point2 const& point)
        {
            Point2 delta = point - data.lastPos;

            switch (data.movement)
            {
            case Movements::Dolly:
            {
                delta *= -0.5f;

                auto view = glm::normalize(data.position - data.target);

                data.position += (view * delta.x);
                data.target += (view * delta.x);
                break;
            }

            case Movements::Track:
            {
                delta *= 0.05f;

                // Compute the full frame.
                auto view  = glm::normalize(data.position - data.target);
                auto right = glm::normalize(glm::cross(data.up, view));
                auto up    = glm::cross(view, right);

                right *= -delta.x;
                up *= delta.y;

                data.position += (right + up);
                data.target += (right + up);
                data.azimuthTarget += (right + up);
                break;
            }

            case Movements::Tumble:
            {
                delta *= 0.5f;

                auto view  = glm::normalize(data.position - data.target);
                auto right = glm::normalize(glm::cross(data.up, view));

                Vector target{0};
                auto azimuth =
                    glm::translate(Matrix4{1.0f}, data.azimuthTarget) *
                    glm::rotate(
                        Matrix4{1.0f}, glm::radians(delta.x), Vector{0, 1, 0}) *
                    glm::translate(Matrix4{1.0f}, -data.azimuthTarget);

                auto elevation =
                    glm::translate(Matrix4{1.0f}, target) *
                    glm::rotate(Matrix4{1.0f}, glm::radians(delta.y), right) *
                    glm::translate(Matrix4{1.0f}, -target);

                data.tumble = elevation * data.tumble * azimuth;
                break;
            }

            case Movements::None:
            {
                break;
            }
            }

            data.lastPos = point;
        }

        void mouseUp(CameraData& data)
        {
            data.movement = Movements::None;
        }

        void mouseScroll(CameraData& data, math::Point2 const& offset)
        {
            if (core::geq(data.fov, 1.0f) && core::leq(data.fov, 45.0f))
            {
                data.fov -= offset.y;
            }

            data.fov = (core::leq(data.fov, 1.0f)) ? 1.0f : data.fov;
            data.fov = (core::geq(data.fov, 45.0f)) ? 45.0f : data.fov;
        }

        math::Matrix4 computeViewMatrix(CameraData const& data)
        {
            return glm::lookAt(data.position, data.target, data.up) *
                   data.tumble;
        }

        void resetCamera(CameraData& data)
        {
            data.position      = {30, 30, 30};
            data.target        = {0, 0, 0};
            data.azimuthTarget = data.target;
            data.up            = {0, 1, 0};
            data.tumble        = {Matrix4{1.0f}};
            data.fov           = 45.0f;
        }

    } // namespace MayaCamera
} // namespace atlas::utils
