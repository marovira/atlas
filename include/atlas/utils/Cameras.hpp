#pragma once

#include <atlas/math/Math.hpp>

namespace atlas::utils
{
    namespace MayaCamera
    {
        enum class Movements
        {
            None = 0,
            Tumble,
            Track,
            Dolly
        };

        struct CameraData
        {
            math::Point position{30};
            math::Point target{0};
            math::Vector up{0, 1, 0};
            math::Point azimuthTarget{target};
            math::Matrix4 tumble{1.0f};
            float fov{45.0f};

            math::Point2 lastPos{0};
            Movements movement{Movements::None};
        };

        void mouseDown(CameraData& data, math::Point2 const& point);
        void mouseMove(CameraData& data, math::Point2 const& point);
        void mouseUp(CameraData& data);
        void mouseScroll(CameraData& data, math::Point2 const& offset);
        math::Matrix4 computeViewMatrix(CameraData const& data);
        void resetCamera(CameraData& data);
    } // namespace MayaCamera

} // namespace atlas::utils
