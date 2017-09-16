/**
 *	\file Geometry.hpp
 *	\brief Defines the main interface for Geometry classes.
 */

#ifndef ATLAS_INCLUDE_ATLAS_UTILS_GEOMETRY_HPP
#define ATLAS_INCLUDE_ATLAS_UTILS_GEOMETRY_HPP

#pragma once

#include "Utils.hpp"
#include "atlas/core/Time.hpp"
#include "atlas/math/Ray.hpp"
#include "atlas/gl/Shader.hpp"
#include "atlas/math/Math.hpp"

#include <vector>
#include <map>
#include <string>
#include <memory>

namespace atlas
{
    namespace utils
    {
        /**
         *	\class Geometry
         *	\brief Provides a simple interface for handling geometry.
         *	
         *	So for every Scene that we create we ultimately want to be able to
         *	draw something. For animation programs we would also like to update
         *	the objects on the scene or transform them as required. This is 
         *	where the Geometry class steps in.
         *	
         *	Essentially, each Geometry class is equipped with the following:
         *	\li An array of shaders.
         *	\li A map for uniform variables.
         *	\li A model matrix.
         *	
         *	The array of shaders is meant for objects that require multiple
         *	shaders to render (such as a shadow and a texture shader for 
         *	example). The user can simply add them in here and then activate
         *	them as needed inside of the renderGeometry function.
         *	
         *	The map for uniforms is meant to remove the overhead of accessing
         *	uniform variables in shaders by storing their locations when
         *	the shaders are created. The variables can then be accessed by
         *	their names as needed.
         *	
         *	Finally, the model matrix is the one that stores all of the 
         *	transformations for that particular object.
         *	
         *	The flow for using Geometries inside the Scene is as follows:
         *	\li Create the Geometry inside the Scene's constructor.
         *	\li Call updateGeometry inside the Scene's updateScene function.
         *	\li Call renderGeometry inside the Scene's renderScene function.
         *	
         *	The last two provided functions can be called anywhere that is
         *	required. The intersectRay function can be used for ray tracing
         *	or for ray casting (a selection technique for UIs).
         */
        class Geometry
        {
        public:
            /**
             *	Empty constructor.
             */
            Geometry();

            /**
             * Move constructor. Since none of the objects that are associated
             * with a Geometry can be copied (shaders, buffers, etc.) it requires
             * move semantics in order to be moved from function to function.
             * 
             * \param[in] g The geometry to move.
             */
            Geometry(Geometry&& g) = default;

            /**
             * Move assignment operator.
             * 
             * \param[in] rhs The geometry to move.
             * 
             * \return The moved geometry.
             */
            Geometry& operator=(Geometry&& rhs) = default;

            /**
             *	Virtual destructor.
             */
            virtual ~Geometry();

            /**
             *	Called whenever the geometry needs to be updated (like in
             *	animations for example). The Time struct contains all of the
             *	information required to perform complex animations, and is
             *	handled by the Scene and passed down through here.
             *	
             *	\param[in] t The time struct containing all timing information.
             */
            virtual void updateGeometry(atlas::core::Time<> const& t);

            /**
             *	This is called whenever the renderScene is called inside the
             *	Scene class. The projection and view matrices are passed down
             *	so that the MVP matrix can be computed and passed down to the
             *	shaders.
             *	
             *	\param[in] projection The projection matrix.
             *	\param[in] view The view matrix.
             */
            virtual void renderGeometry(math::Matrix4 const& projection, 
                math::Matrix4 const& view);

            /**
             * This is called whenever the renderScene is called inside the
             * Scene class. This function assumes that uniform blocks 
             * (or no matrices) are being used.
             * 
             */
            virtual void renderGeometry();

            /**
             * This function can be used to draw any GUI related elements for
             * your geometry, such as rendering modes or other features. Make
             * sure that you call ImGui::Render() after you use this function.
             */
            virtual void drawGui();

            /**
             *	Called whenever the geometry needs to be transformed by a 
             *	specific matrix. This should then be multiplied into the
             *	model matrix in some way.
             *	
             *	\param[in] t The transformation matrix.
             */
            virtual void transformGeometry(math::Matrix4 const& t);

            /**
             *	This is called whenever the geometry needs to be restored
             *	to its initial state. Use it to restart things like
             *	animations.
             */
            virtual void resetGeometry();

            /**
             * Called whenever the geometry needs to reload its shaders.
             */
            virtual void reloadShaders();

        protected:
            /**
             *	This tests the object for intersection against the provided
             *	ray. The tMin variable is used to store the smallest t value
             *	for intersection.
             *	
             *	\param[in] ray The ray against which to intersect.
             *	\param[out] tMin The minimum t value of intersection
             *	\return True if the ray intersects the object, false otherwise.
             */
            virtual bool intersectRay(math::Ray const& ray, float& tMin);

            /**
             *	\var mShaders
             *	The list of shaders for the specific object.
             */
            std::vector<atlas::gl::Shader> mShaders;

            /**
             *	\typedef UniformKey
             *	Makes adding new uniform pairs easier and more readable.
             */
            typedef std::pair<std::string, GLint> UniformKey;

            /**
             *	\var mUniforms
             *	The map of uniform variables for the shaders. The variables
             *	can be accessed by their name.
             */
            std::map<std::string, GLint> mUniforms;

            /**
             *	\var mModel
             *	The model matrix for the object.
             */
            math::Matrix4 mModel;
        };
    }
}

#endif
