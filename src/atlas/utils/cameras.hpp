#pragma once

#include <atlas/math/glm.hpp>

namespace atlas::utils
{
    class Camera
    {
    public:
        virtual ~Camera() = default;

        virtual void on_mouse_down([[maybe_unused]] glm::vec2 const& point)
        {}

        virtual void on_mouse_move([[maybe_unused]] glm::vec2 const& point)
        {}

        virtual void on_mouse_up([[maybe_unused]] glm::vec2 const& point)
        {}

        virtual void on_mouse_scroll([[maybe_unused]] glm::vec2 const& point)
        {}

        virtual void on_key_press([[maybe_unused]] int key,
                                  [[maybe_unused]] int action,
                                  [[maybe_unused]] int mods)
        {}

        virtual glm::mat4 compute_view_matrix() const
        {
            return glm::mat4{1.0f};
        }

        virtual void reset_camera()
        {}
    };

    class SimpleCamera : public Camera
    {
    public:
        ~SimpleCamera() = default;

        enum class Movements
        {
            none = 0,
            tumble,
            track,
            dolly
        };

        void set_movement_type(Movements movement_type);
        void on_mouse_down(glm::vec2 const& point) override;
        void on_mouse_move(glm::vec2 const& point) override;
        void on_mouse_up(glm::vec2 const& point) override;
        void on_mouse_scroll(glm::vec2 const& offset) override;

        glm::mat4 compute_view_matrix() const override;

        void reset_camera() override;

    private:
        glm::vec3 m_position{30};
        glm::vec3 m_target{0};
        glm::vec3 m_up{0, 1, 0};
        glm::vec3 m_azimuth_target{m_target};
        glm::mat4 m_tumble{1.0f};
        float m_fov{45.0f};

        glm::vec2 m_last_pos{0};
        Movements m_movement{Movements::none};
    };

} // namespace atlas::utils
