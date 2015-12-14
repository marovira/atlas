#ifndef ATLAS_INCLUDE_ATLAS_SHADEREC_HPP
#define ATLAS_INCLUDE_ATLAS_SHADEREC_HPP

#pragma once

#include "Ray.hpp"
#include "RGBColour.hpp"

namespace atlas
{
    struct ShadeRec
    {
        ShadeRec() :
            didHitObject(false)
        { }

        bool didHitObject;
        atlas::Point hitPoint;
        atlas::Point localHitPoint;
        atlas::Normal normal;
        Ray ray;
        int depth;
        RGBColour colour;
        float t;
        atlas::Point2 textureCoord;
    };
}

#endif