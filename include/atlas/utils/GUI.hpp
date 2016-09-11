/**
 * \file GUI.hpp
 * \brief Defines functions for ImGUI.
 */

#ifndef ATLAS_INCLUDE_ATLAS_UTILS_GUI_HPP
#define ATLAS_INCLUDE_ATLAS_UTILS_GUI_HPP

#pragma once

#include "Utils.hpp"
#include "atlas/gl/GL.hpp"
#include "atlas/core/ImGUI.hpp"
#include "atlas/core/GLFW.hpp"
#include "atlas/core/Core.hpp"

#define GUI atlas::utils::Gui::getInstance()

namespace atlas
{
    namespace utils
    {
        struct GuiData
        {

            GuiData() :
                time(0.0),
                mousePressed{ false, false, false },
                mouseWheel(0.0f),
                fontTexture(0),
                shaderHandle(0),
                vertHandle(0),
                fragHandle(0),
                attribLocationTex(0),
                attribLocationProjMtx(0),
                attribLocationPosition(0),
                attribLocationUV(0),
                attribLocationColor(0),
                vboHandle(0),
                vaoHandle(0),
                elementsHandle(0),
                window(nullptr)
            { }

            double time;
            bool mousePressed[3];
            float mouseWheel;
            GLuint fontTexture;
            int shaderHandle, vertHandle, fragHandle;
            int attribLocationTex, attribLocationProjMtx;
            int attribLocationPosition, attribLocationUV, attribLocationColor;
            unsigned int vboHandle, vaoHandle, elementsHandle;
            GLFWwindow* window;
        };

        class Gui
        {
            Gui();
            ~Gui();

            Gui(Gui const&) = delete;
            void operator =(Gui const&) = delete;

        public:
            static Gui& getInstance();

            void mousePressed(int button, int action, int mods);
            void mouseMoved(double xPos, double yPos);
            void mouseScroll(double xOffset, double yOffset);
            void keyPress(int key, int scancode, int action, int mods);
            void screenResize(int width, int height);
            void update(atlas::core::Time<> const& t);

            void newFrame();
            void setFontScale(float scale = 1.0f);

            GuiData& getData();

        private:
            void createFontsTexture();
            void createDeviceObjects();
            void invalidateDeviceObjects();

            GuiData mData;
        };
    }
}

#endif