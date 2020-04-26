#pragma once

#include "Math.hpp"

namespace atlas::math
{
    Point cartesianToSpherical(Point const& p)
    {
        float r     = glm::length(p);
        float theta = glm::atan(p.y / p.x);
        float phi   = glm::acos(p.z / r);
        return {r, theta, phi};
    }

    Point sphericalToCartesian(Point const& p)
    {
        float x = p.r * glm::cos(p.y) * glm::sin(p.z);
        float y = p.r * glm::sin(p.y) * glm::sin(p.z);
        float z = p.r * glm::cos(p.z);
        return {x, y, z};
    }

    Point cartesianToCylindrical(Point const& p)
    {
        float r     = glm::length(Point2{p});
        float theta = glm::atan(p.y / p.x);
        return {r, theta, p.z};
    }

    Point cylindricalToCartesian(Point const& p)
    {
        float x = p.x * glm::cos(p.y);
        float y = p.x * glm::sin(p.y);
        return {x, y, p.z};
    }

    Point2 cartesianToPolar(Point2 const& p)
    {
        float r     = glm::length(p);
        float theta = glm::atan(p.y / p.x);
        return {r, theta};
    }

    Point2 polarToCartesian(Point2 const& p)
    {
        float x = p.r * glm::cos(p.y);
        float y = p.r * glm::sin(p.y);
        return {x, y};
    }
} // namespace atlas::math