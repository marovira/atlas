/**
 *	\file Math.hpp
 * \brief Main header file for math module.
 * 
 * Defines forward declarations for all classes, structs, and typedefs in the 
 * module.
 */

#ifndef ATLAS_INCLUDE_ATLAS_MATH_HPP
#define ATLAS_INCLUDE_ATLAS_MATH_HPP

#pragma once

#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/constants.hpp>

namespace atlas
{
    namespace math
    {
        template <typename GenType = float>
        class RandomGenerator;

        struct Ray;

        /**
         * \typedef Vector2
         * Defines an alias for a 2D vector.
         */
        using Vector2 = glm::vec2;

        /**
         * \typedef Vector
         * Defines an alias for a 3D vector.
         */
        using Vector = glm::vec3;

        /**
         * \typedef Vector4
         * Defines an alias for a 4D vector.
         */
        using Vector4 = glm::vec4;

        /**
         * \typedef Point2
         * Defines an alias for a 2D point.
         */
        using Point2 = Vector2;

        /**
         * \typedef Point
         * Defines an alias for a 3D point.
         */
        using Point = Vector;

        /**
         * \typedef Point4
         * Defines an alias for a 4D point.
         */
        using Point4 = Vector4;

        /**
         * \typedef Normal2
         * Defines an alias for a 2D normal.
         */
        using Normal2 = Vector2;

        /**
         * \typedef Normal
         * Defines an alias for a 3D normal.
         */
        using Normal = Vector;

        /**
         * \typedef Normal4
         * Defines an alias for a 4D normal.
         */
        using Normal4 = Vector4;

        /**
         * \typedef Matrix2
         * Defines an alias for a 2x2 matrix.
         */
        using Matrix2 = glm::mat2;

        /**
         * \typedef Matrix3
         * Defines an alias for a 3x3 matrix.
         */
        using Matrix3 = glm::mat3;

        /**
         * \typedef Matrix4
         * Defines an alias for a 4x4 matrix.
         */
        using Matrix4 = glm::mat4;

        /**
         * \typedef Quaternion
         * Defines an alias for a quaternion.
         */
        using Quaternion = glm::quat;
    }
}

#endif