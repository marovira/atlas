/**
 * \file GUI.hpp
 * \brief Defines functions for ImGUI.
 */

#ifndef ATLAS_INCLUDE_ATLAS_UTILS_GUI_HPP
#define ATLAS_INCLUDE_ATLAS_UTILS_GUI_HPP

#pragma once

#include "atlas/gl/GL.hpp"
#include "atlas/core/ImGUI.hpp"
#include "atlas/core/GLFW.hpp"
#include "atlas/core/Core.hpp"

namespace atlas
{
    namespace utils
    {
        /**
         * \class GuiData
         * \brief Holds the data used by ImGui.
         * 
         * This encapsulates all of the required information for ImGUI to 
         * work with GLFW.
         */
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

            /**
             * \var time
             * The current tick.
             */
            double time;

            /**
             * \var mousePressed
             * The three buttons of the mouse.
             */
            bool mousePressed[3];

            /**
             * \var mouseWheel
             * The current scroll offset.
             */
            float mouseWheel;

            /**
             * \var fontTexture
             * The handle for the font texture.
             */
            GLuint fontTexture;

            /**
             * \var shaderHandle
             * The handle for the shader program.
             */
            int shaderHandle;

            /**
             * \var vertHandle
             * The handle for the vertex shader.
             */
            int vertHandle;

            /**
             * \var fragHandle.
             * The handle for the fragment shader.
             */
            int fragHandle;

            /**
             * \var attribLocationTex
             * The location for the texture attribute.
             */
            int attribLocationTex;

            /**
             * \var attribLocationProjMtx
             * The location for the projection matrix attribute.
             */
            int attribLocationProjMtx;

            /**
             * \var attribLocationPosition
             * The location for the position attribute.
             */
            int attribLocationPosition;

            /**
             * \var attribLocationUV
             * The location for the uv attribute.
             */
            int attribLocationUV; 

            /**
             * \var attribLocationColor
             * The location for the color attribute.
             */
            int attribLocationColor;

            /**
             * \var vboHandle
             * The handle for the vbo.
             */
            unsigned int vboHandle; 

            /**
             * \var vaoHandle
             * The handle for the vao.
             */
            unsigned int vaoHandle; 

            /**
             * \var elementsHandle
             * The handle for the elements buffer.
             */
            unsigned int elementsHandle;

            /**
             * \var window
             * The current window.
             */
            GLFWwindow* window;
        };

        /**
         * \class Gui
         * \brief Defines a wrapper for ImGui operations.
         * 
         * Since ImGui requires some interactions with the window, this class
         * abstracts that behaviour so the user doesn't have to worry about it.
         * The API corresponds to the ImGUI examples with GLFW, so all that is
         * needed is for the functions to be called in the appropriate overrides
         * in the Scene class and everything will work as expected.
         * 
         * This is implemented as a static singleton due to the fact that 
         * ImGUI uses a C API, and thus requires static access to functions
         * that are encapsulated here.
         */
        class Gui
        {
            Gui();
            ~Gui();

            Gui(Gui const&) = delete;
            void operator =(Gui const&) = delete;

        public:
            /**
             * Returns the current instance.
             * 
             * \return A reference to the current instance.
             */
            static Gui& getInstance();

            /**
             * This should be called on the mousePressEvent function from
             * the Scene class.
             * 
             * \param[in] button The mouse button that was pressed.
             * \param[in] action The action that was preformed.
             * \param[in] mods The key modifiers that were pressed (if any).
             */
            void mousePressed(int button, int action, int mods);

            /**
             * This should be called on the mouseMoveEvent function from the
             * Scene class.
             * 
             * \param[in] xPos The x-position of the cursor.
             * \param[in] yPos The y-position of the cursor.
             */
            void mouseMoved(double xPos, double yPos);

            /**
             * This should be called on the mouseScrollEvent function from the
             * Scene class.
             * 
             * \param[in] xOffset The x-offset of the scroll.
             * \param[in] yOffset The y-offset of the scroll.
             */
            void mouseScroll(double xOffset, double yOffset);

            /**
             * This should be called on the keyPressEvent function from the
             * Scene class.
             * 
             * \param[in] key The key that was paress/released.
             * \param[in] scancode The scancode of the key.
             * \param[in] action Whether the key was pressed/released.
             * \param[in] mods The modifiers that were pressed (if any).
             */
            void keyPress(int key, int scancode, int action, int mods);

            /**
             * This should be called on the screenResizeEvent function from
             * the Scene class.
             * 
             * \param[in] width The new width of the screen.
             * \param[in] height The new height of the screen.
             */
            void screenResize(int width, int height);

            /**
             * This should be called on the updateScene function from the 
             * Scene class.
             * 
             * \param[in] t The current time.
             */
            void update(atlas::core::Time<> const& t);

            /**
             * This should be called at the top of the renderScene function
             * or before any ImGUI commands are issued.
             */
            void newFrame();

            /**
             * Gets the current Gui data.
             * 
             * \return A reference to the current gui data.
             */
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