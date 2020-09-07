#include "app_properties_widget.hpp"

#include <atlas.hpp>

using namespace atlas;

namespace widgets
{
    AppPropertiesWidget::AppPropertiesWidget(std::string app_name) :
        m_app_name{app_name}
    {}

    void AppPropertiesWidget::draw()
    {
        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);

        ImGui::Begin("App Properties");
        ImGui::Text("App Info");
        ImGui::Separator();
        ImGui::Text("App name: %s", m_app_name.c_str());
        ImGui::Text("Window size: %d x %d", width, height);

        ImGui::Spacing();
        ImGui::Text("GL Info");
        ImGui::Separator();
        ImGui::Text("Vendor graphic card: %s", glGetString(GL_VENDOR));
        ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
        ImGui::Text("OpenGL version: %s", glGetString(GL_VERSION));
        ImGui::Text("GLSL version: %s",
                    glGetString(GL_SHADING_LANGUAGE_VERSION));
        ImGui::Text("Atlas version: %s", atlas_version_string.data());
        ImGui::End();
    }
} // namespace widgets
