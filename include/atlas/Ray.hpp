#ifndef ATLAS_INCLUDE_ATLAS_RAY_HPP
#define ATLAS_INCLUDE_ATLAS_RAY_HPP

#pragma once

#include "Vector.hpp"

namespace atlas
{
    struct Ray
    {
        Ray();
        Ray(atlas::Point const& origin, atlas::Vector const& dir);
        Ray(Ray const& ray) = default;
        ~Ray();

        Ray& operator=(Ray const& rhs) = default;

        atlas::Point operator()(float t) const;
        bool operator==(Ray const& rhs);
        bool operator!=(Ray const& rhs);

        atlas::Point o;
        atlas::Vector d;
    };
}

#endif