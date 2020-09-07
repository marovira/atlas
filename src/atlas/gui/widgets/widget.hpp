#pragma once

#include "../gui.hpp"

#include <memory>

namespace widgets
{
    class Widget
    {
    public:
        Widget()          = default;
        virtual ~Widget() = default;

        void set_window(GLFWwindow* window)
        {
            m_window = window;
        }

        virtual void draw() = 0;

    protected:
        GLFWwindow* m_window{nullptr};
    };

    using WidgetPtr = std::shared_ptr<Widget>;
} // namespace widgets
