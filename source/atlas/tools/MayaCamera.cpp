#include "atlas/tools/MayaCamera.hpp"
#include "atlas/core/Float.hpp"

namespace atlas
{
    namespace tools
    {
        struct MayaCamera::MayaCameraImpl
        {
            MayaCameraImpl() :
                position(30),
                target(0),
                up(0, 1, 0),
                azimuthTarget(target),
                tumble(1.0f),
                fov(45.0f)
            { }

            atlas::math::Matrix4 computeViewMatrix()
            {
                return glm::lookAt(position, target, up) * tumble;
            }

            void reset()
            {
                position = { 30, 30, 30 };
                target = { 0, 0, 0 };
                azimuthTarget = target;
                up = { 0, 1, 0 };
                tumble = glm::mat4(1.0f);
                fov = 45.0f;
            }

            atlas::math::Point position;
            atlas::math::Point target;
            atlas::math::Vector up;
            atlas::math::Point azimuthTarget;
            atlas::math::Matrix4 tumble;
            float fov;

            atlas::math::Point2 lastPos;
            MayaMovements movement;
        };

        MayaCamera::MayaCamera() :
            mImpl(std::make_unique<MayaCameraImpl>())
        { }

        MayaCamera::~MayaCamera()
        { }

        void MayaCamera::mouseDown(atlas::math::Point2 const& point)
        {
            mImpl->lastPos = point;
        }

        void MayaCamera::mouseMove(atlas::math::Point2 const& point)
        {
            auto delta = point - mImpl->lastPos;

            switch (mImpl->movement)
            {
            case MayaMovements::Dolly:
            {
                delta *= -0.5f;

                auto view = glm::normalize(mImpl->position - mImpl->target);

                mImpl->position += (view * delta.x);
                mImpl->target += (view * delta.x);
                break;
            }

            case MayaMovements::Track:
            {
                delta *= 0.05f;

                // Compute the full frame.
                auto view = glm::normalize(mImpl->position - mImpl->target);
                auto right = glm::normalize(glm::cross(mImpl->up, view));
                auto up = glm::cross(view, right);

                right *= -delta.x;
                up *= delta.y;

                mImpl->position += (right + up);
                mImpl->target += (right + up);
                mImpl->azimuthTarget += (right + up);
                break;
            }

            case MayaMovements::Tumble:
            {
                delta *= 0.5f;

                // Compute the full frame.
                auto view = glm::normalize(mImpl->position - mImpl->target);
                auto right = glm::normalize(glm::cross(mImpl->up, view));
                auto up = glm::cross(view, right);

                auto target = math::Vector(0, 0, 0);
                auto azimuth =
                    glm::translate(glm::mat4(1.0f), mImpl->azimuthTarget) *
                    glm::rotate(glm::mat4(1.0f), glm::radians(delta.x),
                        glm::vec3(0, 1, 0)) *
                    glm::translate(glm::mat4(1.0f), -mImpl->azimuthTarget);

                auto elevation =
                    glm::translate(glm::mat4(1.0f), target) *
                    glm::rotate(glm::mat4(1.0f), glm::radians(delta.y), right) *
                    glm::translate(glm::mat4(1.0f), -target);

                mImpl->tumble = elevation * mImpl->tumble * azimuth;
                break;
            }

            case MayaMovements::None:
            {
                break;
            }
            }

            mImpl->lastPos = point;
        }

        void MayaCamera::mouseUp()
        {
            mImpl->movement = MayaMovements::None;
        }

        void MayaCamera::mouseScroll(atlas::math::Point2 const& offset)
        {
            if (core::geq(mImpl->fov, 1.0f) && core::leq(mImpl->fov, 45.0f))
            {
                mImpl->fov -= offset.y;
            }

            mImpl->fov = (core::leq(mImpl->fov, 1.0f)) ? 1.0f : mImpl->fov;
            mImpl->fov = (core::geq(mImpl->fov, 45.0f)) ? 45.0f : mImpl->fov;
        }

        void MayaCamera::resetCamera()
        {
            mImpl->reset();
        }

        void MayaCamera::setMovement(MayaMovements movement)
        {
            mImpl->movement = movement;
        }

        atlas::math::Point MayaCamera::getCameraPosition() const
        {
            return mImpl->position;
        }

        atlas::math::Matrix4 MayaCamera::getCameraMatrix() const
        {
            return mImpl->computeViewMatrix();
        }

        float MayaCamera::getCameraFOV() const
        {
            return mImpl->fov;
        }
    }
}