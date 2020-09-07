#pragma once

#include "widget.hpp"

namespace atlas::gui::widgets
{
    class AppPropertiesWidget : public Widget
    {
    public:
        AppPropertiesWidget(std::string app_name);
        ~AppPropertiesWidget() = default;

        void draw() override;

    private:
        std::string m_app_name;
    };
} // namespace atlas::gui::widgets
