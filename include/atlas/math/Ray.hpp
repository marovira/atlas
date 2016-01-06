#ifndef ATLAS_INCLUDE_ATLAS_MATH_RAY_HPP
#define ATLAS_INCLUDE_ATLAS_MATH_RAY_HPP

#pragma once

#include "Math.hpp"
#include "Vector.hpp"

namespace atlas
{
    namespace math
    {
        struct Ray
        {
            Ray() :
                o(0.0f),
                d(0.0f, 1.0f, 0.0f)
            { }

            Ray(Point const& origin, Vector const& dir) :
                o(origin),
                d(dir)
            { }

            Ray(Ray const& ray) = default;
            ~Ray()
            { }

            Ray& operator=(Ray const& rhs) = default;

            Point operator()(float t) const
            {
                return o + (t * d);
            }

            bool operator==(Ray const& rhs)
            {
                return (o == rhs.o) && (d == rhs.d);
            }

            bool operator!=(Ray const& rhs)
            {
                return !((*this) == rhs);
            }

            Point o;
            Vector d;
        };

    }
}

#endif