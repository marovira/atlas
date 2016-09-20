/**
 *	\file Camera.hpp
 *	\brief Defines a simple camera interface.
 */

#ifndef ATLAS_INCLUDE_ATLAS_UTILS_CAMERA_HPP
#define ATLAS_INCLUDE_ATLAS_UTILS_CAMERA_HPP

#pragma once

#include "Utils.hpp"
#include "atlas/core/Core.hpp"
#include "atlas/math/Math.hpp"

namespace atlas
{
    namespace utils
    {
        /**
         *	\class Camera
         *	\brief Defines a simple camera interface to be used with Scenes.
         *	
         *	This class provides a simple way of integrating a camera into
         *	an existing Scene. To use it, simply subclass from Camera and
         *	implement it however you like. In order to allow the user to
         *	control the camera using the mouse (this is the default provided
         *	option) override the mouse functions and implement them. 
         *	
         *	The next step is to add your camera to your Scene and simply 
         *	invoke the mouse functions within the Scene's mouse functions 
         *	and everything should take care of itself.
         *	
         *	Finally, just remember to get the camera matrix and set it to
         *	the view matrix so it can be passed down to objects when they
         *	render.
         *	
         *	The CameraMovements enum defines the standard functions that 
         *	cameras should have. While this may not cover <i> all </i> possible
         *	cameras, it should suffice for most applications. With this,
         *	it is possible to fully implement a camera that behaves exactly
         *	like the one in Autodesk Maya.
         */
        class Camera
        {
        public:
            /**
             *	Empty constructor.
             */
            Camera();

            /**
             *	Default copy constructor.
             */
            Camera(Camera const& camera) = default;

            /**
             *	Virtual destructor.
             */
            virtual ~Camera();

            /**
             *	This function is called whenever the mouse is pressed and it
             *	generally stores the mouse point so it can be used later on
             *	to compute the movement to perform when the mouse is dragged.
             *	
             *	\param[in] point The current mouse position.
             */
            virtual void mouseDown(atlas::math::Point2 const& point);

            /**
             *	This is called when the mouse has been pressed and is being
             *	dragged. It only receives the current position for the mouse,
             *	since the camera state has been set when the mouse was
             *	initially pressed.
             *	
             *	\param[in] point The current mouse position.
             */
            virtual void mouseMove(atlas::math::Point2 const& point);

            /**
             *	Called when the mouse button is lifted. This generally 
             *	ends the motion and resets the camera state back to idle.
             */
            virtual void mouseUp();

            /**
             * Called when the mouse wheel is moved. The two coordinates of
             * the passed point correspond to the two axis offsets.
             * 
             * \param[in] offset The scrolled offset.
             */
            virtual void mouseScroll(atlas::math::Point2 const& offset);

            /**
             * Called when a key is pressed.
             * 
             * \param[in] key The key id that was pressed.
             */
            virtual void keyDown(int key);

            /**
             * Called when a key is released.
             * 
             * \param[in] key The id of the released key.
             */
            virtual void keyUp(int key);

            /**
             * Called whenever the camera needs to be updated in relation
             * to the current time.
             * 
             * \param[in] time The current time of the scene.
             */
            virtual void updateCamera(atlas::core::Time<> const& time);

            /**
             * Called whenever the screen size is changed so the camera bounds
             * can be updated as well.
             * 
             * \param[in] size The new window size.
             */
            virtual void updateCameraBounds(atlas::math::Point2 const& size);

            /**
             *	Resets the camera to its initial state.
             */
            virtual void resetCamera();

            /**
             *	Returns the final matrix that contains all of the camera
             *	transformations. This should eventually end as the 
             *	view matrix for the Scene.
             *	
             *	\return The matrix containing all camera transforms.
             */
            virtual math::Matrix4 getCameraMatrix() const;

            /**
             * Returns the current position of the camera (the eye value).
             * 
             * \return The camera position.
             */
            virtual atlas::math::Point getCameraPosition() const;

            /**
             * Returns the field of view of the camera.
             * 
             * \return The camera FOV.
             */
            virtual float getCameraFOV() const;
        };
    }
}

#endif