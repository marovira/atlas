#ifndef ATLAS_INCLUDE_ATLAS_CONSTANTS_HPP
#define ATLAS_INCLUDE_ATLAS_CONSTANTS_HPP

#pragma once

#include <glm/gtc/constants.hpp>
#include <limits>

#include "Atlas.hpp"

namespace atlas
{
    template <typename GenType = float>
    inline GenType atlasEpsilon()
    {
        return GenType(0.0001);
    }

    template <typename GenType = float>
    inline GenType atlasInfinity()
    {
        return std::numeric_limits<GenType>::max();
    }
}

#endif
