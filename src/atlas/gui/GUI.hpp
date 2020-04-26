#pragma once

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include <array>
#include <string>

namespace atlas::gui
{
    struct GuiRenderData
    {
        GLuint fontTexture{};
        GLuint shaderHandle{};
        GLuint vertHandle{};
        GLuint fragHandle{};

        int texAttribLocation{};
        int projMtxAttribLocation{};

        int vtxPosAttribLocation{};
        int vtxUVAttribLocation{};
        int vtxColorAttribLocation{};

        GLuint vboHandle{};
        GLuint elementsHandle{};
    };

    struct GuiWindowData
    {
        GLFWwindow* window{nullptr};
        double time{0.0};
        std::array<bool, 5> mouseJustPressed;
        std::array<GLFWcursor*, ImGuiMouseCursor_COUNT> mouseCursors;
    };

    bool initializeGuiRenderData(GuiRenderData& data);
    void destroyGuiRenderData(GuiRenderData& data);
    void renderGuiFrame(GuiRenderData const& data);

    bool initializeGuiWindowData(GuiWindowData& data);
    void setGuiWindow(GuiWindowData& data, GLFWwindow* window);
    void startGuiWindowFrame(GuiWindowData& data);
    void updateGuiWindowFrame(GuiWindowData& data);
    void destroyGuiWindow(GuiWindowData& data);

    void
    mousePressedCallback(GuiWindowData& data, int button, int action, int mode);
    void mouseScrollCallback(double xOffset, double yOffset);
    void keyPressCallback(int key, int scancode, int action, int mods);
    void charCallback(unsigned int c);

    void newFrame(GuiWindowData& data);
    void endFrame(GuiWindowData& windowData, GuiRenderData& renderData);

} // namespace atlas::gui
