#ifndef ATLAS_INCLUDE_ATLAS_CORE_CONSTANTS_HPP
#define ATLAS_INCLUDE_ATLAS_CORE_CONSTANTS_HPP

#pragma once

#include "Core.hpp"

#include <glm/gtc/constants.hpp>
#include <limits>


namespace atlas
{
    namespace core
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
}

#endif