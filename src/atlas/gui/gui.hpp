#pragma once

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include <array>
#include <string>

namespace atlas::gui
{
    struct UIRenderData
    {
        GLuint font_texture{};
        GLuint shader_handle{};
        GLuint vert_handle{};
        GLuint frag_handle{};

        int tex_attrib_location{};
        int proj_mtx_attrib_location{};

        int vtx_pos_attrib_location{};
        int vts_uv_attrib_location{};
        int vtx_colour_attrib_location{};

        GLuint vbo_handle{};
        GLuint elements_handle{};
    };

    struct UIWindowData
    {
        GLFWwindow* window{nullptr};
        double time{0.0};
        std::array<bool, 5> mouse_just_pressed;
        std::array<GLFWcursor*, ImGuiMouseCursor_COUNT> mouse_cursors;
    };

    bool initialize_ui_render_data(UIRenderData& data);
    void destroy_ui_render_data(UIRenderData& data);
    void render_ui_frame(UIRenderData const& data);

    bool initialize_ui_window_data(UIWindowData& data);
    void set_ui_window(UIWindowData& data, GLFWwindow* window);
    void start_ui_window_frame(UIWindowData& data);
    void update_ui_window_frame(UIWindowData& data);
    void destroy_ui_window(UIWindowData& data);

    void mouse_pressed_callback(UIWindowData& data,
                                int button,
                                int action,
                                int mode);
    void mouse_scroll_callback(double x_offset, double y_offset);
    void key_press_callback(int key, int scancode, int action, int mods);
    void char_callback(unsigned int c);

    void new_frame(UIWindowData& data);
    void end_frame(UIWindowData& window_data, UIRenderData& render_data);

} // namespace atlas::gui
