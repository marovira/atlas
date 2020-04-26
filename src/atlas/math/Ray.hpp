#pragma once

#include "Math.hpp"

namespace atlas::math
{
    template<typename T>
    struct Ray
    {
        Ray() : o{0.0f}, d{0.0f}
        {}

        Ray(T const& origin, T const& dir) : o{origin}, d{dir}
        {}

        T operator()(float t) const
        {
            return o + (t * d);
        }

        bool operator==(Ray const& rhs) const
        {
            return (o == rhs.o) && (d == rhs.d);
        }

        bool operator!=(Ray const& rhs) const
        {
            return !(*this == rhs);
        }

        T o;
        T d;
    };
} // namespace atlas::math
