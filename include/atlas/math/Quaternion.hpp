/**
 *	\file Quaternion.hpp
 *	\brief Includes GLM quaternion headers and typedefs the Quaternion class.
 *	
 *	This header includes all of the ncessary GLM headers to perform all
 *	quaternion operations. As a result, the GLM headers should not be included
 *	(unless there is a compelling reason to do so). Instead, the user should
 *	simply add this line:
 *	\code
 *	#include <atlas/math/Quaternion.hpp>
 *	\endcode
 *	and all quaternion operations will become available.
 */
#ifndef ATLAS_INCLUDE_ATLAS_MATH_QUATERNION_HPP
#define ATLAS_INCLUDE_ATLAS_MATH_QUATERNION_HPP

#pragma once

#include "Math.hpp"

#include <glm/gtc/quaternion.hpp>

namespace atlas
{
    namespace math
    {
        /**
         * \typedef Quaternion
         * Defines the typedef for the GLM quat.
         */
        typedef glm::quat Quaternion;
    }
}

#endif