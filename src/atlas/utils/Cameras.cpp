#include "Cameras.hpp"

#include <zeus/float.hpp>

namespace atlas::utils
{
    using namespace atlas::math;

    void SimpleCamera::set_movement_type(Movements movement_type)
    {
        _movement = movement_type;
    }

    void SimpleCamera::on_mouse_down(math::Point2 const& point)
    {
        _last_pos = point;
    }

    void SimpleCamera::on_mouse_move(math::Point2 const& point)
    {
        Point2 delta = point - _last_pos;

        switch (_movement)
        {
        case Movements::dolly: {
            delta *= -0.5f;

            auto view = glm::normalize(_position - _target);

            _position += (view * delta.x);
            _target += (view * delta.x);
            break;
        }

        case Movements::track: {
            delta *= 0.05f;

            // Compute the full frame.
            auto view  = glm::normalize(_position - _target);
            auto right = glm::normalize(glm::cross(_up, view));
            auto up    = glm::cross(view, right);

            right *= -delta.x;
            up *= delta.y;

            _position += (right + up);
            _target += (right + up);
            _azimuth_target += (right + up);
            break;
        }

        case Movements::tumble: {
            delta *= 0.5f;

            auto view  = glm::normalize(_position - _target);
            auto right = glm::normalize(glm::cross(_up, view));

            Vector target{0};
            auto azimuth = glm::translate(Matrix4{1.0f}, _azimuth_target) *
                           glm::rotate(Matrix4{1.0f},
                                       glm::radians(delta.x),
                                       Vector{0, 1, 0}) *
                           glm::translate(Matrix4{1.0f}, -_azimuth_target);

            auto elevation =
                glm::translate(Matrix4{1.0f}, target) *
                glm::rotate(Matrix4{1.0f}, glm::radians(delta.y), right) *
                glm::translate(Matrix4{1.0f}, -target);

            _tumble = elevation * _tumble * azimuth;
            break;
        }

        case Movements::none: {
            break;
        }
        }

        _last_pos = point;
    }

    void SimpleCamera::on_mouse_up([[maybe_unused]] math::Point2 const& point)
    {
        _movement = Movements::none;
    }

    void SimpleCamera::on_mouse_scroll(math::Point2 const& offset)
    {
        if (zeus::geq(_fov, 1.0f) && zeus::leq(_fov, 45.0f))
        {
            _fov -= offset.y;
        }

        _fov = (zeus::leq(_fov, 1.0f)) ? 1.0f : _fov;
        _fov = (zeus::geq(_fov, 45.0f)) ? 45.0f : _fov;
    }

    math::Matrix4 SimpleCamera::compute_view_matrix() const
    {
        return glm::lookAt(_position, _target, _up) * _tumble;
    }

    void SimpleCamera::reset_camera()
    {
        _position       = {30, 30, 30};
        _target         = {0, 0, 0};
        _azimuth_target = _target;
        _up             = {0, 1, 0};
        _tumble         = {Matrix4{1.0f}};
        _fov            = 45.0f;
    }
} // namespace atlas::utils
