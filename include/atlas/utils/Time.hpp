#ifndef ATLAS_INCLUDE_ATLAS_UTILS_TIME_HPP
#define ATLAS_INCLUDE_ATLAS_UTILS_TIME_HPP

#pragma once

#include "Utils.hpp"

namespace atlas
{
    namespace utils
    {
        struct Time
        {
            Time() :
                totalTime(0.0f),
                deltaTime(0.0f),
                currentTime(0.0f)
            { }

            Time(Time const& t) = default;

            ~Time()
            { }

            float totalTime;
            float deltaTime;
            float currentTime;
        };
    }
}

#endif