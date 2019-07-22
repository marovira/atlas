#pragma once

#include "imgui.h"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

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

    bool initializeGUI(GuiRenderData& data);
    void shutdownGUI(GuiRenderData& data);
    void renderGUIFrame(GuiRenderData const& data);

} // namespace atlas::gui
