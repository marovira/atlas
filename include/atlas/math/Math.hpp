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
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
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

        using Vector2 = glm::vec2;
        using Vector = glm::vec3;
        using Vector4 = glm::vec4;

        using Point2 = Vector2;
        using Point = Vector;
        using Point4 = Vector4;

        using Normal2 = Vector2;
        using Normal = Vector;
        using Normal4 = Vector4;

        using Matrix4 = glm::mat4;
        using Matrix3 = glm::mat3;

        using Quaternion = glm::quat;
    }
}

#endif