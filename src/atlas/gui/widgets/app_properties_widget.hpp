#pragma once

#include "widget.hpp"

namespace widgets
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
} // namespace widgets
