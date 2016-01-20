/**
 *	\file Vector.hpp
 *	\brief Includes GLM vector headers and typedefs for Vector classes.
 *	
 *	This header includes all of the necessary GLM headers to perform all 
 *	vector operations. As a result, the GLM headers should not be included
 *	directly (unless there is a compelling reason to do so). Instead, the user
 *	should simply add this line:
 *	\code
 *	#include <atlas/math/Vector.hpp>
 *	\endcode
 *	and all vector operations will become available.
 *	
 *	\note
 *	The typedefs given here are given from a 3D mathematical perspective. As
 *	a result, Vector, Point, and Normal all refer to 3D vectors. The reason
 *	why they are typedefed differently is to provide syntax clarity as to what
 *	each structure is used for.
 */
#ifndef ATLAS_INCLUDE_ATLAS_MATH_VECTOR_HPP
#define ATLAS_INCLUDE_ATLAS_MATH_VECTOR_HPP

#pragma once

#include "Math.hpp"

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/norm.hpp>


namespace atlas
{
    namespace math
    {
        /**
         *	\typedef Vector
         *	A 3D vector.
         */
        typedef glm::vec3 Vector;


        /**
         *	\typedef Vector4
         *	A 4D vector.
         */
        typedef glm::vec4 Vector4;

        /**
         *	\typedef Point
         *	A point in 3D space.
         */
        typedef Vector Point;

        /**
         *	\typedef Normal
         *	A 3D normal.
         */
        typedef Vector Normal;

        /**
         *	\typedef Point2
         *	A point in 2D space.
         */
        typedef glm::vec2 Point2;
    }
}

#endif