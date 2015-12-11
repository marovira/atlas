#ifndef ATLAS_INCLUDE_ATLAS_RGBCOLOUR_HPP
#define ATLAS_INCLUDE_ATLAS_RGBCOLOUR_HPP
#pragma once

#include <glm/vec3.hpp>
#include <glm/exponential.hpp>
#include <algorithm>

namespace atlas
{
    typedef glm::vec3 RGBColour;

    inline RGBColour powc(RGBColour const& c, float po)
    {
        return RGBColour(
            glm::pow(c.r, po),
            glm::pow(c.g, po),
            glm::pow(c.b, po));
    }

    inline RGBColour white()
    {
        return RGBColour(1.0f);
    }

    inline RGBColour black()
    {
        return RGBColour(0.0f);
    }

    inline RGBColour maxToOne(RGBColour const& c)
    {
        float max = std::max(c.r, std::max(c.g, c.b));
        return (max > 1.0f) ? c / max : c;
    }

    inline RGBColour clampToColour(RGBColour const& rawColour)
    {
        return RGBColour(
            (rawColour.r > 1.0f) ? 1.0f : rawColour.r,
            (rawColour.g > 1.0f) ? 1.0f : rawColour.g,
            (rawColour.b > 1.0f) ? 1.0f : rawColour.b);
    }
}

#endif