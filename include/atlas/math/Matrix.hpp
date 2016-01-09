/**
 *	\file Matrix.hpp
 *	\brief Includes GLM matrix headers and typedefs the Matrix classes.
 *	
 *	This header includes all of the necessary GLM headers to perform all
 *	matrix operations. As a result, the GLM headers should not be included
 *	directly (unless there is a compelling reason to do so). Instead, the 
 *	user should simply add this line:
 *	\code
 *	#include <atlas/math/Matrix.hpp>
 *	\endcode
 *	and all matrix operations will become available.
 */
#ifndef ATLAS_INCLUDE_ATLAS_MATH_MATRIX_HPP
#define ATLAS_INCLUDE_ATLAS_MATH_MATRIX_HPP

#pragma once

#include "Math.hpp"

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

namespace atlas
{
    namespace math
    {
        /**
         *	\typedef Matrix4
         *	Defines the typedef for the 4x4 GLM matrix.
         */
        typedef glm::mat4 Matrix4;

        /**
         *	\typedef Matrix3
         *	Defines the typedef for the 3x3 GLM matrix.
         */
        typedef glm::mat3 Matrix3;
    }
}

#endif
