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
        typedef glm::vec3 Vector;
        typedef Vector Point;
        typedef Vector Normal;
        typedef glm::vec2 Point2;
    }
}

#endif