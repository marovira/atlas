#pragma once

#include <atlas/math/math.hpp>

namespace atlas::utils
{
    class Camera
    {
    public:
        virtual ~Camera() = default;

        virtual void on_mouse_down([[maybe_unused]] math::Point2 const& point)
        {}

        virtual void on_mouse_move([[maybe_unused]] math::Point2 const& point)
        {}

        virtual void on_mouse_up([[maybe_unused]] math::Point2 const& point)
        {}

        virtual void on_mouse_scroll([[maybe_unused]] math::Point2 const& point)
        {}

        virtual void on_key_press([[maybe_unused]] int key,
                                  [[maybe_unused]] int action,
                                  [[maybe_unused]] int mods)
        {}

        virtual math::Matrix4 compute_view_matrix() const
        {
            return math::Matrix4{1.0f};
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
        void on_mouse_down(math::Point2 const& point) override;
        void on_mouse_move(math::Point2 const& point) override;
        void on_mouse_up(math::Point2 const& point) override;
        void on_mouse_scroll(math::Point2 const& offset) override;

        math::Matrix4 compute_view_matrix() const override;

        void reset_camera() override;

    private:
        math::Point m_position{30};
        math::Point m_target{0};
        math::Vector m_up{0, 1, 0};
        math::Point m_azimuth_target{m_target};
        math::Matrix4 m_tumble{1.0f};
        float m_fov{45.0f};

        math::Point2 m_last_pos{0};
        Movements m_movement{Movements::none};
    };

} // namespace atlas::utils
