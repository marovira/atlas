/**
 *	\file Application.hpp
 *	\brief Defines the Application class that handles windows and contexts.
 */

#ifndef ATLAS_INCLUDE_ATLAS_UTILS_APPLICATION_HPP
#define ATLAS_INCLUDE_ATLAS_UTILS_APPLICATION_HPP

#pragma once

#include "Utils.hpp"
#include "atlas/gl/GL.hpp"
#include "atlas/core/GLFW.hpp"

#include <string>
#include <memory>

namespace atlas
{
    namespace utils
    {
        /**
         *	\class Application
         *	\brief Defines a class that wraps all of the GLFW windowing system.
         *	
         *	This class is the main object for all applications built with 
         *	Atlas.
         *	
         *	The logic is the following: for each program there is a single
         *	application, which is in charge of handling all of the windows
         *	and rendering contexts that the user requires. The application
         *	also has a list of Scenes that are running and rendering to the
         *	specified windows and contexts. Whenever user input occurs, it
         *	is forwarded to the Scenes so they can handle it themselves.
         *	
         *	In order to make all of this happen, the Application class is 
         *	implemented as a static singleton, and is thereby accessible from
         *	anywhere within the program by just including the header.
         *	
         *	The provided macro serves as a way to prevent having to write the
         *	full path to the getInstance function.
         *	
         *	Whenever a user starts a new program, the general order of 
         *	operations should be similar to this example:
         *	\code
         *	#include "MyScene.hpp"
         *	int main()
         *	{	    
         *	    APPLICATION.createWindow(400, 400, "Some title");
         *	    // Initialize the scene
         *	    auto scene = new MyScene;
         *	    APPLICATION.addScene(scene);
         *	    APPLICATION.runApplication();
         *	
         *	    return 0;
         *	}
         *	\endcode
         *	
         *	The call to \c createWindow sets up the GLFW window and creates
         *	the appropriate rendering context.
         *	The next call to \c addScene simply adds the scene to the queue.
         *	Finally, \c runApplication enters the main loop which then handles
         *	all of the logic for updating, rendering, and handling all input.
         *	After the user closes the window, the program cleans up everything
         *	and exits.
         *	
         *	\note
         *	Support for multiple windows/rendering contexts is currently not
         *	available. This will be removed when it is.
         *	
         *	\note
         *	While the Scenes can use whatever keys they want from the keyboard
         *	to specify their user interaction, be aware that the following keys
         *	are used by the Applciation class:
         *	\li Esc: Used to close the application.
         *	\li Tab: Used to swap to the next scene.
         *	\li Shift + Tab: Used to swap to the previous scene.
         */
        class Application
        {
            Application();
            ~Application();

            Application(Application const&) = delete;
            void operator =(Application const&) = delete;

        public:
            /**
             *	Returns the instance of the application.
             *	\return The application instance.
             */
            static Application& getInstance();

            /**
             * Creates a new window with the specified settings.
             * 
             * \param[in] settings The window settings to use.
             */
            void createWindow(WindowSettings const& settings);

            /**
             *	Creates a new window with rendering context.
             *	
             *	\param[in] width The width of the new window.
             *	\param[in] height The height of the new window.
             *	\param[in] title The title of the new window.
             *	\param[in] contextVersionMajor The OpenGL context major version
             *	number to use. Defaults to 3.
             *	\param[in] contextVersionMinor The OpenGL context minor version
             *	number to use. Defaults to 3.
             */
            void createWindow(int width, int height, std::string const& title,
                int contextVersionMajor = 3, int contextVersionMinor = 3);

            /**
             *	Enters the application main loop and runs the scenes. Exits
             *	whenever the user closes the window.
             */
            void runApplication();

            /**
             *	Returns the current position of the cursor in screen
             *	coordinates.
             *	
             *	\param[out] x The x position of the cursor.
             *	\param[out] y The y position of the cursor.
             */
            void getCursorPosition(double* x, double *y);

            /**
             *	Adds a new scene to the queue. As soon as the application
             *	takes ownership of the scene, it will also be in charge 
             *	of deleting it.
             *	
             *	\param[in] scene The scene to add.
             */
            void addScene(ScenePointer scene);

            /**
             *	Moves to the next scene in the queue. If there is only 
             *	one scene, this does nothing.
             *	
             *	\note
             *	When scenes are shifted, the current tick is stored and
             *	preserved for when that particular scene is displayed again.
             *	In other words, time is "frozen" for scenes when they leave
             *	the screen and is reactivated when they return.
             */
            void nextScene();

            /**
             *	Moves to the previous scene in the queue. If there is only 
             *	one scene, this does nothing.
             *	
             *	\note
             *	When scenes are shifted, the current tick is stored and
             *	preserved for when that particular scene is displayed again.
             *	In other words, time is "frozen" for scenes when they leave
             *	the screen and is reactivated when they return.
             */
            void previousScene();

            /**
             *	Returns the scene that is currently being played.
             *	
             *	\return The current scene.
             */
            Scene* getCurrentScene() const;

            /**
             * Returns the current window.
             * 
             * \return The current window.
             */
            GLFWwindow* getCurrentWindow() const;

        private:
            struct ApplicationImpl;
            std::unique_ptr<ApplicationImpl> mImpl;
        };
    }
}

#endif