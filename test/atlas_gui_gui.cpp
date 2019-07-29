#include <atlas/glx/Context.hpp>
#include <atlas/gui/GUI.hpp>

#include <atlas/core/FMT.hpp>

#include <catch2/catch.hpp>

#if defined(ATLAS_BUILD_GUI_TESTS)
void errorCallback(int code, char const* message)
{
    fmt::print("error ({}):{}\n", code, message);
}

namespace gui = atlas::gui;
using namespace atlas::glx;

TEST_CASE("Checking ImGui demo", "[gui]")
{
    REQUIRE(initializeGLFW(errorCallback));

    WindowSettings settings;
    settings.size.width  = 1280;
    settings.size.height = 720;
    auto window          = createGLFWWindow(settings);
    REQUIRE(window != nullptr);

    gui::GuiRenderData guiRenderData;
    gui::GuiWindowData guiWindowData;

    auto mousePressCallback = [&guiWindowData](int button, int action,
                                               int mode) {
        gui::mousePressedCallback(guiWindowData, button, action, mode);
    };

    auto mouseScrollCallback = [](double xOffset, double yOffset) {
        gui::mouseScrollCallback(xOffset, yOffset);
    };

    auto keyPressCallback = [](int key, int scancode, int action, int mods) {
        gui::keyPressCallback(key, scancode, action, mods);
    };

    WindowCallbacks callbacks;
    callbacks.mousePressCallback  = mousePressCallback;
    callbacks.mouseScrollCallback = mouseScrollCallback;
    callbacks.keyPressCallback    = keyPressCallback;

    bindWindowCallbacks(window, callbacks);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    REQUIRE(createGLContext(window, settings.version));

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    initializeGuiWindowData(guiWindowData);
    initializeGuiRenderData(guiRenderData);
    setGuiWindow(guiWindowData, window);
    bool showDemo{true};
    bool windowOk{false};

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        updateGuiWindowFrame(guiWindowData);
        ImGui::NewFrame();

        ImGui::ShowDemoWindow(&showDemo);
        {
            ImGui::Begin("Test window");
            ImGui::Text("Please press on the button below");
            if (ImGui::Button("OK"))
            {
                windowOk = true;
                glfwSetWindowShouldClose(window, true);
            }
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                        1000.0f / ImGui::GetIO().Framerate,
                        ImGui::GetIO().Framerate);
            ImGui::End();
        }

        ImGui::Render();
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0.45f, 0.55f, 0.60f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderGuiFrame(guiRenderData);
        glfwSwapBuffers(window);
    }

    gui::destroyGuiRenderData(guiRenderData);
    gui::destroyGuiWindow(guiWindowData);
    destroyGLFWWindow(window);
    terminateGLFW();

    REQUIRE(windowOk);
}
#endif
