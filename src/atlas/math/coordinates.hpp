#pragma once

#include "glm.hpp"

namespace atlas::math
{
    glm::vec3 cartesian_to_spherical(glm::vec3 const& p)
    {
        float r     = glm::length(p);
        float theta = glm::atan(p.y / p.x);
        float phi   = glm::acos(p.z / r);
        return {r, theta, phi};
    }

    glm::vec3 spherical_to_cartesian(glm::vec3 const& p)
    {
        float x = p.r * glm::cos(p.y) * glm::sin(p.z);
        float y = p.r * glm::sin(p.y) * glm::sin(p.z);
        float z = p.r * glm::cos(p.z);
        return {x, y, z};
    }

    glm::vec3 cartesian_to_cylindrical(glm::vec3 const& p)
    {
        float r     = glm::length(glm::vec2{p});
        float theta = glm::atan(p.y / p.x);
        return {r, theta, p.z};
    }

    glm::vec3 cylindrical_to_cartesian(glm::vec3 const& p)
    {
        float x = p.x * glm::cos(p.y);
        float y = p.x * glm::sin(p.y);
        return {x, y, p.z};
    }

    glm::vec2 cartesian_to_polar(glm::vec2 const& p)
    {
        float r     = glm::length(p);
        float theta = glm::atan(p.y / p.x);
        return {r, theta};
    }

    glm::vec2 polar_to_cartesian(glm::vec2 const& p)
    {
        float x = p.r * glm::cos(p.y);
        float y = p.r * glm::sin(p.y);
        return {x, y};
    }
} // namespace atlas::math
