#ifndef ATLAS_INCLUDE_ATLAS_UTILS_CAMERA_HPP
#define ATLAS_INCLUDE_ATLAS_UTILS_CAMERA_HPP

#pragma once

#include "atlas/math/Matrix.hpp"
#include "atlas/math/Vector.hpp"
#include "atlas/math/Ray.hpp"

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
             *	Standard empty constructor.
             */
            Camera();

            /**
             *	Standard default copy constructor.
             */
            Camera(Camera const& camera) = default;

            /**
             *	Standard virtual destructor.
             */
            virtual ~Camera();

            /**
             *	\enum CameraMovements
             *	Provides a set of motions that most cameras should provide.
             *	
             *	\var IDLE
             *	The camera is at rest. There is no motion happening (default 
             *	state).
             *	\var TUMBLE
             *	This rotates the camera around a fixed point (similar to an
             *	arcball).
             *	\var TRACK
             *	Moves the camera within its viewing plane without changing
             *	the viewing angle (also known as panning).
             *	\var DOLLY
             *	Not to be confused with zooming, this moves the camera 
             *	closer or further away from its viewing point.
             */
            enum class CameraMovements
            {
                IDLE = 0,
                TUMBLE,
                TRACK,
                DOLLY
            };

            /**
             *	This function is called whenever the mouse is pressed and it
             *	generally stores the mouse point so it can be used later on
             *	to compute the movement to perform when the mouse is dragged.
             *	
             *	\param[in] point The current mouse position.
             *	\param[in] movement The type of movement to perform.
             */
            virtual void mouseDown(math::Point2 const& point, 
                CameraMovements movement);

            /**
             *	This is called when the mouse has been pressed and is being
             *	dragged. It only receives the current position for the mouse,
             *	since the camera state has been set when the mouse was
             *	initially pressed.
             *	
             *	\param[in] point The current mouse position.
             */
            virtual void mouseDrag(math::Point2 const& point);

            /**
             *	Called when the mouse button is lifted. This generally 
             *	ends the motion and resets the camera state back to idle.
             */
            virtual void mouseUp();

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
            virtual math::Matrix4 getCameraMatrix();
        };
    }
}

#endif