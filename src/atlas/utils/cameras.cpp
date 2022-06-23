#include "cameras.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <zeus/float.hpp>

namespace atlas::utils
{
    void SimpleCamera::set_movement_type(Movements movement_type)
    {
        m_movement = movement_type;
    }

    void SimpleCamera::on_mouse_down(glm::vec2 const& point)
    {
        m_last_pos = point;
    }

    void SimpleCamera::on_mouse_move(glm::vec2 const& point)
    {
        glm::vec2 delta = point - m_last_pos;

        switch (m_movement)
        {
        case Movements::dolly:
        {
            delta *= -0.5f;

            auto view = glm::normalize(m_position - m_target);

            m_position += (view * delta.x);
            m_target += (view * delta.x);
            break;
        }

        case Movements::track:
        {
            delta *= 0.05f;

            // Compute the full frame.
            auto view  = glm::normalize(m_position - m_target);
            auto right = glm::normalize(glm::cross(m_up, view));
            auto up    = glm::cross(view, right);

            right *= -delta.x;
            up *= delta.y;

            m_position += (right + up);
            m_target += (right + up);
            m_azimuth_target += (right + up);
            break;
        }

        case Movements::tumble:
        {
            delta *= 0.5f;

            auto view  = glm::normalize(m_position - m_target);
            auto right = glm::normalize(glm::cross(m_up, view));

            glm::vec3 target{0};
            auto azimuth = glm::translate(glm::mat4{1.0f}, m_azimuth_target) *
                           glm::rotate(glm::mat4{1.0f},
                                       glm::radians(delta.x),
                                       glm::vec3{0, 1, 0}) *
                           glm::translate(glm::mat4{1.0f}, -m_azimuth_target);

            auto elevation =
                glm::translate(glm::mat4{1.0f}, target) *
                glm::rotate(glm::mat4{1.0f}, glm::radians(delta.y), right) *
                glm::translate(glm::mat4{1.0f}, -target);

            m_tumble = elevation * m_tumble * azimuth;
            break;
        }

        case Movements::none:
        {
            break;
        }
        }

        m_last_pos = point;
    }

    void SimpleCamera::on_mouse_up([[maybe_unused]] glm::vec2 const& point)
    {
        m_movement = Movements::none;
    }

    void SimpleCamera::on_mouse_scroll(glm::vec2 const& offset)
    {
        if (zeus::geq(m_fov, 1.0f) && zeus::leq(m_fov, 45.0f))
        {
            m_fov -= offset.y;
        }

        m_fov = (zeus::leq(m_fov, 1.0f)) ? 1.0f : m_fov;
        m_fov = (zeus::geq(m_fov, 45.0f)) ? 45.0f : m_fov;
    }

    glm::mat4 SimpleCamera::compute_view_matrix() const
    {
        return glm::lookAt(m_position, m_target, m_up) * m_tumble;
    }

    void SimpleCamera::reset_camera()
    {
        m_position       = {30, 30, 30};
        m_target         = {0, 0, 0};
        m_azimuth_target = m_target;
        m_up             = {0, 1, 0};
        m_tumble         = {glm::mat4{1.0f}};
        m_fov            = 45.0f;
    }
} // namespace atlas::utils
