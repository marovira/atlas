#pragma once

#include <atlas/math/Math.hpp>

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
        math::Point _position{30};
        math::Point _target{0};
        math::Vector _up{0, 1, 0};
        math::Point _azimuth_target{_target};
        math::Matrix4 _tumble{1.0f};
        float _fov{45.0f};

        math::Point2 _last_pos{0};
        Movements _movement{Movements::none};
    };

} // namespace atlas::utils
