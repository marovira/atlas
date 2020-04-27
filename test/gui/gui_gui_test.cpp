#include <atlas/glx/context.hpp>
#include <atlas/gui/gui.hpp>

#include <fmt/printf.h>

#include <catch2/catch.hpp>

#if defined(ATLAS_BUILD_GUI_TESTS)
void errorCallback(int code, char const* message)
{
    fmt::print("error ({}):{}\n", code, message);
}

namespace gui = atlas::gui;
using namespace atlas::glx;

TEST_CASE("[gui] - gui functions: explicit GUI", "[gui]")
{
    REQUIRE(initialize_glfw(errorCallback));

    WindowSettings settings;
    settings.size.width  = 1280;
    settings.size.height = 720;
    auto window          = create_glfw_window(settings);
    REQUIRE(window != nullptr);

    gui::UIRenderData ui_render_data;
    gui::UIWindowData ui_window_data;

    auto mouse_press_callback =
        [&ui_window_data](int button, int action, int mode, double, double) {
            gui::mouse_pressed_callback(ui_window_data, button, action, mode);
        };

    auto mouse_scroll_callback = [](double xOffset, double yOffset) {
        gui::mouse_scroll_callback(xOffset, yOffset);
    };

    auto key_press_callback = [](int key, int scancode, int action, int mods) {
        gui::key_press_callback(key, scancode, action, mods);
    };

    auto char_callback = [](unsigned int codepoint) {
        gui::char_callback(codepoint);
    };

    WindowCallbacks callbacks;
    callbacks.mouse_press_callback  = mouse_press_callback;
    callbacks.mouse_scroll_callback = mouse_scroll_callback;
    callbacks.key_press_callback    = key_press_callback;
    callbacks.char_callback         = char_callback;

    bind_window_callbacks(window, callbacks);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    REQUIRE(create_gl_context(window, settings.version));

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    initialize_ui_window_data(ui_window_data);
    initialize_ui_render_data(ui_render_data);
    set_ui_window(ui_window_data, window);
    bool show_demo{true};
    bool window_ok{false};

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0.45f, 0.55f, 0.60f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        start_ui_window_frame(ui_window_data);
        ImGui::NewFrame();

        ImGui::ShowDemoWindow(&show_demo);
        {
            ImGui::Begin("Test window");
            ImGui::Text("Please press on the button below");
            if (ImGui::Button("OK"))
            {
                window_ok = true;
                glfwSetWindowShouldClose(window, true);
            }
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                        1000.0f / ImGui::GetIO().Framerate,
                        ImGui::GetIO().Framerate);
            ImGui::End();
        }

        ImGui::Render();
        render_ui_frame(ui_render_data);

        glfwSwapBuffers(window);
        glfwPollEvents();
        update_ui_window_frame(ui_window_data);
    }

    gui::destroy_ui_render_data(ui_render_data);
    gui::destroy_ui_window(ui_window_data);
    ImGui::DestroyContext();

    destroy_glfw_window(window);
    terminate_glfw();

    REQUIRE(window_ok);
}

TEST_CASE("[gui] - gui functions: helper GUI", "[gui]")
{
    REQUIRE(initialize_glfw(errorCallback));

    WindowSettings settings;
    settings.size.width  = 1280;
    settings.size.height = 720;
    auto window          = create_glfw_window(settings);
    REQUIRE(window != nullptr);

    gui::UIRenderData ui_render_data;
    gui::UIWindowData ui_window_data;

    auto mouse_press_callback =
        [&ui_window_data](int button, int action, int mode, double, double) {
            gui::mouse_pressed_callback(ui_window_data, button, action, mode);
        };

    auto mouse_scroll_callback = [](double xOffset, double yOffset) {
        gui::mouse_scroll_callback(xOffset, yOffset);
    };

    auto key_press_callback = [](int key, int scancode, int action, int mods) {
        gui::key_press_callback(key, scancode, action, mods);
    };

    WindowCallbacks callbacks;
    callbacks.mouse_press_callback  = mouse_press_callback;
    callbacks.mouse_scroll_callback = mouse_scroll_callback;
    callbacks.key_press_callback    = key_press_callback;

    bind_window_callbacks(window, callbacks);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    REQUIRE(create_gl_context(window, settings.version));

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    initialize_ui_window_data(ui_window_data);
    initialize_ui_render_data(ui_render_data);
    set_ui_window(ui_window_data, window);
    bool window_ok{false};

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0.45f, 0.55f, 0.60f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        new_frame(ui_window_data);
        {
            ImGui::Begin("Test window");
            ImGui::Text("Please press on the button below");
            if (ImGui::Button("OK"))
            {
                window_ok = true;
                glfwSetWindowShouldClose(window, true);
            }
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                        1000.0f / ImGui::GetIO().Framerate,
                        ImGui::GetIO().Framerate);
            ImGui::End();
        }
        end_frame(ui_window_data, ui_render_data);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    gui::destroy_ui_render_data(ui_render_data);
    gui::destroy_ui_window(ui_window_data);
    ImGui::DestroyContext();

    destroy_glfw_window(window);
    terminate_glfw();

    REQUIRE(window_ok);
}
#endif
