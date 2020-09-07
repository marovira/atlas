#include "fps_widget.hpp"

#include <algorithm>
#include <limits>
#include <numeric>
#include <vector>

static constexpr std::size_t max_size{50};

namespace widgets
{
    FPSWidget::FPSWidget() :
        m_min_fps{std::numeric_limits<float>::infinity()},
        m_max_fps{-std::numeric_limits<float>::infinity()},
        m_fps_points(max_size, 0.0f)
    {}

    void FPSWidget::draw()
    {
        float current_fps = ImGui::GetIO().Framerate;

        m_min_fps = std::min(m_min_fps, current_fps);
        m_max_fps = std::max(m_max_fps, current_fps);

        m_fps_points.push_back(current_fps);
        m_fps_points.pop_front();

        float average =
            std::accumulate(m_fps_points.begin(), m_fps_points.end(), 0.0f) /
            m_fps_points.size();

        std::vector<float> points{m_fps_points.begin(), m_fps_points.end()};

        ImGui::Begin("FPS Info");
        ImGui::Text("Min FPS: %.1f", m_min_fps);
        ImGui::Text("Max FPS: %.1f", m_max_fps);
        ImGui::Text("Average FPS: %.1f", average);
        ImGui::PlotLines("Frame times",
                         points.data(),
                         static_cast<int>(points.size()),
                         0,
                         nullptr,
                         0.0f,
                         200.0f,
                         ImVec2(0, 80));
        ImGui::End();
    }
} // namespace widgets
