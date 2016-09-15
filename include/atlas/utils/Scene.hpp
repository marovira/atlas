/**
 *	\file Scene.hpp
 *	\brief Defines the main interface for Scenes.
 */

#ifndef ATLAS_INCLUDE_ATLAS_UTILS_SCENE_HPP
#define ATLAS_INCLUDE_ATLAS_UTILS_SCENE_HPP

#pragma once

#include "Utils.hpp"
#include "atlas/core/Time.hpp"
#include "atlas/math/Math.hpp"

namespace atlas
{
    namespace utils
    {
        /**
         *	\class Scene
         *	\brief Provides a way of interfacing with the Application.
         *	
         *	The Scene is the fundamental class (right after Application) that 
         *	is used in Atlas. In order to run, the Application needs at least
         *	one Scene.
         *	
         *	With the importance of the scene outlined, here's a brief
         *	explanation of how it works. The Application class is in charge
         *	of directly handling all of GLFW. In other words, it handles
         *	all of the input callbacks, main loop, queries for events,
         *	and swaps buffers. All of this is then forwarded down to the
         *	currently active scene.
         *	
         *	The event functions are invoked from the Application callbacks
         *	after they have been processed (essentially checks whether the
         *	events that the Application handles have occurred). This means
         *	that whenever a Scene is subclassed, the user does need to know
         *	what macros GLFW offers for knowing which keys/buttons are 
         *	pressed, what modifiers, etc.
         *	
         *	The screenResizeEvent is called every time the screen changes size.
         *	Generally speaking this is the place where you want to set your
         *	projection matrix, since this is directly tied to the screen size.
         *	
         *	The next two functions are all called within the main loop of the
         *	application in succession. Of particular note is the updateScene
         *	signature. Notice that it takes in a double, while Geometry
         *	classes (the objects that are ultimately rendered) take in a Time 
         *	struct in their equivalent functions. That means that the Scene
         *	is in charge of keeping track of all the timing details and then
         *	sending it down to the objects.
         *	
         *	The reason for this split is pretty simple: the Application has no
         *	need of knowing the number of ticks its been running since it is
         *	pointless due to scene switches. As a result, Scenes need to handle
         *	this for themselves.
         *	
         *	The sceneEnded function is used by the Application to determine
         *	whether the window should close after the user closes it or 
         *	simply hide it. This allows for mechanics were the user closes
         *	a window, some processing happens, and a new result is shown.
         *	
         *	Finally, note that all Scenes are equipped with:
         *	\li A projection matrix,
         *	\li a view matrix,
         *	\li and a Time struct containing all of the timing information.
         */
        class Scene
        {
        public:
            /**
             *	Standard constructor.
             */
            Scene();

            /**
             *	Standard virtual destructor.
             */
            virtual ~Scene();

            /**
             *	This is called by the Application after a mouse event happens.
             *	\note
             *	You need to be familiar with GLFW mouse button codes in order
             *	to properly override this function.
             *	
             *	\param[in] button The button for which the event happened.
             *	\param[in] action The action that occurred.
             *	\param[in] modifiers The modifier keys that were pressed.
             *  \param[in] xPos The x position of the cursor on the screen.
             *  \param[in] yPos The y position of the cursor on the screen.
             */
            virtual void mousePressEvent(int button, int action, int modifiers,
                double xPos, double yPos);

            /**
             *	This is called by the Application after a mouse move happens.
             *	
             *	\param[in] xPos The x position of the mouse.
             *	\param[in] yPos The y position of the mouse.
             */
            virtual void mouseMoveEvent(double xPos, double yPos);

            /**
             * This is called by the Application after a mouse scroll 
             * happens.
             * \param[in] xOffset The horizontal offset of the mouse wheel.
             * \param[in] yOffset The vertical offset of the mouse wheel.
             */
            virtual void mouseScrollEvent(double xOffset, double yOffset);

            /**
             *	This is called by the Application when a key event occurs.
             *	
             *	\note
             *	You need to be familiar with GLFW mouse button codes in
             *	order to properly override this function.
             *	
             *	\param[in] key The key for which the event happened.
             *	\param[in] scancode The scancode for that key.
             *	\param[in] action The action that occurred.
             *	\param[in] mods The modifier keys that were pressed.
             */
            virtual void keyPressEvent(int key, int scancode, int action,
                int mods);

            /**
             *	This is called by the Application every time the screen 
             *	changes size. Since this is where the projection matrix
             *	should be modified, the Application already ensures that the
             *	dimensions of the windows are never less than 1.
             *	
             *	\param[in] width The window width.
             *	\param[in] height The window height.
             */
            virtual void screenResizeEvent(int width, int height);

            /**
             * This is called before a scene enters the active window. Any
             * global OpenGL states that your scene requires should be 
             * activated in here in order to prevent state leakage.
             */
            virtual void onSceneEnter();

            /**
             * This is called after a scene exits the active window. Any
             * global OpenGL states that you activatd in onSceneEnter should
             * be disabled here.
             */
            virtual void onSceneExit();

            /**
             *	This is called for every pass of the main loop. All that is
             *	given here is the current tick. It is the Scene's responsibility
             *	to handle the timing as is necessary.
             *	
             *	\warning
             *	The space between ticks is not even, so do not assume that 
             *	the delta between ticks will always be the same.
             *	
             *	\param[in] time The current tick.
             */
            virtual void updateScene(double time);

            /**
             *	Called after the Scene is updated by the Application so it
             *	can render.
             */
            virtual void renderScene();

            /**
             *	The Application checks this to determine what to do with the
             *	window in the event the user closes it.
             *	\retval true The Application closes the window.
             *	\retval false The Application hides the window. In this case,
             *	the user is free to setup a new scene and run it (for example).
             */
            virtual bool sceneEnded();

        protected:
            /**
             * Sets whether the cursor should be visible for the scene.
             * 
             * \param[in] enabled Whether the cursor is enabled or not.
             */
            void setCursorEnabled(bool enabled);

            /**
             * \var mProjection
             * The projection matrix for the Scene. This should be updated
             * inside screenResizeEvent.
             */
            atlas::math::Matrix4 mProjection;

            /**
             *	\var mView
             *	The view matrix for the Scene. This can be paired with the
             *	Camera class to provide a better viewing of objects.
             */
            atlas::math::Matrix4 mView;

            /**
             *	\var mTime
             *	The Time data for the Scene.
             */
            atlas::core::Time<> mTime;
        };
    }
}

#endif