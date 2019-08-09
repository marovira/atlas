#include <atlas/glx/Context.hpp>
#include <atlas/gui/GUI.hpp>
#include <fmt/printf.h>

using namespace atlas;

// We need to define an error callback function for GLFW, so we do that here.
void errorCallback(int code, char const* message)
{
    fmt::print("error ({}):{}\n", code, message);
}

int main()
{
    // First let's create the structs that we're going to be using. First the
    // window struct.
    glx::WindowSettings settings;
    settings.size.width  = 1280;
    settings.size.height = 720;
    settings.title       = "Hello Triangle";

    // Next, we create the structs needed for the GUI.
    gui::GuiRenderData guiRenderData;
    gui::GuiWindowData guiWindowData;

    // Next, let's make the callbacks struct.
    glx::WindowCallbacks callbacks;
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

    auto charCallback = [](unsigned int codepoint) {
        gui::charCallback(codepoint);
    };

    glx::WindowCallbacks callbacks;
    callbacks.mousePressCallback  = mousePressCallback;
    callbacks.mouseScrollCallback = mouseScrollCallback;
    callbacks.keyPressCallback    = keyPressCallback;
    callbacks.charCallback        = charCallback;

    // Now that we have everything setup, let's start initializing things.
    // First, let's initialize GLFW.
    glx::initializeGLFW(errorCallback);

    // Next comes the window.
    auto window = createGLFWWindow(settings);

    // Bind the callbacks.
    glx::bindWindowCallbacks(window, callbacks);

    // Now bind the context.
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Create the OpenGL context.
    glx::createGLContext(window, settings.version);

    // Initialize ImGui next.
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    // Initialize the GUI structs. Remember to set the window!
    gui::initializeGuiWindowData(guiWindowData);
    gui::initializeGuiRenderData(guiRenderData);
    setGuiWindow(guiWindowData, window);

    // Now we are ready for our main loop.
    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Do your GL stuff here.

        // Now draw the GUI elements.
        gui::newFrame(guiWindowData);
        ImGui::NewFrame();
        ImGui::Begin("HUD");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);
        ImGui::End();

        // Render the GUI.
        ImGui::Render();
        gui::endFrame(guiWindowData, guiRenderData);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up our resources.
    gui::destroyGuiRenderData(guiRenderData);
    gui::destroyGuiWindow(guiWindowData);
    ImGui::DestroyContext();

    glx::destroyGLFWWindow(window);
    glx::terminateGLFW();

    return 0;
}
