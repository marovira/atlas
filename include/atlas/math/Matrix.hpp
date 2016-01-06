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
        typedef glm::mat4 Matrix4;
        typedef glm::mat3 Matrix3;
    }
}

#endif
