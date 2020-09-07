#pragma once

#include "widget.hpp"

#include <list>

namespace atlas::gui::widgets
{
    class FPSWidget : public Widget
    {
    public:
        FPSWidget();
        ~FPSWidget() = default;

        void draw() override;

    private:
        float m_min_fps;
        float m_max_fps;
        std::list<float> m_fps_points;
    };
} // namespace atlas::gui::widgets
